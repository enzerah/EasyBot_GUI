#include "AgentAIController.h"

#include <iostream>
#include <QJsonArray>


AgentAIController::AgentAIController(QObject *parent)
    : QObject(parent) {
    m_model = new AgentAIModel(this);
    m_view = new AgentAIView();
    m_view->show();

}

AgentAIController::~AgentAIController() {
}

void AgentAIController::showView() {
    m_view->showNormal();
    m_view->show();
}

QJsonArray AgentAIController::saveSettings() const {
    if (m_model) return m_model->toJson();
    return QJsonArray();
}

void AgentAIController::loadSettings(const QJsonArray &json) {
    if (m_model) m_model->fromJson(json);
}

