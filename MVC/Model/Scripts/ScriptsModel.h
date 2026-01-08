#ifndef SCRIPTSMODEL_H
#define SCRIPTSMODEL_H
#include <QObject>
#include <QVariant>
#include <QJsonArray>
#include <vector>
#include "../../const.h"
#include "ScriptsThread/RunScripts_Thread.h"


class ScriptsModel : public QObject {
    Q_OBJECT
public:
    explicit ScriptsModel(QObject *parent = nullptr);
    ~ScriptsModel();


    void addItem(const int &id, bool state, const QString &name, const QString &script_text);
    void deleteItem(const int &index);
    void startScripts(bool state);

    QJsonArray toJson() const;
    void fromJson(const QJsonArray &json);

    signals:
    void clearTableWidget_signal();
    void addItem_signal(bool state, const QString &name, const QString &script_text);

private:
    std::vector<Script> scripts;
    RunScripts_Thread *runScriptsThread = nullptr;
};

#endif // SCRIPTSMODEL_H
