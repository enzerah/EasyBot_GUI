#ifndef ALARMSMODEL_H
#define ALARMSMODEL_H
#include <QObject>
#include <QVariant>
#include <map>
#include "../../const.h"
#include "AlarmsThread/StartAlarms_Thread.h"


class AlarmsModel : public QObject {
    Q_OBJECT
public:
    explicit AlarmsModel(QObject *parent = nullptr);
    ~AlarmsModel();

    void startAlarms(bool state);

    void addItem(const QString& option, const int &value, const std::vector<QString> list, bool enabled);

private:
    std::map<std::string, Alarm> alarms;
    StartAlarms_Thread *startAlarmsThread = nullptr;
};



#endif
