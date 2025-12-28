#include "ScriptsModel.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>



ScriptsModel::ScriptsModel(QObject *parent)
    : QObject(parent) {
}

ScriptsModel::~ScriptsModel() {
}


QJsonArray ScriptsModel::toJson() const {
    QJsonArray jsonArray;
    return jsonArray;
}

void ScriptsModel::fromJson(const QJsonArray &json) {
    for (const auto &val : json) {
        QJsonObject obj = val.toObject();
        QString targets = obj["targets"].toString();
        int dist = obj["dist"].toInt();
        int count = obj["count"].toInt();
        QString option = obj["option"].toString();
        QString name = obj["name"].toString();
        int type = obj["type"].toInt();
        int from = obj["from"].toInt();
        int to = obj["to"].toInt();
        int minHp = obj["minHp"].toInt();
        int minMp = obj["minMp"].toInt();
    }
}

