#include "TargetingController.h"
#include <QJsonArray>



TargetingController::TargetingController(QObject* parent)
    : QObject(parent) {
    m_model = new TargetingModel(this);
    m_view = new TargetingView();

    // Targeting View requests
    connect(m_view, &TargetingView::addItem_signal, this, &TargetingController::addItem_View);
    connect(m_view, &TargetingView::shootableState_signal, this, &TargetingController::shootableState_View);
    connect(m_view, &TargetingView::reachableState_signal, this, &TargetingController::reachableState_View);
    connect(m_view, &TargetingView::openCorpseState_signal, this, &TargetingController::openCorpseState_View);
    connect(m_view, &TargetingView::stayAwayDist_signal, this, &TargetingController::stayAwayDist_View);
    connect(m_view, &TargetingView::deleteItem_signal, this, &TargetingController::deleteItem_View);
    connect(m_view, &TargetingView::clearTableWidget_signal, this, &TargetingController::clearListWidget_View);

    // Connect blocked tiles signals
    connect(m_view, &TargetingView::addBlockedTile_signal, this, &TargetingController::addBlockedTile_View);
    connect(m_view, &TargetingView::deleteBlockedTile_signal, this, &TargetingController::deleteBlockedTile_View);

    // Targeting Model requests
    connect(m_model, &TargetingModel::addItem_signal, this, &TargetingController::addItem_Model);
    connect(m_model, &TargetingModel::clearListWidget_signal, this, &TargetingController::clearListWidget_Model);
    
    // Connect blocked tiles model signals
    connect(m_model, &TargetingModel::addBlockedTile_signal, this, &TargetingController::addBlockedTile_Model);
    connect(m_model, &TargetingModel::clearBlockedTilesListWidget_signal, this, &TargetingController::clearBlockedTilesListWidget_Model);


    m_view->show();
}

TargetingController::~TargetingController() {
}

void TargetingController::showView()
{
    m_view->showNormal();
    m_view->show();
}

void TargetingController::addItem_View(const QString &targetName, const int &dist, const int &count,
    const QString &desiredStance, const QString &monstersAttacks) {
    m_model->addItem(targetName, dist, count, desiredStance, monstersAttacks);
}

void TargetingController::shootableState_View(bool state) {
    m_model->shootableState(state);
}

void TargetingController::reachableState_View(bool state) {
    m_model->reachableState(state);
}

void TargetingController::openCorpseState_View(bool state) {
    m_model->openCorpseState(state);
}

void TargetingController::stayAwayDist_View(int value) {
    m_model->stayAwayDist(value +1);
}

void TargetingController::deleteItem_View(const int& index) {
    m_model->deleteItem(index);
}

void TargetingController::addItem_Model(const QString &targetName, const QString &dist, const QString &count,
    const QString &desiredStance, const QString &monstersAttacks) {
    m_view->addItem(targetName, dist, count, desiredStance, monstersAttacks);
}

void TargetingController::clearListWidget_Model() {
    m_view->clearTableWidget();
}

void TargetingController::startTargeting_slot(bool state) {
    m_model->startTargeting(state);
}

QJsonArray TargetingController::saveSettings() const {
    if (m_model) return m_model->toJson();
    return QJsonArray();
}

void TargetingController::loadSettings(const QJsonArray &json) {
    if (m_model) m_model->fromJson(json);
}

void TargetingController::addBlockedTile_View(const int &x, const int &y, const int &z) {
    m_model->addBlockedTile(x, y, z);
}

void TargetingController::deleteBlockedTile_View(const int &index) {
    m_model->deleteBlockedTile(index);
}

void TargetingController::clearListWidget_View() {
    m_model->clearListWidget();
}

void TargetingController::addBlockedTile_Model(const QString &tile) {
    m_view->addBlockedTile(tile);
}

void TargetingController::clearBlockedTilesListWidget_Model() {
    m_view->clearBlockedTilesListWidget();
}


