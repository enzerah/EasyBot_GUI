#ifndef MiscellaneousMODEL_H
#define MiscellaneousMODEL_H
#include <QObject>
#include <QVariant>
#include "MiscellaneousThread/Conditions_Thread.h"
#include "MiscellaneousThread/Others_Thread.h"

class MiscellaneousModel : public QObject {
    Q_OBJECT
public:
    explicit MiscellaneousModel(QObject *parent = nullptr);
    ~MiscellaneousModel() override;
    void startConditions(const QString &spellName, const int &manaCost, const QString &condition, bool state);
    void startOthers(const QString &option, const int &itemID, const int &condition, const int &minValue, bool state);

private:
    Conditions_Thread *conditionsThread = nullptr;
    Others_Thread *othersThread = nullptr;
};



#endif
