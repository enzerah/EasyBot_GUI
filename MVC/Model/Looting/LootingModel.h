#ifndef LOOTINGMODEL_H
#define LOOTINGMODEL_H
#include <QObject>
#include "../../const.h"
#include "LootingThread/LootItems_Thread.h"

class LootingModel : public QObject {
    Q_OBJECT
public:
    explicit LootingModel(QObject *parent = nullptr);
    ~LootingModel();

    void addItem(const QString &srcItem, const QString &dstItem, const QString &delay);
    void startLooting(bool state);

    QJsonArray toJson() const;
    void fromJson(const QJsonArray &json);

    void deleteItem(const int &index);
    void clearTableWidget();

    signals:
    void addItem_signal(const QString &srcItem, const QString &dstItem, const QString &delay);
    void clearTableWidget_signal();


private:
    std::vector<Item> items;
    LootItems_Thread *lootItemsThread = nullptr;
};



#endif //LOOTINGMODEL_H
