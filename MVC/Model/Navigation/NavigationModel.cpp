#include "NavigationModel.h"
#include <QJsonObject>


NavigationModel::NavigationModel(QObject *parent)
    : QObject(parent) {
}

NavigationModel::~NavigationModel() = default;

void NavigationModel::refreshServers() {
    auto clientName = proto->getCharacterName();
    QString item = "";
    if (clientName == "") {
        item = QStringLiteral("Client");
    } else {
        item = QStringLiteral("%1").arg(clientName);
    }
    emit addItem_signal(item, true);
}



