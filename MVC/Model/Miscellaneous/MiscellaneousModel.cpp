#include "MiscellaneousModel.h"
#include <QJsonObject>


MiscellaneousModel::MiscellaneousModel(QObject *parent)
    : QObject(parent) {

}

MiscellaneousModel::~MiscellaneousModel() {
    if (conditionsThread) {
        conditionsThread->requestInterruption();
        conditionsThread->quit();
        conditionsThread->wait();
    }
    if (othersThread) {
        othersThread->requestInterruption();
        othersThread->quit();
        othersThread->wait();
    }
}

void MiscellaneousModel::startConditions(const QString& spellName, const int& manaCost, const QString& condition, bool state) {
    if (state) {
        if (!conditionsThread) {
            conditionsThread = new Conditions_Thread(this);
            connect(conditionsThread, &QThread::finished, conditionsThread, &QObject::deleteLater);
            connect(conditionsThread, &QThread::finished, this, [this]() {
                this->conditionsThread = nullptr;
            });
            conditionsThread->start();
        }
        conditionsThread->updateCondition(condition.toStdString(), spellName.toStdString(), manaCost, true);
    } else {
        if (conditionsThread) {
            conditionsThread->updateCondition(condition.toStdString(), spellName.toStdString(), manaCost, false);
        }
    }
}

void MiscellaneousModel::startOthers(const QString &option, const int &itemID, const int &condition, const int &minValue, bool state) {
    if (state) {
        if (!othersThread) {
            othersThread = new Others_Thread(this);
            connect(othersThread, &QThread::finished, othersThread, &QObject::deleteLater);
            connect(othersThread, &QThread::finished, this, [this]() {
                this->othersThread = nullptr;
            });
            othersThread->start();
        }
        othersThread->updateCondition(option.toStdString(), itemID, condition, minValue, true);
    } else {
        if (othersThread) {
            othersThread->updateCondition(option.toStdString(), itemID, condition, minValue, false);
        }
    }
}



