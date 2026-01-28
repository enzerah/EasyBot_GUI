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

        if (playerPos.z != wpt.position.z) findNextValidWaypoint();

        if (wpt.option != "Lure" && engine->hasTarget) {
            proto->stopAutoWalk(localPlayer);
            msleep(100);
            proto->stop();
            msleep(100);
        }

        // If we are stuck on same waypoint for more than 10 seconds
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
        if (dist <= 1) {
            index = (index + 1) % waypoints.size();
            emit indexUpdate_signal(static_cast<int>(index));
            return;
        }
    } else {
        if (playerPos.x == wpt.position.x && playerPos.y == wpt.position.y && playerPos.z == wpt.position.z) {
            if (wpt.direction != "C") {
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
                proto->autoWalk(localPlayer, wptPos);
                msleep(500);
            }
            index = (index + 1) % waypoints.size();
            emit indexUpdate_signal(static_cast<int>(index));
            return;
        }
    }
    proto->autoWalk(localPlayer, wpt.position, false);
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
    if (wpt.action.empty()) {
        auto tile = proto->getTile(wptPos);
        auto topThing = proto->getTopUseThing(tile);
        proto->use(topThing);
        msleep(1000);
        index = (index + 1) % waypoints.size();
        emit indexUpdate_signal(static_cast<int>(index));
    } else {
        int itemId = std::stoi(wpt.action);
        auto item = proto->findItemInContainers(itemId, -1, 0);
        if (item) {
            auto tile = proto->getTile(wptPos);
            auto topThing = proto->getTopUseThing(tile);
            proto->useWith(item, topThing);
            msleep(1000);
            index = (index + 1) % waypoints.size();
            emit indexUpdate_signal(static_cast<int>(index));
        }
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
        if (playerPos.z != wpt.position.z && (wpt.option != "Node" && wpt.option != "Stand")) continue;
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
        if (playerPos.z != wpt.position.z && (wpt.option != "Node" && wpt.option != "Stand")) continue;
        int dist = std::max(
            std::abs(static_cast<int>(playerPos.x) - static_cast<int>(wpt.position.x)),
            std::abs(static_cast<int>(playerPos.y) - static_cast<int>(wpt.position.y))
        );
        if (dist < 7) {
            index = i;
            return;
        }
    }
    // If not found any ValidWpt finds closest
    findClosestWaypoint();
}


