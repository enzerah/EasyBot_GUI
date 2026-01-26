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
    while (!isInterruptionRequested()) {
        {
            QMutexLocker locker(&mutex);
            if (!m_enabled) break;
        }

        bool triggerAlarm = false;
        
        uintptr_t localPlayer = proto->getLocalPlayer();
        if (localPlayer != 0) {
            AlarmSettings currentSettings;
            {
                QMutexLocker locker(&mutex);
                currentSettings = settings;
            }

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

            // Player/Creature Detected
            if (!triggerAlarm && (currentSettings.playerDetectedEnabled || currentSettings.creatureDetectedEnabled)) {
                Position pos = proto->getPosition(localPlayer);
                auto spectators = proto->getSpectators(pos);
                for (auto spectator : spectators) {
                    if (spectator == localPlayer) continue;

                    if (currentSettings.playerDetectedEnabled && proto->isPlayer(spectator)) {
                        std::string name = proto->getCreatureName(spectator);
                        bool inList = currentSettings.playerList.count(name) > 0;
                        if (currentSettings.playerBlackListMode) {
                            if (inList) { triggerAlarm = true; break; }
                        } else {
                            if (!inList) { triggerAlarm = true; break; }
                        }
                    }

                    if (currentSettings.creatureDetectedEnabled && proto->isMonster(spectator)) {
                        std::string name = proto->getCreatureName(spectator);
                        bool inList = currentSettings.creatureList.count(name) > 0;
                        if (currentSettings.creatureBlackListMode) {
                            if (inList) { triggerAlarm = true; break; }
                        } else {
                            if (!inList) { triggerAlarm = true; break; }
                        }
                    }
                }
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
