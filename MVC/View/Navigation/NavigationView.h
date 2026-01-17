#ifndef NAVIGATIONVIEW_H
#define NAVIGATIONVIEW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class Navigation_View; }
QT_END_NAMESPACE

class NavigationView : public QMainWindow {
Q_OBJECT

public:
    explicit NavigationView(QWidget *parent = nullptr);
    ~NavigationView() override;
    void addItem(const QString &clientName, bool status);
signals:
    void refreshSevers_signal();
    void connectPort_signal(int index);
    void disconnectPort_signal(int index);


private:
    Ui::Navigation_View *ui;
};


#endif //NAVIGATIONVIEW_H
