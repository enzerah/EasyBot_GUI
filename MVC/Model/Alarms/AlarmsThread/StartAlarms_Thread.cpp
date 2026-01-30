#include "StartAlarms_Thread.h"
#include "../../BotEngine.h"
#include <fstream>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

void StartAlarms_Thread::run() {
    if (m_alarms.empty()) return;
    bool playedSound = false;
    while (!isInterruptionRequested()) {
        if (g_Engine->alarmsState) {
            auto localPlayer = proto->getLocalPlayer();
            for (auto alarm : m_alarms) {
                playedSound = false;
                if (alarm.option == "Low Health" && localPlayer) {
                    auto hpPc = proto->getHealthPercent(localPlayer);
                    if (hpPc <= alarm.value) {
                        PlaySoundA("Sounds/Low_Health.wav", NULL, SND_FILENAME | SND_SYNC);
                        playedSound = true;
                    }
                }
                if (alarm.option == "Low Mana" && localPlayer) {
                    auto mana = proto->getMana(localPlayer);
                    auto maxMana = proto->getMaxMana(localPlayer);
                    if (maxMana > 0) {
                        auto mpPc = (mana * 100) / maxMana;
                        if (mpPc <= alarm.value) {
                            PlaySoundA("Sounds/Low_Mana.wav", NULL, SND_FILENAME | SND_SYNC);
                            playedSound = true;
                        }
                    }
                }
                if (alarm.option == "Disconnect") {
                    if (localPlayer == 0) {
                        PlaySoundA("Sounds/ding.wav", NULL, SND_FILENAME | SND_SYNC);
                        playedSound = true;
                    }
                }
                if (alarm.option == "Creature Detected" && localPlayer) {
                    auto playerPos = proto->getPosition(localPlayer);
                    auto spectators = proto->getSpectators(playerPos);
                    
                    for (auto spectator : spectators) {
                        if (proto->isLocalPlayer(spectator)) continue;
                        auto spectatorName = proto->getCreatureName(spectator);
                        bool nameInList = false;
                        for (const auto& name : alarm.names) {
                            if (name == spectatorName) {
                                nameInList = true;
                                break;
                            }
                        }

                        if (alarm.value == 0) { // Black list: Alarm if NOT in list
                            if (!nameInList) {
                                PlaySoundA("Sounds/Creature_Detected.wav", NULL, SND_FILENAME | SND_SYNC);
                                playedSound = true;
                                break;
                            }
                        } else { // White list: Alarm if IN list
                            if (nameInList) {
                                PlaySoundA("Sounds/Creature_Detected.wav", NULL, SND_FILENAME | SND_SYNC);
                                playedSound = true;
                                break;
                            }
                        }
                    }
                }
            }
            if (playedSound) msleep(1000);
        }
        msleep(100);
    }
}
