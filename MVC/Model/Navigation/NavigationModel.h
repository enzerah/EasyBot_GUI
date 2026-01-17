#ifndef NavigationMODEL_H
#define NavigationMODEL_H
#include <QObject>
#include <QVariant>
#include "../../const.h"
#include "../../proto_functions_client.h"


class NavigationModel : public QObject {
    Q_OBJECT
public:
    explicit NavigationModel(QObject *parent = nullptr);
    ~NavigationModel();
    void refreshServers();
    signals:
    void addItem_signal(const QString &clientName, bool status);

private:
    std::vector<int> connectedPorts{};
};



#endif
