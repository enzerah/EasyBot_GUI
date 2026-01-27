#include "LootingModel.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>




LootingModel::LootingModel(QObject *parent)
    : QObject(parent) {
}

LootingModel::~LootingModel() {
    if (lootItemsThread) {
        lootItemsThread->quit();
        lootItemsThread->wait();
        delete lootItemsThread;
    }
}

void LootingModel::addItem(const QString &srcItem, const QString &dstItem, const QString &delay) {
    Item item;
    item.itemID = srcItem.toInt();
    auto dest= dstItem.toStdString();
    std::transform(dest.begin(), dest.end(), dest.begin(), ::tolower);
    item.destination = dest;
    item.delay = delay.toInt();
    items.push_back(item);
    emit addItem_signal(srcItem, dstItem, delay);
    emit updateData_signal(items);
}

void LootingModel::startLooting(bool state) {
    if (state) {
        if (!lootItemsThread) {
            lootItemsThread = new LootItems_Thread(items, this);
            connect(lootItemsThread, &QThread::finished, lootItemsThread, &QObject::deleteLater);
            connect(this, &LootingModel::updateData_signal,lootItemsThread, &LootItems_Thread::updateData);
            connect(lootItemsThread, &QThread::finished, this, [this]() {
                this->lootItemsThread = nullptr;
            });
            lootItemsThread->start();
        }
    } else {
        if (lootItemsThread) {
            lootItemsThread->requestInterruption();
            lootItemsThread->quit();
        }
    }
}


QJsonArray LootingModel::toJson() const {
    QJsonArray jsonArray;
    for (const auto &item : items) {
        QJsonObject jsonObj;
        jsonObj["id"] = item.itemID;
        jsonObj["destination"] = QString::fromStdString(item.destination);
        jsonObj["delay"] = item.delay;
        jsonArray.append(jsonObj);
    }
    return jsonArray;
}

void LootingModel::fromJson(const QJsonArray &json) {
    items.clear();
    emit clearTableWidget_signal();
    for (const auto &val : json) {
        QJsonObject obj = val.toObject();
        QString srcItem = obj["id"].toString();
        QString dest = obj["destination"].toString();
        QString delay = obj["delay"].toString();
        addItem(srcItem, dest, delay);
    }
}

void LootingModel::deleteItem(const int& index) {
    items.erase(items.begin() + index);
    emit updateData_signal(items);
}

void LootingModel::clearTableWidget() {
    items.clear();
    emit updateData_signal(items);
}


