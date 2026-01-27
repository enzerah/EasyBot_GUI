#include "BotEngine.h"

#include "proto_functions_client.h"
BotEngine* BotEngine::instance{nullptr};
std::mutex BotEngine::mutex;


BotEngine::BotEngine() = default;

BotEngine* BotEngine::getInstance()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr)
    {
        instance = new BotEngine();
    }
    return instance;
}

std::vector<int> BotEngine::countItems(std::vector<int> itemId) {
    std::vector<int> count(itemId.size());
    auto containers = proto->getContainers();
    for (auto container : containers) {
        auto items = proto->getItems(container);
        for (auto item : items) {
            for (int i = 0; i < count.size(); i++) {
                if (itemId[i] == proto->getItemId(item)) {
                    auto itemCount = proto->getCount(item);
                    count[i] += itemCount;
                }
            }
        }
    }
    return count;
}

void BotEngine::depositItems(std::vector<int> itemId, std::string containerToDeposit) {
    auto containers = proto->getContainers();
    std::transform(containerToDeposit .begin(), containerToDeposit .end(), containerToDeposit .begin(), ::tolower);
    Position savedPos = {};
    for (auto container : containers) {
        auto containerName = proto->getContainerName(container);
        std::transform(containerName .begin(), containerName .end(), containerName .begin(), ::tolower);
        if (containerName == containerToDeposit) {
            savedPos = proto->getSlotPosition(container, 0);
            break;
        }
    }
    containers = proto->getContainers();
    for (auto container : containers) {
        auto containerName = proto->getContainerName(container);
        std::transform(containerName .begin(), containerName .end(), containerName .begin(), ::tolower);
        if (containerName == containerToDeposit) continue;
        auto items = proto->getItems(container);
        for (auto item : items) {
            for (int i = 0; i < itemId.size(); i++) {
                if (itemId[i] == proto->getItemId(item)) {
                    auto itemCount = proto->getCount(item);
                    proto->move(item, savedPos, itemCount);
                    Sleep(1000);
                }
            }
        }
    }
}

void BotEngine::findDeposit(std::vector<int> depoIds) {
    auto localPlayer = proto->getLocalPlayer();
    Position playerPos = proto->getPosition(localPlayer);
    bool found = false;
    for (int x = -7; x <= 7 && !found; ++x) {
        for (int y = -7; y <= 7 && !found; ++y) {
            if (x ==0 && y == 0) continue;
            Position checkPos;
            checkPos.x = playerPos.x + x;
            checkPos.y = playerPos.y + y;
            checkPos.z = playerPos.z;

            auto tile = proto->getTile(checkPos);
            auto thing = proto->getTopUseThing(tile);
            auto itemId = proto->getItemId(thing);
            for (int depoId : depoIds) {
                if (itemId == depoId) {
                    Position itemPos = checkPos;
                    Position standPos;
                    for (int depoX = -1; depoX <= 1; ++depoX) {
                        if (depoX == 0) continue;
                        standPos.x = itemPos.x + depoX;
                        standPos.y = itemPos.y;
                        standPos.z = itemPos.z;
                        auto path = proto->findPath(playerPos, standPos, 100, 0);
                        if (path.empty()) continue;
                        found = true;
                    }
                    if (!found) {
                        for (int depoY = -1; depoY <= 1; ++depoY) {
                            if (depoY == 0) continue;
                            standPos.x = itemPos.x;
                            standPos.y = itemPos.y + depoY;
                            standPos.z = itemPos.z;
                            auto path = proto->findPath(playerPos, standPos, 100, 0);
                            if (path.empty()) continue;
                            found = true;
                        }
                    }
                    if (!found) continue;
                    proto->autoWalk(localPlayer, standPos, false);
                    bool reached = false;
                    // Waits 10 sec
                    for (int i = 0; i < 50; ++i) {
                        Position currentPos = proto->getPosition(localPlayer);
                        if (currentPos.x == standPos.x && currentPos.y == standPos.y && currentPos.z == standPos.z){
                            reached = true;
                            break;
                        }
                        Sleep(200);
                    }
                    if (reached) {
                        proto->use(thing);
                        Sleep(1000);
                        return;
                    }
                }
            }
        }
    }
}



