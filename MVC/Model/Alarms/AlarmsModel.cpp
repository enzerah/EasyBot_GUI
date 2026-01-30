#include "AlarmsModel.h"


AlarmsModel::AlarmsModel(QObject *parent)
    : QObject(parent) {

}

AlarmsModel::~AlarmsModel() {
    if (startAlarmsThread) {
        startAlarmsThread->quit();
        startAlarmsThread->wait();
        delete startAlarmsThread;
    }
}

void AlarmsModel::startAlarms(bool state) {
    if (state) {
        if (startAlarmsThread) {
            startAlarmsThread->requestInterruption();
            startAlarmsThread->quit();
            startAlarmsThread->wait();
        }
        std::vector<Alarm> alarmList;
        for (auto const& [key, val] : alarms) {
            alarmList.push_back(val);
        }
        startAlarmsThread = new StartAlarms_Thread(alarmList, this);
        connect(startAlarmsThread, &QThread::finished, startAlarmsThread, &QObject::deleteLater);
        connect(startAlarmsThread, &QThread::finished, this, [this]() {
            this->startAlarmsThread = nullptr;
        });
        startAlarmsThread->start();
    } else {
        if (startAlarmsThread) {
            startAlarmsThread->requestInterruption();
            startAlarmsThread->quit();
        }
    }
}

void AlarmsModel::addItem(const QString &option, const int &value, const std::vector<QString> list, bool enabled) {
    std::string optName = option.toStdString();
    if (!enabled) {
        alarms.erase(optName);
    } else {
        std::vector<std::string> items;
        for (const auto& nickname : list) {
            items.push_back(nickname.toStdString());
        }
        Alarm alarm;
        alarm.names = items;
        alarm.value = value;
        alarm.option = optName;
        alarms[optName] = alarm;
    }
}



