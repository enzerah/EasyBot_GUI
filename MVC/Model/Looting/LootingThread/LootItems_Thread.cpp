#include "LootItems_Thread.h"
#include "../../BotEngine.h"
void LootItems_Thread::run() {
    if (items.empty()) return;
    while (!isInterruptionRequested()) {
        // Signal to start loot
        if (engine->isLooting) {
            for (auto &item : items) {
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
            for (auto item : items) {
                auto destination = item.destination;
                auto itemID = item.itemID;
                auto destPos = item.itemPosition;
                auto sleepTime = item.delay;
            }
            engine->isLooting = false;
        }
        msleep(50);
    }
}




