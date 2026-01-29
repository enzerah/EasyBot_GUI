#include "LootItems_Thread.h"
#include "../../BotEngine.h"

void LootItems_Thread::updateData(std::vector<Item> items)
{
    QMutexLocker locker(&m_mutex);
    m_items = items;
}

void LootItems_Thread::run() {
    while (!isInterruptionRequested()) {
        // Signal to start loot
        if (g_Engine->isLooting) {
            m_mutex.lock();
            auto itemsCopy = m_items;
            m_mutex.unlock();
            
            for (auto &item : itemsCopy) {
                auto destination = item.destination;
                auto containers = proto->getContainers();
                for (auto container : containers) {
                    auto container_name = proto->getContainerName(container);
                    std::transform(container_name.begin(), container_name.end(), container_name.begin(), ::tolower);
                    if (container_name == destination) {
                        auto containerPos = proto->getSlotPosition(container, 0);
                        item.itemPosition = containerPos;
                    }
                }
            }
            for (auto item : itemsCopy) {
                auto destination = item.destination;
                auto itemID = item.itemID;
                auto destPos = item.itemPosition;
                auto sleepTime = item.delay;
            }
            g_Engine->isLooting = false;
        }
        msleep(50);
    }
}




