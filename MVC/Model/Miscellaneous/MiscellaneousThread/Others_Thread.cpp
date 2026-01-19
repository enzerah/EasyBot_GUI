#include "Others_Thread.h"

void Others_Thread::updateCondition(const std::string& optionType, const int& itemId, const int& condition,
    const int& value, bool state) {
    mutex.lock();
    if (state) {
        activeOthers[optionType] = {itemId, condition, value};
    } else {
        activeOthers.erase(optionType);
    }
    mutex.unlock();
}

bool Others_Thread::hasActiveOthers() {
    mutex.lock();
    bool empty = activeOthers.empty();
    mutex.unlock();
    return !empty;
}

void Others_Thread::run()
{
    while (!isInterruptionRequested()) {
        mutex.lock();
        std::map<std::string, OtherData> others = activeOthers;
        mutex.unlock();

        if (others.empty()) break;
        auto localPlayer = proto->getLocalPlayer();
        auto currentHp = proto->getHealth(localPlayer);
        auto maxHp = proto->getMaxHealth(localPlayer);
        auto currentMp = proto->getMana(localPlayer);
        auto maxMp = proto->getMaxMana(localPlayer);
        auto hpPct = (currentHp / maxHp)*100;
        auto mpPct = (currentMp / maxMp)*100;

        for (auto it = others.begin(); it != others.end(); ++it) {
            std::string other = it->first;
            OtherData data = it->second;


            if (other == "Ammo") {
                auto equippedAmmo = proto->getInventoryItem(localPlayer, Otc::InventorySlotAmmo);
                if (!equippedAmmo) {
                    proto->equipItemId(data.itemId, -1);
                }
            } else if (other == "Ring") {
                // 1 - HP%
                if (data.condition == 1) {
                    if (hpPct < data.value){
                        proto->equipItemId(data.itemId, -1);
                    }
                } else if (data.condition == 2)
                {
                    if (mpPct < data.value){
                        proto->equipItemId(data.itemId, -1);
                    }
                }
            } else if (other == "Amulet") {
                // 1 - HP%
                if (data.condition == 1) {
                    if (hpPct < data.value){
                        proto->equipItemId(data.itemId, -1);
                    }
                } else if (data.condition == 2)
                {
                    if (mpPct < data.value){
                        proto->equipItemId(data.itemId, -1);
                    }
                }
            }
            msleep(100);
        }

        msleep(100);
    }
}
