#include "FollowWaypoints_Thread.h"
#include <QElapsedTimer>
#include <iostream>
#include "../../BotEngine.h"
#include "../../../../LuaEngine.h"

void FollowWaypoints_Thread::run() {
    if (waypoints.empty()) return;

    findClosestWaypoint();

    size_t lastIndex = index;

    QElapsedTimer stuckTimer;
    stuckTimer.start();

    Position walkingTo{};
    bool isAutoWalking = false;


    while (!isInterruptionRequested()) {
        if (engine->hasTarget) stuckTimer.restart();
        auto localPlayer = proto->getLocalPlayer();
        auto playerPos = proto->getPosition(localPlayer);
        isAutoWalking = proto->isAutoWalking(localPlayer);

        auto wpt = waypoints[index];
        if (wpt.option == "Label") {
            index = (index + 1) % waypoints.size();
            emit indexUpdate_signal(static_cast<int>(index));
            continue;
        }

        // If we are stuck on same waypoint for more than 20 seconds
        if (index != lastIndex) {
            lastIndex = index;
            stuckTimer.restart();
        } else if (stuckTimer.hasExpired(10000)) {
            findNextValidWaypoint();
            lastIndex = index;
            stuckTimer.restart();
            emit indexUpdate_signal(static_cast<int>(index));
        }
        // Only walks if we do not have target, or we want to make Lure
        if (!engine->hasTarget || wpt.option == "Lure") {
            if (wpt.position.z != playerPos.z && wpt.direction == "C" && wpt.option == "Stand") {
                findNextValidWaypoint();
                stuckTimer.restart();
                continue;
            }
            // If walkingTo waypoint is not the same as current wpt we set canWalk variable to True
            if (wpt.position.x != walkingTo.x || wpt.position.y != walkingTo.y) {
                isAutoWalking = false;
            }
            // If our character is not walking already
            if (!isAutoWalking) {
                // Saving to variable walkingTo current wptPos
                walkingTo = wpt.position;
                if (wpt.option == "Stand" || wpt.option == "Lure" || wpt.option == "Node") performWalk(wpt, localPlayer, playerPos);
                if (wpt.option == "Use") performUse(wpt, localPlayer, playerPos);
                if (wpt.option == "Action") performAction(wpt);
            } else if (stuckTimer.hasExpired(5000)) {
                if (wpt.option == "Stand" || wpt.option == "Lure" || wpt.option == "Node") performWalk(wpt, localPlayer, playerPos);
            }
        }
        msleep(10);
    }
}

void FollowWaypoints_Thread::performWalk(Waypoint wpt, uintptr_t localPlayer, Position playerPos) {
    // If Position is already reached go to next wpt.
    if (wpt.option == "Node") {
        int dist = std::max(std::abs(static_cast<int>(playerPos.x) - static_cast<int>(wpt.position.x)),
            std::abs(static_cast<int>(playerPos.y) - static_cast<int>(wpt.position.y)));
        if (dist <= 2) {
            index = (index + 1) % waypoints.size();
            emit indexUpdate_signal(static_cast<int>(index));
            return;
        }
    } else {
        if (playerPos.x == wpt.position.x && playerPos.y == wpt.position.y && playerPos.z == wpt.position.z) {
            index = (index + 1) % waypoints.size();
            emit indexUpdate_signal(static_cast<int>(index));
            return;
        }
    }
    if (wpt.direction != "C" && wpt.option == "Stand") {
        auto direction = getDirection(wpt.direction);
        proto->walk(direction);
        msleep(500);
        return;
    }
    proto->autoWalk(localPlayer, wpt.position, true);
}

void FollowWaypoints_Thread::performAction(Waypoint wpt) {
    auto* actionEngine = new LuaEngine(wpt.action, nullptr);
    actionEngine->start();
    while (actionEngine->isRunning()) {
        if (isInterruptionRequested()) {
            actionEngine->requestStop();
            actionEngine->wait(1000);
            delete actionEngine;
            return;
        }
        msleep(100);
    }
    std::string returnedLabel = actionEngine->getReturnedString();
    delete actionEngine;
    if (!returnedLabel.empty()) {
        for (size_t i = 0; i < waypoints.size(); ++i) {
            if (waypoints[i].action == returnedLabel) {
                index = i;
                return;
            }
        }
    }
    index = (index + 1) % waypoints.size();
    emit indexUpdate_signal(static_cast<int>(index));
}

void FollowWaypoints_Thread::performUse(Waypoint wpt, uintptr_t localPlayer, Position playerPos) {
    if ((playerPos.x != wpt.position.x || playerPos.y != wpt.position.y) && playerPos.z == wpt.position.z) {
        proto->autoWalk(localPlayer, wpt.position, false);
        return;
    }
    int itemId = std::stoi(wpt.action);
    auto wptPos = wpt.position;
    auto direction = getDirection(wpt.direction);
    if (direction == Otc::North) wptPos.y -=1;
    if (direction == Otc::East) wptPos.x +=1;
    if (direction == Otc::South) wptPos.y +=1;
    if (direction == Otc::West) wptPos.x -=1;
    if (direction == Otc::NorthEast) wptPos.x +=1, wptPos.y -=1;
    if (direction == Otc::SouthEast) wptPos.x +=1, wptPos.y +=1;
    if (direction == Otc::SouthWest) wptPos.x -=1, wptPos.y +=1;
    if (direction == Otc::NorthWest) wptPos.x -=1, wptPos.y -=1;
    if (itemId == 0) {
        auto tile = proto->getTile(wptPos);
        auto topThing = proto->getTopUseThing(tile);
        proto->use(topThing);
        msleep(500);
        index = (index + 1) % waypoints.size();
        emit indexUpdate_signal(static_cast<int>(index));
    } else {
        auto containers = proto->getContainers();
        for (auto container : containers) {
            auto items = proto->getItems(container);
            for (auto item : items) {
                if (proto->getItemId(item) == itemId) {
                    auto tile = proto->getTile(wptPos);
                    auto topThing = proto->getTopUseThing(tile);
                    proto->useWith(item, topThing);
                    msleep(500);
                    index = (index + 1) % waypoints.size();
                    emit indexUpdate_signal(static_cast<int>(index));
                    return;
                }
            }
        }
        msleep(100);
    }
}

Otc::Direction FollowWaypoints_Thread::getDirection(const std::string& wpt_direction) {
    if (wpt_direction == "N") return Otc::North;
    if (wpt_direction == "E") return Otc::East;
    if (wpt_direction == "S") return Otc::South;
    if (wpt_direction == "W") return Otc::West;
    if (wpt_direction == "NW") return Otc::NorthWest;
    if (wpt_direction == "NE") return Otc::NorthEast;
    if (wpt_direction == "SW") return Otc::SouthWest;
    if (wpt_direction == "SE") return Otc::SouthEast;
    return Otc::InvalidDirection;
}


void FollowWaypoints_Thread::findClosestWaypoint() {
    int minDistance = std::numeric_limits<int>::max();
    auto localPlayer = proto->getLocalPlayer();
    auto playerPos = proto->getPosition(localPlayer);
    for (int i = 0; i < waypoints.size(); ++i) {
        const auto& wpt = waypoints[i];
        if (playerPos.z != wpt.position.z || wpt.direction != "C") {
            continue;
        }
        int dist = std::max(
            std::abs(static_cast<int>(playerPos.x) - static_cast<int>(wpt.position.x)),
            std::abs(static_cast<int>(playerPos.y) - static_cast<int>(wpt.position.y))
        );
        if (dist < minDistance) {
            minDistance = dist;
            index = i;
        }
    }
}

void FollowWaypoints_Thread::findNextValidWaypoint() {
    auto localPlayer = proto->getLocalPlayer();
    auto playerPos = proto->getPosition(localPlayer);
    for (int i = index; i < waypoints.size(); ++i) {
        const auto& wpt = waypoints[i];
        if ((playerPos.z != wpt.position.z || wpt.direction != "C") && (wpt.option != "Node" || wpt.option != "Stand")) {
            continue;
        }
        int dist = std::max(
            std::abs(static_cast<int>(playerPos.x) - static_cast<int>(wpt.position.x)),
            std::abs(static_cast<int>(playerPos.y) - static_cast<int>(wpt.position.y))
        );
        if (dist < 7) {
            index = i;
            return;
        }
    }

}


