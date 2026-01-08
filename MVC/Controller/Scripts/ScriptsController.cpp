#include "ScriptsController.h"

ScriptsController::ScriptsController(QObject *parent)
    : QObject(parent) {
    m_model = new ScriptsModel(this);
    m_view = new ScriptsView();
    m_view->show();

    // Scripts View requests
    connect(m_view, &ScriptsView::addItem_signal, this, &ScriptsController::addItem_View);
    connect(m_view, &ScriptsView::deleteItem_signal, this, &ScriptsController::deleteItem_View);

    // Scripts Model requests
    connect(m_model, &ScriptsModel::clearTableWidget_signal, this, &ScriptsController::clearTableWidget_Model);
    connect(m_model, &ScriptsModel::addItem_signal, this, &ScriptsController::addItem_Model);
}

ScriptsController::~ScriptsController() {
}

void ScriptsController::showView() {
    m_view->showNormal();
    m_view->show();
}

void ScriptsController::addItem_View(const int &id, bool state, const QString &name, const QString &script_text) {
    m_model->addItem(id, state, name, script_text);
}

void ScriptsController::deleteItem_View(const int& index) {
    m_model->deleteItem(index);
}

void ScriptsController::startScripts_slot(bool state) {
    m_model->startScripts(state);
}

QJsonArray ScriptsController::saveSettings() const {
    if (m_model) return m_model->toJson();
    return QJsonArray();
}

void ScriptsController::loadSettings(const QJsonArray& json) {
    if (m_model) m_model->fromJson(json);
}

void ScriptsController::addItem_Model(bool state, const QString &name, const QString &script_text) {
    m_view->addItem(state, name, script_text);
}

void ScriptsController::clearTableWidget_Model() {
    m_view->clearTableWidget();
}
