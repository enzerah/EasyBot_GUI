#ifndef STARTHEALING_THREAD_H
#define STARTHEALING_THREAD_H
#include <Qthread>
#include <QMutex>
#include "../../const.h"
#include "../../proto_functions_client.h"

class StartHealing_Thread : public QThread {
    Q_OBJECT
    public:
    explicit StartHealing_Thread(const std::vector<Heal> &heals, QObject *parent = nullptr)
    : QThread(parent), m_heals(heals) {}
    public slots:
    void updateData(std::vector<Heal> heals);
protected:
    void run() override;
private:
    std::vector<Heal> m_heals;
    QMutex m_mutex;
};



#endif //STARTHEALING_THREAD_H
