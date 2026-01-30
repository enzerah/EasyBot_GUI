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
        if (!startAlarmsThread) {
            startAlarmsThread = new StartAlarms_Thread(alarms, this);
            connect(startAlarmsThread, &QThread::finished, startAlarmsThread, &QObject::deleteLater);
            connect(startAlarmsThread, &QThread::finished, this, [this]() {
                this->startAlarmsThread = nullptr;
            });
            startAlarmsThread->start();
        }
    } else {
        if (startAlarmsThread) {
            startAlarmsThread->requestInterruption();
            startAlarmsThread->quit();
        }
    }
}

void AlarmsModel::addItem(const QString &option, const int &value, const std::vector<QString> list) {
    std::vector<std::string> items;
    for (auto nickname : list) {
        items.push_back(nickname.toStdString());
    }
    Alarm alarm;
    alarm.names = items;
    alarm.value = value;
    alarm.option = option.toStdString();
    alarms.push_back(alarm);
}



