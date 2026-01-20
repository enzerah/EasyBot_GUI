#ifndef LOOTINGCONTROLLER_H
#define LOOTINGCONTROLLER_H
#include <QObject>

#include "Looting/LootingModel.h"
#include "Looting/LootingView.h"


class LootingController : public QObject {
    Q_OBJECT
public:
    explicit LootingController(QObject *parent = nullptr);
    ~LootingController();

    void showView();
    QJsonArray saveSettings() const;
    void loadSettings(const QJsonArray &json);

public slots:
    void startLooting_slot(bool state);

private:
    LootingView *m_view;
    LootingModel *m_model;
};


#endif //LOOTINGCONTROLLER_H
