#include "AlarmsController.h"
#include <iostream>
#include <QJsonArray>


AlarmsController::AlarmsController(QObject *parent)
    : QObject(parent) {
    m_view = new AlarmsView();
    m_view->show();
}

AlarmsController::~AlarmsController() {
    delete m_view;
}


void AlarmsController::showView() {
    m_view->showNormal();
    m_view->show();
}


