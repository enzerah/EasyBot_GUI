#include "ScriptsController.h"

ScriptsController::ScriptsController(QObject *parent)
    : QObject(parent) {
    m_model = new ScriptsModel(this);
    m_view = new ScriptsView();
    m_view->show();
}

ScriptsController::~ScriptsController() {
}

void ScriptsController::showView() {
    m_view->showNormal();
    m_view->show();
}

QJsonArray ScriptsController::saveSettings() const {
    if (m_model) return m_model->toJson();
    return QJsonArray();
}

void ScriptsController::loadSettings(const QJsonArray& json) {
    if (m_model) m_model->fromJson(json);
}

