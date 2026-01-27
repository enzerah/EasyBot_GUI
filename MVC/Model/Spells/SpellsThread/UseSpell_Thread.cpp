#include "UseSpell_Thread.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

void UseSpell_Thread::updateData(std::vector<Spell> spells) {
    QMutexLocker locker(&m_mutex);
    m_spells = spells;
    std::sort(m_spells.begin(), m_spells.end(), [](const Spell& a, const Spell& b) {
        return a.priority > b.priority;
    });
}

void UseSpell_Thread::run() {
    m_mutex.lock();
    // Sort spells by priority (highest first)
    std::sort(m_spells.begin(), m_spells.end(), [](const Spell& a, const Spell& b) {
        return a.priority > b.priority;
    });
    m_mutex.unlock();

    while (!isInterruptionRequested()) {
        uintptr_t localPlayer = proto->getLocalPlayer();
        int playerHpPercent = proto->getHealthPercent(localPlayer);
        double playerMana = proto->getMana(localPlayer);
        Position playerPos = proto->getPosition(localPlayer);

        m_mutex.lock();
        if (m_spells.empty()) {
            m_mutex.unlock();
            msleep(10);
            continue;
        }
        size_t nSpells = m_spells.size();
        std::vector<int> countsToFind(nSpells);
        for (size_t i = 0; i < nSpells; ++i) {
            countsToFind[i] = m_spells[i].count;
        }
        auto spellsCopy = m_spells;
        m_mutex.unlock();

        auto spectators = proto->getSpectators(playerPos, false);
        for (uintptr_t creature : spectators) {
            if (creature == localPlayer) continue;
            if (proto->isLocalPlayer(creature)) continue;
            Position creaturePos = proto->getPosition(creature);
            int dx = std::abs(static_cast<int>(playerPos.x) - static_cast<int>(creaturePos.x));
            int dy = std::abs(static_cast<int>(playerPos.y) - static_cast<int>(creaturePos.y));
            int distance = std::max(dx, dy);
            auto creatureName = proto->getCreatureName(creature);
            std::transform(creatureName.begin(), creatureName.end(), creatureName.begin(), ::tolower);
            auto isPlayer = proto->isPlayer(creature);
            for (size_t i = 0; i < nSpells; ++i) {
                const auto& spell = spellsCopy[i];
                if (distance > spell.dist) continue;
                if (isPlayer && spell.playerProtection) {
                    if (distance <= spell.dist + 1) countsToFind[i] = 999; // Prevent to use spell when there is a player in distance
                }
                if (spell.targetName != "*" && creatureName != spell.targetName) continue;

                countsToFind[i]--;
            }
        }
        auto isAttacking = proto->isAttacking();
        for (size_t i = 0; i < nSpells; ++i) {
            const auto& spell = spellsCopy[i];
            if (countsToFind[i] <= 0) {
                if (playerHpPercent < spell.minHp) continue;
                if (playerMana < spell.costMp) continue;
                if (spell.requiresTarget) {
                    if (!isAttacking) continue;
                }
                if (spell.option == 0) { // Say
                    proto->talk(spell.spellName);
                    msleep(300);
                } else if (spell.option == 1) { // Rune
                    auto runeId = std::stoi(spell.spellName);
                    if (isAttacking) {
                        auto currentTarget = proto->getAttackingCreature();
                        proto->useInventoryItemWith(runeId, currentTarget);
                    } else {
                        proto->useInventoryItemWith(runeId, localPlayer);
                    }
                    msleep(300);
                }
                break; // Process one spell per tick
            }
        }
        msleep(10);
    }
}
