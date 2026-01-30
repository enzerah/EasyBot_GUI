#ifndef ALARMSVIEW_H
#define ALARMSVIEW_H
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Alarms_View; }
QT_END_NAMESPACE


class AlarmsView : public QMainWindow {
Q_OBJECT

public:
    explicit AlarmsView(QWidget *parent = nullptr);
    ~AlarmsView() override;

    signals:
    void addItem_signal(const QString& option, const int &value, const std::vector<QString> list);

private:
    Ui::Alarms_View *ui;
};



#endif //ALARMSVIEW_H
