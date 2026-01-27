#include "StartHealing_Thread.h"


void StartHealing_Thread::updateData(std::vector<Heal> heals) {
    QMutexLocker locker(&m_mutex);
    m_heals = heals;
}

void StartHealing_Thread::run() {
    size_t index = 0;
    while (!isInterruptionRequested()) {
        auto localPlayer = proto->getLocalPlayer();
        auto current_hp = proto->getHealth(localPlayer);
        auto current_mp = proto->getMana(localPlayer);
        auto max_hp = proto->getMaxHealth(localPlayer);
        auto max_mp = proto->getMaxMana(localPlayer);
        double current_hp_pc = current_hp / max_hp * 100;
        double current_mp_pc = current_mp / max_mp * 100;
        
        m_mutex.lock();
        if (m_heals.empty()) {
            m_mutex.unlock();
            msleep(10);
            continue;
        }
        auto heal = m_heals[index];
        index = (index + 1) % m_heals.size();
        m_mutex.unlock();
        
        if (heal.condition == "HP%") {
            if (heal.below >= current_hp_pc && current_hp_pc >= heal.above &&
                current_mp >= heal.minimum) {
                if (heal.action == "Say") {
                    proto->talk(heal.heal);
                }
                else if (heal.action == "Use") {
                    int itemId = std::stoi(heal.heal);
                    proto->useInventoryItemWith(itemId, localPlayer);
                    proto->useInventoryItem(itemId);

                }
            }
        }
        if (heal.condition == "MP%") {
            if (heal.below >= current_mp_pc && current_mp_pc >= heal.above &&
                current_hp_pc >= heal.minimum) {
                if (heal.action == "Say") {
                    proto->talk(heal.heal);
                } else if (heal.action == "Use") {
                    int itemId = std::stoi(heal.heal);
                    proto->useInventoryItemWith(itemId, localPlayer);
                    proto->useInventoryItem(itemId);
                }
            }
        }
        msleep(10);
    }
}
