#include "MiscellaneousModel.h"
#include <QJsonObject>


MiscellaneousModel::MiscellaneousModel(QObject *parent)
    : QObject(parent) {

}

MiscellaneousModel::~MiscellaneousModel() {
    if (unified_conditionsThread) {
        unified_conditionsThread->requestInterruption();
        unified_conditionsThread->quit();
        unified_conditionsThread->wait();
    }
}

void MiscellaneousModel::startConditions(const QString& spellName, const int& manaCost, const QString& condition,
    bool state) {
    if (state) {
        if (!unified_conditionsThread) {
            unified_conditionsThread = new Conditions_Thread(this);
            connect(unified_conditionsThread, &QThread::finished, unified_conditionsThread, &QObject::deleteLater);
            connect(unified_conditionsThread, &QThread::finished, this, [this]() {
                this->unified_conditionsThread = nullptr;
            });
            unified_conditionsThread->start();
        }
        unified_conditionsThread->updateCondition(condition.toStdString(), spellName.toStdString(), manaCost, true);
    } else {
        if (unified_conditionsThread) {
            unified_conditionsThread->updateCondition(condition.toStdString(), spellName.toStdString(), manaCost, false);
        }
    }
}



