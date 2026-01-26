//
// Created by Wojciech on 08.01.2026.
//

#ifndef ALARMSVIEW_H
#define ALARMSVIEW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class Alarms_View; }
QT_END_NAMESPACE

#include "../Model/Alarms/AlarmsThread/PlayAlarm_Thread.h"

class AlarmsView : public QMainWindow {
Q_OBJECT

public:
    explicit AlarmsView(QWidget *parent = nullptr);
    ~AlarmsView() override;

    PlayAlarm_Thread::AlarmSettings getSettings() const;

signals:
    void settingsChanged();

private:
    Ui::Alarms_View *ui;
};



#endif //ALARMSVIEW_H
