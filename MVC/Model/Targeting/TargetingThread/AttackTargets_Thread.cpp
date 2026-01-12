#include "AttackTargets_Thread.h"
#include "../../BotEngine.h"
#include <algorithm>
#include <QElapsedTimer>


void AttackTargets_Thread::run() {
    if (m_targets.empty()) return;

    engine->hasTarget = false;
    currentTarget = {};

    bool lootCorpse = false;

    while (!isInterruptionRequested()) {
        auto localPlayer = proto->getLocalPlayer();
        auto playerPos = proto->getPosition(localPlayer);

        // If player is not attacking
        if (!proto->isAttacking()) {
            if (m_openCorpseState && lootCorpse) {
                lootCorpse = false;
                auto tile = proto->getTile(currentTarget.truePos);
                auto tileItems = proto->getTileItems(tile);
                for (auto tileItem : tileItems) {
                    if (proto->isContainer(tileItem)) {
                        proto->open(tileItem, 0);
                        engine->isLooting = true;
                        // Waits till looting is finished
                        QElapsedTimer lootingTimer;
                        lootingTimer.start();
                        while (engine->isLooting) {
                            // Loot for only 5 sec
                            if (lootingTimer.hasExpired(5000)) break;
                            msleep(50);
                        }
                        break;
                    }
                }
            }
            auto spectators = proto->getSpectators(playerPos, false);
            // Iterate over all spectators
            std::vector<MonsterCandidate> monsters;
            for (auto spectator : spectators) {
                if (!proto->isMonster(spectator)) continue;
                auto specName = proto->getCreatureName(spectator); // Grabs name of the monster
                std::transform(specName.begin(), specName.end(), specName.begin(), ::tolower);
                for (auto target : m_targets) {
                    if (specName != target.name && target.name != "*") continue;
                    auto monsterPos = proto->getPosition(spectator);
                    int dist = std::max(std::abs(static_cast<int>(playerPos.x) - static_cast<int>(monsterPos.x)),
                        std::abs(static_cast<int>(playerPos.y) - static_cast<int>(monsterPos.y)));
                    if (target.dist > 0 && dist > target.dist) continue;
                    monsters.push_back({dist, spectator, monsterPos, target});
                    break; // One target can only match one name
                }
            }
            // If there is no Targets
            if (!monsters.size()) {
                engine->hasTarget = false;
                msleep(50);
                continue;
            }
            // Sort monsters by dist to Local Player
            std::sort(monsters.begin(), monsters.end(), [](const MonsterCandidate& a, const MonsterCandidate& b) {
                return a.dist < b.dist;
            });
            // Rest of checks
            for (auto monster : monsters) {
                bool reachable = true;
                bool shootable = true;
                // If monster stays above us we consider it as reachable and shootable
                if (monster.dist < 2) {
                    engine->hasTarget = true;
                    proto->attack(monster.id, false);
                    msleep(100);
                    currentTarget = monster;
                    break;
                }
                if (m_reachableState) {
                    reachable = isReachable(playerPos, monster.truePos);
                }
                if (m_shootableState) {
                    shootable = isShootable(monster.id, monster.dist);
                }
                if (reachable && shootable) {
                    engine->hasTarget = true;
                    proto->attack(monster.id, false);
                    msleep(100);
                    currentTarget = monster;
                    break;
                }
            }
        } else {
            lootCorpse = true;
            bool reachable = true;
            bool shootable = true;
            if (proto->isDead(currentTarget.id)) {
                continue;
            }
            auto truePos = proto->getPosition(currentTarget.id);
            // It also means target is dead
            if (truePos.x == 0xFFFF) {
                continue;
            }
            currentTarget.truePos = truePos;
            int dist = std::max(std::abs(static_cast<int>(playerPos.x) - static_cast<int>(currentTarget.truePos.x)),
                std::abs(static_cast<int>(playerPos.y) - static_cast<int>(currentTarget.truePos.y)));
            currentTarget.dist = dist;
            if (dist > 1) {
                if (m_reachableState) {
                    reachable = isReachable(playerPos, currentTarget.truePos);
                }
                if (m_shootableState) {
                    shootable = isShootable(currentTarget.id, currentTarget.dist);
                }
            }
            if (!reachable || !shootable) {
                proto->cancelAttack();
                msleep(100);
                proto->cancelAttackAndFollow();
                msleep(100);
                lootCorpse = false;
                continue;
            }
            // If Target is reachable and shootable
            desiredStance(localPlayer, playerPos, currentTarget.truePos);
        }
        msleep(50);
    }
}

bool AttackTargets_Thread::isReachable(Position playerPos, Position spectatorPos) {
    auto path = proto->findPath(playerPos, spectatorPos, 100, Otc::PathFindIgnoreCreatures | Otc::PathFindAllowNonPathable);
    if (path.empty()) return false;
    auto lastDir = path.at(path.size() - 1);
    auto newPos = spectatorPos;
    if (lastDir == Otc::North) newPos.y +=1;
    if (lastDir == Otc::East) newPos.x -=1;
    if (lastDir == Otc::South) newPos.y -=1;
    if (lastDir == Otc::West) newPos.x +=1;
    if (lastDir == Otc::NorthEast) newPos.x -=1, newPos.y +=1;
    if (lastDir == Otc::SouthEast) newPos.x -=1, newPos.y -=1;
    if (lastDir == Otc::SouthWest) newPos.x +=1, newPos.y -=1;
    if (lastDir == Otc::NorthWest) newPos.x +=1, newPos.y +=1;
    auto second_path = proto->findPath(playerPos, newPos, 100, Otc::PathFindAllowNonPathable);
    if (!second_path.empty()) {
        return true;
    }
    // Check all dirs around the target
    for (int allDir =0; allDir < 8; allDir++) {
        if (allDir == lastDir) continue;
        newPos = spectatorPos;
        if (allDir == Otc::North) newPos.y +=1;
        if (allDir == Otc::East) newPos.x -=1;
        if (allDir == Otc::South) newPos.y -=1;
        if (allDir == Otc::West) newPos.x +=1;
        if (allDir == Otc::NorthEast) newPos.x -=1, newPos.y +=1;
        if (allDir == Otc::SouthEast) newPos.x -=1, newPos.y -=1;
        if (allDir == Otc::SouthWest) newPos.x +=1, newPos.y -=1;
        if (allDir == Otc::NorthWest) newPos.x +=1, newPos.y +=1;
        second_path = proto->findPath(playerPos, newPos, 100, Otc::PathFindAllowNonPathable);
        if (!second_path.empty()) {
            return true;
        }
    }
    return false;
}

bool AttackTargets_Thread::isShootable(uintptr_t spectator, int dist) {
    return proto->canShoot(spectator, dist);
}

void AttackTargets_Thread::desiredStance(uintptr_t localPlayer, Position playerPos, Position spectatorPos) {
    if (currentTarget.target.desiredStance == "Chase") {
        for (auto blockedTile : m_blockedTiles) {
            if (blockedTile.x == currentTarget.truePos.x && blockedTile.y == currentTarget.truePos.y && blockedTile.z == currentTarget.truePos.z) return;
        }
        // Check all dirs around the target
        for (int allDir = 0; allDir < 8; allDir++) {
            auto newPos = spectatorPos;
            if (allDir == Otc::North) newPos.y +=1;
            if (allDir == Otc::East) newPos.x -=1;
            if (allDir == Otc::South) newPos.y -=1;
            if (allDir == Otc::West) newPos.x +=1;
            if (allDir == Otc::NorthEast) newPos.x -=1, newPos.y +=1;
            if (allDir == Otc::SouthEast) newPos.x -=1, newPos.y -=1;
            if (allDir == Otc::SouthWest) newPos.x +=1, newPos.y -=1;
            if (allDir == Otc::NorthWest) newPos.x +=1, newPos.y +=1;
            auto second_path = proto->findPath(playerPos, newPos, 100, Otc::PathFindAllowNonPathable);
            if (!second_path.empty()) {
                proto->autoWalk(localPlayer, currentTarget.truePos, false);
                msleep(100);
                return;
            }
        }
    }
}


void AttackTargets_Thread::shootableStateChange(bool state) { m_shootableState = state; }
void AttackTargets_Thread::reachableStateChange(bool state) { m_reachableState = state; }
void AttackTargets_Thread::openCorpseStateChange(bool state) { m_openCorpseState = state; }
void AttackTargets_Thread::stayAwayDistChange(int currentDist){ m_stayAwayDistance = currentDist; }
