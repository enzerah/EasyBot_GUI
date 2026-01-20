#ifndef LOOTITEMS_THREAD_H
#define LOOTITEMS_THREAD_H
#include <Qthread>
#include "../../const.h"
#include "../../proto_functions_client.h"


class LootItems_Thread : public QThread {
    Q_OBJECT
    public:
    explicit LootItems_Thread(const std::vector<Item> &items, QObject *parent = nullptr)
        : QThread(parent), items(items) {}
public slots:
protected:
    void run() override;
private:
    std::vector<Item> items;
};



#endif //LOOTITEMS_THREAD_H
