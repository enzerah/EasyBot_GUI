#ifndef STARTHEALING_THREAD_H
#define STARTHEALING_THREAD_H
#include <Qthread>
#include "../../const.h"
#include "../../proto_functions_client.h"

class StartHealing_Thread : public QThread {
    Q_OBJECT
    public:
    explicit StartHealing_Thread(const std::vector<Heal> &heals, QObject *parent = nullptr)
    : QThread(parent), heals(heals) {}
    public slots:
    void updateData(std::vector<Heal>);
protected:
    void run() override;
private:
    std::vector<Heal> heals;
};



#endif //STARTHEALING_THREAD_H
