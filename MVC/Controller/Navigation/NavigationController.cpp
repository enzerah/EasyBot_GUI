#include "NavigationController.h"

NavigationController::NavigationController(QObject *parent)
    : QObject(parent) {
    m_model = new NavigationModel(this);
    m_view = new NavigationView();
    m_view->show();


    // Navigation View requests
    connect(m_view, &NavigationView::refreshSevers_signal, this, &NavigationController::refreshServers_View);
    connect(m_view, &NavigationView::connectPort_signal, m_model, &NavigationModel::connectPort);
    connect(m_view, &NavigationView::disconnectPort_signal, m_model, &NavigationModel::disconnectPort);

    // Navigation Model requests
    connect(m_model, &NavigationModel::addItem_signal, m_view, &NavigationView::addItem);

    refreshServers_View();
}

NavigationController::~NavigationController() = default;

void NavigationController::showView() {
    m_view->showNormal();
    m_view->show();
}

void NavigationController::refreshServers_View() {
    m_model->refreshServers();
}



