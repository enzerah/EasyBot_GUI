#include "HealingController.h"
#include <QJsonArray>


HealingController::HealingController(QObject *parent)
    : QObject(parent) {
    m_model = new HealingModel(this);
    m_view = new HealingView();
    m_view->show();

    // Healing View requests
    connect(m_view, &HealingView::addItem_signal, m_model, &HealingModel::addItem);
    connect(m_view, &HealingView::deleteItem_signal, m_model, &HealingModel::deleteItem);
    connect(m_view, &HealingView::clearTableWidget_signal, m_model, &HealingModel::clearTableWidget);

    // Healing Model requests
    connect(m_model, &HealingModel::addItem_signal, m_view, &HealingView::addItem);
    connect(m_model, &HealingModel::clearTableWidget_signal, m_view, &HealingView::clearTableWidget);


}

HealingController::~HealingController() {
}

void HealingController::showView() {
    m_view->showNormal();
    m_view->show();
}

void HealingController::startHealing_slot(bool state) {
    m_model->startHealing(state);
}

QJsonArray HealingController::saveSettings() const {
    if (m_model) return m_model->toJson();
    return QJsonArray();
}

void HealingController::loadSettings(const QJsonArray &json) {
    if (m_model) m_model->fromJson(json);
}

