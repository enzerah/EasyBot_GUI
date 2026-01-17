#include "NavigationModel.h"
#include <QJsonObject>


NavigationModel::NavigationModel(QObject *parent)
    : QObject(parent) {
    connectedPort = 50051;
}

NavigationModel::~NavigationModel() = default;

void NavigationModel::refreshServers() {
    availablePorts = proto->availablePorts();
    if (availablePorts.empty()) return;
    for (auto port : availablePorts) {
        proto->connect(port);
        auto clientName = proto->getCharacterName();
        QString item = "";
        if (clientName == "") {
            item = QStringLiteral("Unknown");
        } else {
            item = QStringLiteral("%1").arg(clientName);
        }
        bool isConnected = (port == connectedPort);
        emit addItem_signal(item, isConnected);
    }
    proto->connect(connectedPort);
}

void NavigationModel::connectPort(int index) {
    connectedPort = availablePorts[index];
    proto->connect(availablePorts[index]);
}

void NavigationModel::disconnectPort(int index) {
    proto->disconnect(availablePorts[index]);
}

