#include "AlarmsController.h"
#include <iostream>
#include <QJsonArray>


AlarmsController::AlarmsController(QObject *parent)
    : QObject(parent) {
    m_model = new AlarmsModel(this);
    m_view = new AlarmsView();

    connect(m_view, &AlarmsView::settingsChanged, this, &AlarmsController::onSettingsChanged);
}

AlarmsController::~AlarmsController() {
    delete m_view;
}

void AlarmsController::startAlarms_slot(bool state) {
    if (state) {
        m_model->updateAlarmSettings(m_view->getSettings());
    }
    m_model->setAlarmEnabled(state);
}

void AlarmsController::onSettingsChanged() {
    m_model->updateAlarmSettings(m_view->getSettings());
}


void AlarmsController::showView() {
    m_view->showNormal();
    m_view->show();
}


