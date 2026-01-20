#include "HealingModel.h"
#include <iostream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


HealingModel::HealingModel(QObject *parent)
    : QObject(parent) {

}

HealingModel::~HealingModel() {
    if (startHealingThread) {
        startHealingThread->quit();
        startHealingThread->wait();
        delete startHealingThread;
    }
}

void HealingModel::addItem(const QString &action, const QString &heal, const QString &condition, const int &random,
    const int &below, const int &above, const int &minimum) {
    Heal healing;
    healing.action = action.toStdString();
    healing.heal = heal.toStdString();
    healing.condition = condition.toStdString();
    healing.random = random;
    healing.below = below;
    healing.above = above;
    healing.minimum = minimum;
    heals.push_back(healing);
    emit addItem_signal(action, heal, condition, random, below, above, minimum);
}


void HealingModel::deleteItem(const int& index) {
    heals.erase(heals.begin() + index);
}

void HealingModel::startHealing(bool state) {
    if (state) {
        if (!startHealingThread) {
            startHealingThread = new StartHealing_Thread(heals, this);
            connect(startHealingThread, &QThread::finished, startHealingThread, &QObject::deleteLater);
            connect(startHealingThread, &QThread::finished, this, [this]() {
                this->startHealingThread = nullptr;
            });
            startHealingThread->start();
        }
    } else {
        if (startHealingThread) {
            startHealingThread->requestInterruption();
            startHealingThread->quit();
        }
    }
}

QJsonArray HealingModel::toJson() const {
    QJsonArray jsonArray;
    for (const auto &heal : heals) {
        QJsonObject jsonObj;
        jsonObj["action"] = QString::fromStdString(heal.action);
        jsonObj["heal_text"] = QString::fromStdString(heal.heal);
        jsonObj["condition"] = QString::fromStdString(heal.condition);
        jsonObj["random"] = heal.random;
        jsonObj["below"] = heal.below;
        jsonObj["above"] = heal.above;
        jsonObj["minimum"] = heal.minimum;
        jsonArray.append(jsonObj);
    }
    return jsonArray;
}

void HealingModel::fromJson(const QJsonArray &json) {
    heals.clear();
    emit clearTableWidget_signal();
    for (const auto &val : json) {
        QJsonObject obj = val.toObject();
        QString action = obj["action"].toString();
        QString heal_text = obj["heal_text"].toString();
        QString condition = obj["condition"].toString();
        int random = obj["random"].toInt();
        int below = obj["below"].toInt();
        int above = obj["above"].toInt();
        int minimum = obj["minimum"].toInt();
        addItem(action, heal_text, condition, random, below, above, minimum);
    }
}

void HealingModel::clearTableWidget() {
    heals.clear();
}

