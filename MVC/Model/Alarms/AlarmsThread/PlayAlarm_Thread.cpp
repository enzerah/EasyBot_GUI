#include "PlayAlarm_Thread.h"
#include <windows.h>
#include <mmsystem.h>
#include <iostream>

#pragma comment(lib, "winmm.lib")

PlayAlarm_Thread::PlayAlarm_Thread(QObject *parent)
    : QThread(parent) {}

PlayAlarm_Thread::~PlayAlarm_Thread() {
    requestInterruption();
    wait();
}

void PlayAlarm_Thread::updateSettings(const AlarmSettings& newSettings) {
    QMutexLocker locker(&mutex);
    settings = newSettings;
}

void PlayAlarm_Thread::setEnabled(bool enabled) {
    QMutexLocker locker(&mutex);
    if (m_enabled == enabled) return;
    
    m_enabled = enabled;
    if (enabled) {
        if (!isRunning()) {
            start();
        }
    } else {
        requestInterruption();
    }
}

void PlayAlarm_Thread::playAlarmSound() {
    PlaySoundW(L"Sounds/ding.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void PlayAlarm_Thread::run() {
    proto->clearMessages(); // Initial clear
    
    while (!isInterruptionRequested()) {
        {
            QMutexLocker locker(&mutex);
            if (!m_enabled) break;
        }

        bool triggerAlarm = false;
        AlarmSettings currentSettings;
        {
            QMutexLocker locker(&mutex);
            currentSettings = settings;
        }

        // Disconnect Alarm
        if (currentSettings.disconnectEnabled) {
            if (!proto->getLocalPlayer()) {
                triggerAlarm = true;
            }
        }

        if (!triggerAlarm) {
            uintptr_t localPlayer = proto->getLocalPlayer();
            if (localPlayer != 0) {
                // Low Health
                if (currentSettings.lowHealthEnabled) {
                    double hp = proto->getHealth(localPlayer);
                    double maxHp = proto->getMaxHealth(localPlayer);
                    if (maxHp > 0 && (hp / maxHp) * 100 <= currentSettings.lowHealthValue) {
                        triggerAlarm = true;
                    }
                }

                // Low Mana
                if (!triggerAlarm && currentSettings.lowManaEnabled) {
                    double mp = proto->getMana(localPlayer);
                    double maxMp = proto->getMaxMana(localPlayer);
                    if (maxMp > 0 && (mp / maxMp) * 100 <= currentSettings.lowManaValue) {
                        triggerAlarm = true;
                    }
                }

                // Creature Detected
                if (!triggerAlarm && currentSettings.creatureDetectedEnabled) {
                    Position pos = proto->getPosition(localPlayer);
                    auto spectators = proto->getSpectators(pos);
                    std::string myName = proto->getCharacterName();

                    for (auto spectator : spectators) {
                        if (spectator == localPlayer) continue;
                        if (!proto->isMonster(spectator) && !proto->isPlayer(spectator)) continue;

                        std::string name = proto->getCreatureName(spectator);
                        bool inList = currentSettings.creatureList.count(name) > 0;

                        if (currentSettings.creatureBlackListMode) {
                            // Reaguje na wszystkie nicki prócz danego wpisanego
                            if (!inList) {
                                triggerAlarm = true;
                                break;
                            }
                        } else {
                            // Reaguje tylko na nazwy wpisane
                            if (inList) {
                                triggerAlarm = true;
                                break;
                            }
                        }
                    }
                }
            }
        }

        // Message Alarms
        if (!triggerAlarm && (currentSettings.defaultMessageEnabled || currentSettings.privateMessageEnabled)) {
            auto messages = proto->getMessages(10);
            std::string myName = proto->getCharacterName();

            for (const auto& msg : messages) {
                bool isPrivate = (msg.mode == Otc::MessagePrivateFrom || msg.mode == Otc::MessageGamemasterPrivateFrom);
                bool isDefault = (msg.mode == Otc::MessageSay || msg.mode == Otc::MessageYell || msg.mode == Otc::MessageWhisper);

                if (currentSettings.privateMessageEnabled && isPrivate) {
                    triggerAlarm = true;
                    break;
                }

                if (currentSettings.defaultMessageEnabled && isDefault) {
                    if (msg.name != myName) {
                        triggerAlarm = true;
                        break;
                    }
                }
            }
            if (!messages.empty()) {
                proto->clearMessages();
            }
        }

        if (triggerAlarm) {
            playAlarmSound();
            msleep(1000); // Wait 1 second before next alarm if triggered
        } else {
            msleep(200); // Check every 200ms
        }
    }
}
