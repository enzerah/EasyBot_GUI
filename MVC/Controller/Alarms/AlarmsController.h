#ifndef ALARMSCONTROLLER_H
#define ALARMSCONTROLLER_H
#include <QObject>
#include "Alarms/AlarmsView.h"


class AlarmsController : public QObject {
    Q_OBJECT
public:
    explicit AlarmsController(QObject *parent = nullptr);
    ~AlarmsController();

    void showView();

private:
    AlarmsView *m_view;
};



#endif //ALARMSCONTROLLER_H
