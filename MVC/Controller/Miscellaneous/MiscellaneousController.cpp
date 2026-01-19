#include "MiscellaneousController.h"
#include <iostream>
#include <QJsonArray>


MiscellaneousController::MiscellaneousController(QObject *parent)
    : QObject(parent) {
    m_model = new MiscellaneousModel(this);
    m_view = new MiscellaneousView();
    m_view->show();
    // Miscellaneous View request
    connect(m_view, &MiscellaneousView::startCondition_signal, m_model, &MiscellaneousModel::startConditions);
    connect(m_view, &MiscellaneousView::startOthers_signal, m_model, &MiscellaneousModel::startOthers);

}

MiscellaneousController::~MiscellaneousController() {
}

void MiscellaneousController::showView() {
    m_view->showNormal();
    m_view->show();
}


