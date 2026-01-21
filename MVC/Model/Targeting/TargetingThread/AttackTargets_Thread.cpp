#include "AttackTargets_Thread.h"
#include "../../BotEngine.h"
#include <algorithm>
#include <QElapsedTimer>


void AttackTargets_Thread::run() {
    if (m_targets.empty()) return;

    std::cout << m_targets[0].count << std::endl;


    engine->hasTarget = false;
    currentTarget = {};


    while (!isInterruptionRequested()) {
        msleep(50);

        auto localPlayer = proto->getLocalPlayer();
        auto playerPos = proto->getPosition(localPlayer);

        // If player is not attacking
        if (!proto->isAttacking() || engine->hasTarget == false) {
            currentTarget = {};
            auto spectators = proto->getSpectators(playerPos, false);

            std::map<std::string, int> countsToFind;
            for (const auto& target : m_targets) {
                countsToFind[target.name] = target.count;
            }
            // Iterate over all spectators
            std::vector<MonsterCandidate> monsters;
            for (auto spectator : spectators) {
                if (!proto->isMonster(spectator)) continue;
                auto specName = proto->getCreatureName(spectator); // Grabs name of the monster
                std::transform(specName.begin(), specName.end(), specName.begin(), ::tolower);
                for (const auto& target : m_targets) {
                    if (specName != target.name && target.name != "*") continue;
                    auto monsterPos = proto->getPosition(spectator);
                    int dist = std::max(std::abs(static_cast<int>(playerPos.x) - static_cast<int>(monsterPos.x)),
                        std::abs(static_cast<int>(playerPos.y) - static_cast<int>(monsterPos.y)));
                    if (target.dist > 0 && dist > target.dist) continue;
                    countsToFind[target.name] -= 1;
                    monsters.push_back({dist, spectator, monsterPos, target});
                    break; // One target can only match one name
                }
            }
            // If there is no Targets
            if (monsters.empty()) {
                engine->hasTarget = false;
                msleep(50);
                continue;
            }
            // Sort monsters by dist to Local Player
            std::sort(monsters.begin(), monsters.end(), [](const MonsterCandidate& a, const MonsterCandidate& b) {
                return a.dist < b.dist;
            });
            proto->setChaseMode(Otc::DontChase);
            // Rest of checks
            for (const auto& monster : monsters) {

                bool reachable = true;
                bool shootable = true;
                // If monster stays above us we consider it as reachable and shootable
                if (monster.dist < 2) {
                    if (countsToFind[monster.target.name] <= 0) {
                        std::cout << "Blocking Walker " << std::endl;
                        std::cout << countsToFind[monster.target.name] << std::endl;
                        engine->hasTarget = true;
                    } else {
                        std::cout << "Unlocking Walker " << std::endl;
                        std::cout << countsToFind[monster.target.name] << std::endl;
                        engine->hasTarget = false;
                    }
                    if (proto->getAttackingCreature() != monster.id) proto->attack(monster.id, false);
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
                    if (countsToFind[monster.target.name] <= 0) {
                        std::cout << "Blocking Walker " << std::endl;
                        std::cout << countsToFind[monster.target.name] << std::endl;
                    } else {
                        std::cout << "Unlocking Walker " << std::endl;
                        std::cout << countsToFind[monster.target.name] << std::endl;
                        engine->hasTarget = false;
                    }
                    if (proto->getAttackingCreature() != monster.id) proto->attack(monster.id, false);
                    msleep(100);
                    currentTarget = monster;
                    break;
                }
            }
        } else if (engine->hasTarget) {
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
                continue;
            }
            // If Target is reachable and shootable
            if (engine->hasTarget) {
                desiredStance(localPlayer, playerPos, currentTarget.truePos);
            } else {
                proto->setChaseMode(Otc::DontChase);
            }
        }
    }
}

bool AttackTargets_Thread::isReachable(Position playerPos, Position spectatorPos) {
    auto path = proto->findPath(playerPos, spectatorPos, 100, Otc::PathFindIgnoreCreatures | Otc::PathFindAllowNonPathable);
    if (path.empty()) return false;
    return true;
}

bool AttackTargets_Thread::isShootable(uintptr_t spectator, int dist) {
    return proto->canShoot(spectator, dist);
}

void AttackTargets_Thread::desiredStance(uintptr_t localPlayer, Position playerPos, Position spectatorPos) {
    if (currentTarget.target.desiredStance == "Chase") {
        for (auto blockedTile : m_blockedTiles) {
            if (blockedTile.x == currentTarget.truePos.x && blockedTile.y == currentTarget.truePos.y && blockedTile.z == currentTarget.truePos.z) return;
        }
        proto->setChaseMode(Otc::ChaseOpponent);
    }
}


void AttackTargets_Thread::shootableStateChange(bool state) { m_shootableState = state; }
void AttackTargets_Thread::reachableStateChange(bool state) { m_reachableState = state; }
void AttackTargets_Thread::openCorpseStateChange(bool state) { m_openCorpseState = state; }
void AttackTargets_Thread::stayAwayDistChange(int currentDist){ m_stayAwayDistance = currentDist; }