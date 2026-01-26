#include "AlarmsModel.h"
#include <iostream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


AlarmsModel::AlarmsModel(QObject *parent)
    : QObject(parent) {
    m_playAlarm_Thread = new PlayAlarm_Thread(this);
}

AlarmsModel::~AlarmsModel() {
    delete m_playAlarm_Thread;
}

void AlarmsModel::updateAlarmSettings(const PlayAlarm_Thread::AlarmSettings& settings) {
    m_playAlarm_Thread->updateSettings(settings);
}

void AlarmsModel::setAlarmEnabled(bool enabled) {
    m_playAlarm_Thread->setEnabled(enabled);
}




