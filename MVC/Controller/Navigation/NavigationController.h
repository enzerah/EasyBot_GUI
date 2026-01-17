#ifndef NavigationController_H
#define NavigationController_H
#include <QObject>
#include "Navigation/NavigationView.h"
#include "Navigation/NavigationModel.h"


class NavigationController : public QObject {
    Q_OBJECT
public:
    explicit NavigationController(QObject *parent = nullptr);
    ~NavigationController();

    void showView();

private slots:
    void refreshServers_View();

private:
    NavigationView *m_view;
    NavigationModel *m_model;
};


#endif
