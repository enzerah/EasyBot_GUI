#ifndef STARTALARMS_THREAD_H
#define STARTALARMS_THREAD_H
#include <QObject>
#include <QThread>
#include "../../const.h"
#include "../../proto_functions_client.h"

class StartAlarms_Thread : public QThread {
    Q_OBJECT
    public:
    explicit StartAlarms_Thread(std::vector<Alarm> alarms, QObject *parent = nullptr) :
    m_alarms(alarms) {}
    protected:
    void run() override;

private:
    std::vector<Alarm> m_alarms;
};



#endif //STARTALARMS_THREAD_H
