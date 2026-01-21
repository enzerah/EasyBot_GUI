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
                auto equippedRing = proto->getInventoryItem(localPlayer, Otc::InventorySlotRing);
                if (data.condition == 0) {
                    if (hpPct < data.value){
                        if (equippedRing) {
                            auto ringId = proto->getItemId(equippedRing);
                            if (ringId != data.itemId) proto->equipItemId(data.itemId, -1);
                        } else {
                            proto->equipItemId(data.itemId, -1);
                        }
                    } else if (hpPct > data.value + 15 || hpPct == 100) {
                        if (equippedRing) {
                            auto ringId = proto->getItemId(equippedRing);
                            if (ringId == data.itemId) proto->equipItemId(data.itemId, -1);
                        }
                    }
                } else {
                    if (mpPct < data.value){
                        if (equippedRing) {
                            auto ringId = proto->getItemId(equippedRing);
                            if (ringId != data.itemId) proto->equipItemId(data.itemId, -1);
                        } else {
                            proto->equipItemId(data.itemId, -1);
                        }
                    } else if (mpPct > data.value + 15  || mpPct == 100) {
                        if (equippedRing) {
                            auto ringId = proto->getItemId(equippedRing);
                            if (ringId == data.itemId) proto->equipItemId(data.itemId, -1);
                        }
                    }
                }
            } else if (other == "Amulet") {
                auto equippedAmulet = proto->getInventoryItem(localPlayer, Otc::InventorySlotNecklace);
                if (data.condition == 0) {
                    if (hpPct < data.value){
                        if (equippedAmulet) {
                            auto ringId = proto->getItemId(equippedAmulet);
                            if (ringId != data.itemId) proto->equipItemId(data.itemId, -1);
                        } else {
                            proto->equipItemId(data.itemId, -1);
                        }
                    } else if (hpPct > data.value + 15 || hpPct == 100) {
                        if (equippedAmulet) {
                            auto ringId = proto->getItemId(equippedAmulet);
                            if (ringId == data.itemId) proto->equipItemId(data.itemId, -1);
                        }
                    }
                } else {
                    if (mpPct < data.value){
                        if (equippedAmulet) {
                            auto ringId = proto->getItemId(equippedAmulet);
                            if (ringId != data.itemId) proto->equipItemId(data.itemId, -1);
                        } else {
                            proto->equipItemId(data.itemId, -1);
                        }
                    } else if (mpPct > data.value + 15 || mpPct == 100) {
                        if (equippedAmulet) {
                            auto ringId = proto->getItemId(equippedAmulet);
                            if (ringId == data.itemId) proto->equipItemId(data.itemId, -1);
                        }
                    }
                }
            }
            msleep(100);
        }

        msleep(100);
    }
}
