#ifndef CONDITIONS_THREAD_H
#define CONDITIONS_THREAD_H
#include <QThread>
#include <QMutex>
#include <map>
#include "../../const.h"
#include "../../proto_functions_client.h"

class Conditions_Thread : public QThread {
    Q_OBJECT
public:
    struct ConditionData {
        std::string spellName;
        int manaCost;
    };

    explicit Conditions_Thread(QObject *parent = nullptr)
        : QThread(parent) {};
    ~Conditions_Thread() override = default;

    void updateCondition(const std::string& conditionType, const std::string& spellName, int manaCost, bool state);
    bool hasActiveConditions();

protected:
    void run() override;

private:
    std::map<std::string, ConditionData> activeConditions;
    QMutex mutex;
};

#endif //CONDITIONS_THREAD_H
