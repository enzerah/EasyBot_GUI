#include "NavigationModel.h"
#include <QJsonObject>


NavigationModel::NavigationModel(QObject *parent)
    : QObject(parent) {
    connectedPorts.push_back(50051);
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
        bool isConnected = false;
        for (auto connectedPort : connectedPorts) {
            isConnected = (port == connectedPort);
        }

        emit addItem_signal(item, isConnected);
    }
    proto->connect(connectedPorts[0]);
}

void NavigationModel::connectPort(int index) {
    connectedPorts[0] = availablePorts[index];
    proto->connect(availablePorts[index]);
}

void NavigationModel::disconnectPort(int index) {
    proto->disconnect(availablePorts[index]);
}

