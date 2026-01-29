#ifndef ALARMSCONTROLLER_H
#define ALARMSCONTROLLER_H
#include <QObject>
#include "Alarms/AlarmsView.h"
#include "Alarms/AlarmsModel.h"


class AlarmsController : public QObject {
    Q_OBJECT
public:
    explicit AlarmsController(QObject *parent = nullptr);
    ~AlarmsController();

    void showView();

public slots:
    void startAlarms_slot(bool state);

private slots:
    void onSettingsChanged();

private:
    AlarmsView *m_view;
    AlarmsModel *m_model;
};



#endif //ALARMSCONTROLLER_H
