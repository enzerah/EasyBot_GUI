#ifndef ALARMSMODEL_H
#define ALARMSMODEL_H
#include <QObject>
#include <QVariant>
#include "../../const.h"


#include "AlarmsThread/PlayAlarm_Thread.h"


class AlarmsModel : public QObject {
    Q_OBJECT
public:
    explicit AlarmsModel(QObject *parent = nullptr);
    ~AlarmsModel();

    void updateAlarmSettings(const PlayAlarm_Thread::AlarmSettings& settings);
    void setAlarmEnabled(bool enabled);

private:
    PlayAlarm_Thread* m_playAlarm_Thread;
};




#endif // ALARMSMODEL_H
