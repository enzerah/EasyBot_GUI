#include "AgentAIModel.h"
#include <iostream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


AgentAIModel::AgentAIModel(QObject *parent)
    : QObject(parent) {

}

AgentAIModel::~AgentAIModel() {
}
QJsonArray AgentAIModel::toJson() const {
    QJsonArray jsonArray;
    return jsonArray;
}

void AgentAIModel::fromJson(const QJsonArray &json) {
}

