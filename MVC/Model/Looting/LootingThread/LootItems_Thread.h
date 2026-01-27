#ifndef LOOTITEMS_THREAD_H
#define LOOTITEMS_THREAD_H
#include <Qthread>
#include <QMutex>
#include "../../const.h"
#include "../../proto_functions_client.h"


class LootItems_Thread : public QThread {
    Q_OBJECT
    public:
    explicit LootItems_Thread(const std::vector<Item> &items, QObject *parent = nullptr)
        : QThread(parent), m_items(items) {}
public slots:
    void updateData(std::vector<Item> items);
protected:
    void run() override;
private:
    std::vector<Item> m_items;
    QMutex m_mutex;
};



#endif //LOOTITEMS_THREAD_H
