#include "Conditions_Thread.h"

void Conditions_Thread::updateCondition(const std::string& conditionType, const std::string& spellName, int manaCost, bool state) {
    mutex.lock();
    if (state) {
        activeConditions[conditionType] = {spellName, manaCost};
    } else {
        activeConditions.erase(conditionType);
    }
    mutex.unlock();
}

bool Conditions_Thread::hasActiveConditions() {
    mutex.lock();
    bool empty = activeConditions.empty();
    mutex.unlock();
    return !empty;
}

void Conditions_Thread::run()
{
    while (!isInterruptionRequested()) {
        mutex.lock();
        std::map<std::string, ConditionData> conditions = activeConditions;
        mutex.unlock();

        if (conditions.empty()) break;

        auto localPlayer = proto->getLocalPlayer();
        auto playerState = proto->getStates(localPlayer);
        auto currentMp = proto->getMana(localPlayer);

        for (auto it = conditions.begin(); it != conditions.end(); ++it) {
            std::string condition = it->first;
            ConditionData data = it->second;
            bool shouldCast = false;
            
            if (condition == "Haste") {
                if (!(playerState & Otc::PlayerStates::IconHaste)) shouldCast = true;
            } else if (condition == "Buff") {
                if (!(playerState & Otc::PlayerStates::IconPartyBuff)) shouldCast = true;
            } else if (condition == "ManaShield") {
                if (!(playerState & Otc::PlayerStates::IconManaShield)) shouldCast = true;
            } else if (condition == "AntyParal") {
                if (playerState & Otc::PlayerStates::IconParalyze) shouldCast = true;
            }

            if (shouldCast && currentMp >= data.manaCost) {
                proto->talk(data.spellName);
            }
            msleep(100);
        }

        msleep(500);
    }
}