#include "AlarmsController.h"
#include <iostream>
#include <QJsonArray>

#include "Alarms/AlarmsModel.h"


AlarmsController::AlarmsController(QObject *parent)
    : QObject(parent) {
    m_model = new AlarmsModel(this);
    m_view = new AlarmsView();
    m_view->show();

    connect(m_view, &AlarmsView::addItem_signal, m_model, &AlarmsModel::addItem);

}

AlarmsController::~AlarmsController() {
}


void AlarmsController::showView() {
    m_view->showNormal();
    m_view->show();
}

void AlarmsController::startAlarms_slot(bool state) {
    m_model->startAlarms(state);
}

