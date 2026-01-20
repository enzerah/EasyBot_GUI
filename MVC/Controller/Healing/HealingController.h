#ifndef HEALINGCONTROLLER_H
#define HEALINGCONTROLLER_H
#include <QObject>

#include "Healing/HealingModel.h"
#include "Healing/HealingView.h"


class HealingController : public QObject {
    Q_OBJECT
public:
    explicit HealingController(QObject *parent = nullptr);
    ~HealingController();

    void showView();
    QJsonArray saveSettings() const;
    void loadSettings(const QJsonArray &json);

public slots:
void startHealing_slot(bool state);

private:
    HealingView *m_view;
    HealingModel *m_model;
};


#endif //HEALINGCONTROLLER_H
