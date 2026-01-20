#include "LootingController.h"
#include <QJsonArray>



LootingController::LootingController(QObject *parent)
    : QObject(parent) {
    m_model = new LootingModel(this);
    m_view = new LootingView();

    // Looting View requests
    connect(m_view, &LootingView::addItem_signal, m_model, &LootingModel::addItem);
    connect(m_view, &LootingView::deleteItem_signal, m_model, &LootingModel::deleteItem);
    connect(m_view, &LootingView::clearTableWidget_signal, m_model, &LootingModel::clearTableWidget);

    // Looting Model requests
    connect(m_model, &LootingModel::addItem_signal, m_view, &LootingView::addItem);
    connect(m_model, &LootingModel::clearTableWidget_signal, m_view, &LootingView::clearTableWidget);


    m_view->show();
}

LootingController::~LootingController() {
}

void LootingController::showView() {
    m_view->showNormal();
    m_view->show();
}

void LootingController::startLooting_slot(bool state) {
    m_model->startLooting(state);
}

QJsonArray LootingController::saveSettings() const {
    if (m_model) return m_model->toJson();
    return QJsonArray();
}

void LootingController::loadSettings(const QJsonArray &json) {
    if (m_model) m_model->fromJson(json);
}


