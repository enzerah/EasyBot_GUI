#include "TargetingModel.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


TargetingModel::TargetingModel(QObject* parent)
    : QObject(parent) {

}

TargetingModel::~TargetingModel() {
    if (attackTargetsThread) {
        attackTargetsThread->quit();
        attackTargetsThread->wait();
        delete attackTargetsThread;
    }
}

void TargetingModel::addItem(const QString &targetName, const int &dist, const int &count,
    const QString &desiredStance, const QString &monstersAttacks, bool openCorpse) {
    Target target;
    target.name = targetName.toStdString();
    std::transform(target.name .begin(), target.name .end(), target.name .begin(), ::tolower);
    target.count = count;
    target.dist = dist;
    target.desiredStance = desiredStance.toStdString();
    target.monstersAttacks = monstersAttacks.toStdString();
    target.openCorpse = openCorpse;
    targets.push_back(target);
    QString distString = QString::number(dist);
    QString countString = QString::number(count);

    if (!dist) distString = "All";
    if (count == 1) countString = "Any";
    emit addItem_signal(targetName, distString, countString, desiredStance, monstersAttacks, openCorpse);
}

void TargetingModel::shootableState(bool state) {
    m_shootable = state;
    if (attackTargetsThread) {
        emit shootableStateChanged_signal(state);
    }
}


void TargetingModel::reachableState(bool state) {
    m_reachable = state;
    if (attackTargetsThread) {
        emit reachableStateChanged_signal(state);
    }
}


void TargetingModel::stayAwayDist(int currentDist) {
    m_stayAwayDist = currentDist;
    if (attackTargetsThread) {
        emit stayAwayDistChanged_signal(currentDist);
    }
}


void TargetingModel::startTargeting(bool state) {
    if (state) {
        if (!attackTargetsThread) {
            attackTargetsThread = new AttackTargets_Thread(targets, m_reachable, m_shootable, m_stayAwayDist, blockedTiles, this);
            connect(this, &TargetingModel::shootableStateChanged_signal,attackTargetsThread, &AttackTargets_Thread::shootableStateChange);
            connect(this, &TargetingModel::reachableStateChanged_signal,attackTargetsThread, &AttackTargets_Thread::reachableStateChange);
            connect(this, &TargetingModel::stayAwayDistChanged_signal,attackTargetsThread, &AttackTargets_Thread::stayAwayDistChange);
            connect(this, &TargetingModel::updateData_signal,attackTargetsThread, &AttackTargets_Thread::updateData);
            connect(attackTargetsThread, &QThread::finished, attackTargetsThread, &QObject::deleteLater);
            connect(attackTargetsThread, &QThread::finished, this, [this]() {
                this->attackTargetsThread = nullptr;
            });
            attackTargetsThread->start();
        }
    } else {
        if (attackTargetsThread) {
            attackTargetsThread->requestInterruption();
            attackTargetsThread->quit();
        }
    }
}

QJsonArray TargetingModel::toJson() const {
    QJsonArray jsonArray;
    for (const auto &target : targets) {
        QJsonObject jsonObj;
        jsonObj["name"] = QString::fromStdString(target.name);
        jsonObj["dist"] = target.dist;
        jsonObj["count"] = target.count;
        jsonObj["stance"] = QString::fromStdString(target.desiredStance);
        jsonObj["attack"] = QString::fromStdString(target.monstersAttacks);
        jsonObj["open"] = target.openCorpse;
        jsonArray.append(jsonObj);
    }
    QJsonArray blockedTilesArray;
    for (const auto &tile : blockedTiles) {
        QJsonObject tileObj;
        tileObj["x"] = static_cast<int>(tile.x);
        tileObj["y"] = static_cast<int>(tile.y);
        tileObj["z"] = static_cast<int>(tile.z);
        blockedTilesArray.append(tileObj);
    }
    QJsonObject mainObj;
    mainObj["targets"] = jsonArray;
    mainObj["blockedTiles"] = blockedTilesArray;
    
    QJsonArray result;
    result.append(mainObj);
    return result;
}

void TargetingModel::fromJson(const QJsonArray &json) {
    targets.clear();
    blockedTiles.clear();
    emit clearListWidget_signal();
    emit clearBlockedTilesListWidget_signal();
    if (!json.isEmpty() && json[0].isObject()) {
        QJsonObject mainObj = json[0].toObject();
        if (mainObj.contains("targets")) {
            QJsonArray targetsArray = mainObj["targets"].toArray();
            for (const auto &val : targetsArray) {
                QJsonObject obj = val.toObject();
                QString name = obj["name"].toString();
                int dist = obj["dist"].toInt();
                int count = obj["count"].toInt();
                QString stance = obj["stance"].toString();
                QString attack = obj["attack"].toString();
                bool open = obj["open"].toBool();
                addItem(name, dist, count, stance, attack, open);
            }
        }
        if (mainObj.contains("blockedTiles")) {
            QJsonArray tilesArray = mainObj["blockedTiles"].toArray();
            for (const auto &val : tilesArray) {
                QJsonObject tileObj = val.toObject();
                int x = tileObj["x"].toInt();
                int y = tileObj["y"].toInt();
                int z = tileObj["z"].toInt();
                addBlockedTile(x, y, z);
            }
        }
    } else {
        for (const auto &val : json) {
            QJsonObject obj = val.toObject();
            QString name = obj["name"].toString();
            int dist = obj["dist"].toInt();
            int count = obj["count"].toInt();
            QString stance = obj["stance"].toString();
            QString attack = obj["attack"].toString();
            bool open = obj["open"].toBool();
            addItem(name, dist, count, stance, attack, open);
        }
    }
}

void TargetingModel::deleteItem(const int& index) {
    targets.erase(targets.begin() + index);
}

void TargetingModel::clearListWidget() {
    targets.clear();
}

void TargetingModel::addBlockedTile(const int &x, const int &y, const int &z) {
    Position pos;
    pos.x = static_cast<uint16_t>(x);
    pos.y = static_cast<uint16_t>(y);
    pos.z = static_cast<uint8_t>(z);
    blockedTiles.push_back(pos);
    
    QString tileStr = QStringLiteral("X: %1, Y: %2, Z: %3").arg(x).arg(y).arg(z);
    emit addBlockedTile_signal(tileStr);
}

void TargetingModel::deleteBlockedTile(const int &index) {
    if (index >= 0 && index < static_cast<int>(blockedTiles.size())) {
        blockedTiles.erase(blockedTiles.begin() + index);
    }
}


