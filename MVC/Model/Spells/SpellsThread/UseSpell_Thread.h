#ifndef USESPELL_THREAD_H
#define USESPELL_THREAD_H
#include <QThread>
#include <QMutex>
#include "../../const.h"
#include "../../proto_functions_client.h"


class UseSpell_Thread : public QThread {
    Q_OBJECT
    public:
    explicit UseSpell_Thread(const std::vector<Spell> &spells, QObject *parent = nullptr)
    : QThread(parent), m_spells(spells) {}

    public slots:
    void updateData(std::vector<Spell> spells);

protected:
    void run() override;


private:
    std::vector<Spell> m_spells;
    QMutex m_mutex;
};

#endif //USESPELL_THREAD_H
