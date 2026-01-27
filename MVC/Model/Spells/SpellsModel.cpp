#include "SpellsModel.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>



SpellsModel::SpellsModel(QObject *parent)
    : QObject(parent) {
}

SpellsModel::~SpellsModel() {
    if (useSpellThread) {
        useSpellThread->quit();
        useSpellThread->wait();
        delete useSpellThread;
    }
}

void SpellsModel::addItem(const QString &target, const int &option, const QString &spellName, const int &count,
    const int &dist, const int &minHp, const int &costMp, int priority, bool requiresTarget, bool playerProtection) {
    Spell spell;
    spell.targetName = target.toStdString();
    std::transform(spell.targetName.begin(), spell.targetName.end(), spell.targetName.begin(), ::tolower);
    spell.option = option;
    spell.spellName = spellName.toStdString();
    spell.count = count;
    spell.dist = dist;
    spell.minHp = minHp;
    spell.costMp = costMp;
    spell.priority = priority;
    spell.requiresTarget = requiresTarget;
    spell.playerProtection = playerProtection;
    spells.push_back(spell);
    emit addItem_signal(target, option, spellName, count, dist, minHp, costMp, priority, requiresTarget, playerProtection);
    emit updateData_signal(spells);
}

void SpellsModel::startSpells(bool state) {
    if (state) {
        if (!useSpellThread) {
            useSpellThread = new UseSpell_Thread(spells, this);
            connect(this, &SpellsModel::updateData_signal,useSpellThread, &UseSpell_Thread::updateData);
            connect(useSpellThread, &QThread::finished, useSpellThread, &QObject::deleteLater);
            connect(useSpellThread, &QThread::finished, this, [this]() {
                this->useSpellThread = nullptr;
            });
            useSpellThread->start();
        }
    } else {
        if (useSpellThread) {
            useSpellThread->requestInterruption();
            useSpellThread->quit();
        }
    }
}

QJsonArray SpellsModel::toJson() const {
    QJsonArray jsonArray;
    for (const auto &spell : spells) {
        QJsonObject jsonObj;
        jsonObj["targetName"] = QString::fromStdString(spell.targetName);
        jsonObj["dist"] = spell.dist;
        jsonObj["count"] = spell.count;
        jsonObj["option"] = spell.option;
        jsonObj["spellName"] = QString::fromStdString(spell.spellName);
        jsonObj["priority"] = spell.priority;
        jsonObj["minHp"] = spell.minHp;
        jsonObj["costMp"] = spell.costMp;
        jsonObj["protection"] = spell.playerProtection;
        jsonObj["requiresTarget"] = spell.requiresTarget;
        jsonArray.append(jsonObj);
    }
    return jsonArray;
}

void SpellsModel::fromJson(const QJsonArray &json) {
    spells.clear();
    emit clearTableWidget_signal();
    for (const auto &val : json) {
        QJsonObject obj = val.toObject();
        QString target = obj["targetName"].toString();
        int option = obj["option"].toInt();
        QString spellName = obj["spellName"].toString();
        int dist = obj["dist"].toInt();
        int count = obj["count"].toInt();
        int minHp = obj["minHp"].toInt();
        int costMp = obj["costMp"].toInt();
        int priority = obj["priority"].toInt();
        bool playerProtection = obj["protection"].toBool();
        bool requiresTarget = obj["requiresTarget"].toBool();
        addItem(target, option, spellName, count, dist, minHp, costMp, priority, requiresTarget, playerProtection);
    }
}

void SpellsModel::deleteItem(const int& index) {
    spells.erase(spells.begin() + index);
    emit updateData_signal(spells);
}

void SpellsModel::clearTableWidget() {
    spells.clear();
    emit updateData_signal(spells);
}

