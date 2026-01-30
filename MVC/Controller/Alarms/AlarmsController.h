#ifndef ALARMSCONTROLLER_H
#define ALARMSCONTROLLER_H
#include <QObject>
#include "Alarms/AlarmsModel.h"
#include "Alarms/AlarmsView.h"


class AlarmsController : public QObject {
    Q_OBJECT
public:
    explicit AlarmsController(QObject *parent = nullptr);
    ~AlarmsController();

    void showView();


public slots:
    void startAlarms_slot(bool state);

private:
    AlarmsView *m_view;
    AlarmsModel *m_model;
};



#endif //ALARMSCONTROLLER_H
