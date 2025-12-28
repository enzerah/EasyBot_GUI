#ifndef SCRIPTSCONTROLLER_H
#define SCRIPTSCONTROLLER_H
#include <QObject>
#include <QJsonArray>
#include "Scripts/ScriptsModel.h"
#include "Scripts/ScriptsView.h"


class ScriptsController : public QObject {
    Q_OBJECT
public:
    explicit ScriptsController(QObject *parent = nullptr);
    ~ScriptsController();

    void showView();

    QJsonArray saveSettings() const;
    void loadSettings(const QJsonArray &json);



private:
    ScriptsView *m_view;
    ScriptsModel *m_model;
};


#endif // SCRIPTSCONTROLLER_H
