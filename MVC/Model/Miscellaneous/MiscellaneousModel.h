#ifndef MiscellaneousMODEL_H
#define MiscellaneousMODEL_H
#include <QObject>
#include <QVariant>
#include "MiscellaneousThread/Conditions_Thread.h"

class MiscellaneousModel : public QObject {
    Q_OBJECT
public:
    explicit MiscellaneousModel(QObject *parent = nullptr);
    ~MiscellaneousModel();
    void startConditions(const QString &spellName, const int &manaCost, const QString &condition, bool state);

private:
    Conditions_Thread *unified_conditionsThread = nullptr;
};



#endif
