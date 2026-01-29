#include "AttackTargets_Thread.h"
#include "../../BotEngine.h"
#include <algorithm>
#include <QElapsedTimer>


void AttackTargets_Thread::run() {
    g_Engine->hasTarget = false;
    currentTarget = {};
    while (!isInterruptionRequested()) {
        msleep(50);

        auto localPlayer = proto->getLocalPlayer();
        auto playerPos = proto->getPosition(localPlayer);

        // If player is not attacking
        if (!proto->isAttacking() || g_Engine->hasTarget == false) {
            currentTarget = {};
            auto spectators = proto->getSpectators(playerPos, false);

            m_mutex.lock();
            if (m_targets.empty()) {
                m_mutex.unlock();
                msleep(50);
                continue;
            }
            std::map<std::string, int> countsToFind;
            for (const auto& target : m_targets) {
                countsToFind[target.name] = target.count;
            }
            auto targetsCopy = m_targets;
            m_mutex.unlock();
            
            // Iterate over all spectators
            std::vector<MonsterCandidate> monsters;
            for (auto spectator : spectators) {
                if (!proto->isMonster(spectator)) continue;
                auto specName = proto->getCreatureName(spectator); // Grabs name of the monster
                std::transform(specName.begin(), specName.end(), specName.begin(), ::tolower);
                for (const auto& target : targetsCopy) {
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
                g_Engine->hasTarget = false;
                msleep(50);
                continue;
            }
            // Sort monsters by dist to Local Player
            std::sort(monsters.begin(), monsters.end(), [](const MonsterCandidate& a, const MonsterCandidate& b) {
                return a.dist < b.dist;
            });
            // Rest of checks
            for (const auto& monster : monsters) {

                bool reachable = true;
                bool shootable = true;
                // If monster stays above us we consider it as reachable and shootable
                if (monster.dist < 2) {
                    if (countsToFind[monster.target.name] <= 0) {
                        g_Engine->hasTarget = true;
                    } else {
                        g_Engine->hasTarget = false;
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
                    } else {
                        g_Engine->hasTarget = false;
                    }
                    if (proto->getAttackingCreature() != monster.id) proto->attack(monster.id, false);
                    msleep(100);
                    currentTarget = monster;
                    break;
                }
            }
        } else if (g_Engine->hasTarget) {
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
            desiredStance(localPlayer, playerPos, currentTarget.truePos);
        }
    }
}

bool AttackTargets_Thread::isReachable(Position playerPos, Position spectatorPos) {
    auto path = proto->findPath(playerPos, spectatorPos, 15, Otc::PathFindIgnoreCreatures | Otc::PathFindAllowNonPathable);
    if (path.empty()) return false;
    return true;
}

bool AttackTargets_Thread::isShootable(uintptr_t spectator, int dist) {
    return proto->canShoot(spectator, dist);
}

void AttackTargets_Thread::desiredStance(uintptr_t localPlayer, Position playerPos, Position spectatorPos) {
    if (currentTarget.target.desiredStance == "Chase") {
        proto->setChaseMode(Otc::ChaseOpponent);
    }
}


void AttackTargets_Thread::shootableStateChange(bool state) { m_shootableState = state; }
void AttackTargets_Thread::reachableStateChange(bool state) { m_reachableState = state; }
void AttackTargets_Thread::stayAwayDistChange(int currentDist){ m_stayAwayDistance = currentDist; }

void AttackTargets_Thread::updateData(std::vector<Target> targets) {
    QMutexLocker locker(&m_mutex);
    m_targets = targets;
}
