#ifndef PLAYALARM_THREAD_H
#define PLAYALARM_THREAD_H

#include <QThread>
#include <QMutex>
#include <string>
#include <vector>
#include <set>
#include "../../proto_functions_client.h"

class PlayAlarm_Thread : public QThread {
    Q_OBJECT
public:
    struct AlarmSettings {
        bool lowHealthEnabled = false;
        int lowHealthValue = 20;
        bool lowManaEnabled = false;
        int lowManaValue = 20;
        
        bool playerDetectedEnabled = false;
        bool playerBlackListMode = true;
        std::set<std::string> playerList;
        
        bool creatureDetectedEnabled = false;
        bool creatureBlackListMode = true;
        std::set<std::string> creatureList;
    };

    explicit PlayAlarm_Thread(QObject *parent = nullptr);
    ~PlayAlarm_Thread() override;

    void updateSettings(const AlarmSettings& newSettings);
    void setEnabled(bool enabled);

protected:
    void run() override;

private:
    AlarmSettings settings;
    bool m_enabled = false;
    QMutex mutex;

    void playAlarmSound();
};

#endif // PLAYALARM_THREAD_H
