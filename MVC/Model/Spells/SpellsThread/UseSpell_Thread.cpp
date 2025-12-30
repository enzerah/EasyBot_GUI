#include "UseSpell_Thread.h"


void UseSpell_Thread::run() {
    if (m_spells.empty()) return;
    auto clientVersion = proto->getClientVersion();
    if (clientVersion >= 758) hotkeys = true;
    while (!isInterruptionRequested()) {
        auto localPlayer = proto->getLocalPlayer();
        auto current_hp = proto->getHealth(localPlayer);
        auto max_hp = proto->getMaxHealth(localPlayer);
        auto current_mp = proto->getMana(localPlayer);
        double current_hp_pc = (current_hp / max_hp) * 100;
        for (auto spell : m_spells) {
            if (spell.type == 0) areaSpell(spell, localPlayer, current_hp_pc, current_mp);
            if (spell.type == 1) targetSpell(spell, localPlayer, current_hp_pc, current_mp);
            if (spell.type == 2) runeSpell(spell, localPlayer, current_hp_pc, current_mp);
        }
        msleep(100);
    }
}

void UseSpell_Thread::targetSpell(Spell spell, uintptr_t localPlayer, double current_hp_pc, double current_mp) {
    if (!proto->isAttacking()) return;
    int dist = 999;
    auto currentTarget = proto->getAttackingCreature();
    if (spell.minHp > current_hp_pc || spell.minMp > current_mp) return;
    auto currentTarget_Name = proto->getCreatureName(currentTarget);
    std::transform(currentTarget_Name.begin(), currentTarget_Name.end(), currentTarget_Name.begin(), ::tolower);
    if (currentTarget_Name != spell.targets && spell.targets != "*") return;
    auto monsterHp = proto->getHealthPercent(currentTarget);
    std::cout << monsterHp << std::endl;
    if (spell.from < monsterHp && spell.to > monsterHp) return;
    if (spell.dist > 0) {
        auto playerPos = proto->getPosition(localPlayer);
        auto targetPos = proto->getPosition(currentTarget);
        dist = std::max(std::abs(static_cast<int>(playerPos.x) - static_cast<int>(targetPos.x)),
            std::abs(static_cast<int>(playerPos.y) - static_cast<int>(targetPos.y)));
    }
    if (dist < spell.dist) return;
    if (spell.option == "Say") {
        proto->talk(spell.name);
        return;
    }
    if (!hotkeys) {
        int itemId = std::stoi(spell.name);
        auto containers = proto->getContainers();
        for (auto container : containers) {
            auto items = proto->getItems(container);
            for (auto item : items) {
                if (proto->getItemId(item) == itemId) {
                    if (spell.option == "Use on Target") {
                        auto targetPos = proto->getPosition(currentTarget);
                        auto tile = proto->getTile(targetPos);
                        auto topThing = proto->getTopUseThing(tile);
                        proto->useWith(item, topThing);
                        return;
                    } else {
                        proto->useWith(item, localPlayer);
                        return;
                    }
                }
            }
        }
    }
}

void UseSpell_Thread::areaSpell(Spell spell, uintptr_t localPlayer, double current_hp_pc, double current_mp) {
    auto playerPos = proto->getPosition(localPlayer);
    auto spectators = proto->getSpectators(playerPos, false);
    for (auto spectator : spectators) {
        auto currentTarget_Name = proto->getCreatureName(spectator);
        std::transform(currentTarget_Name.begin(), currentTarget_Name.end(), currentTarget_Name.begin(), ::tolower);
        if (currentTarget_Name != spell.targets && spell.targets != "*") continue;
        auto targetPos = proto->getPosition(spectator);
        int dist = std::max(std::abs(static_cast<int>(playerPos.x) - static_cast<int>(targetPos.x)),
            std::abs(static_cast<int>(playerPos.y) - static_cast<int>(targetPos.y)));
        if (dist > spell.dist && spell.dist > 0) continue;
        spell.count -= 1;
    }
    if (spell.count > 0) return;
    if (spell.option == "Say") {
        proto->talk(spell.name);
        return;
    }
    if (!hotkeys) {
        int itemId = std::stoi(spell.name);
        auto containers = proto->getContainers();
        for (auto container : containers) {
            auto items = proto->getItems(container);
            for (auto item : items) {
                if (proto->getItemId(item) == itemId) {
                    if (spell.option == "Use on Target") {
                        if (!proto->isAttacking()) return;
                        auto currentTarget = proto->getAttackingCreature();
                        auto targetPos = proto->getPosition(currentTarget);
                        auto tile = proto->getTile(targetPos);
                        auto topThing = proto->getTopUseThing(tile);
                        proto->useWith(item, topThing);
                        return;
                    } else {
                        proto->useWith(item, localPlayer);
                        return;
                    }
                }
            }
        }
    }
}

void UseSpell_Thread::runeSpell(Spell spell, uintptr_t localPlayer, double current_hp_pc, double current_mp) {
}

