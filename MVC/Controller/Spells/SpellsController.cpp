#include "SpellsController.h"




SpellsController::SpellsController(QObject *parent)
    : QObject(parent) {
    m_model = new SpellsModel(this);
    m_view = new SpellsView();
    m_view->show();

    // Targeting View requests
    connect(m_view, &SpellsView::addItem_signal, m_model, &SpellsModel::addItem);
    connect(m_view, &SpellsView::deleteItem_signal, m_model, &SpellsModel::deleteItem);
    connect(m_view, &SpellsView::clearTableWidget_signal, m_model, &SpellsModel::clearTableWidget);

    // Targeting Model requests
    connect(m_model, &SpellsModel::addItem_signal, m_view, &SpellsView::addItem);
    connect(m_model, &SpellsModel::clearTableWidget_signal, m_view, &SpellsView::clearTableWidget);
}

SpellsController::~SpellsController() {
}

void SpellsController::showView() {
    m_view->showNormal();
    m_view->show();
}

QJsonArray SpellsController::saveSettings() const {
    if (m_model) return m_model->toJson();
    return QJsonArray();
}

void SpellsController::loadSettings(const QJsonArray& json) {
    if (m_model) m_model->fromJson(json);
}

void SpellsController::startSpells_slot(bool state) {
    m_model->startSpells(state);
}

