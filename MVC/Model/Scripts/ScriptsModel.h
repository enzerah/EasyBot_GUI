#ifndef SCRIPTSMODEL_H
#define SCRIPTSMODEL_H
#include <QObject>
#include <QVariant>
#include "../../const.h"

class ScriptsModel : public QObject {
    Q_OBJECT
public:
    explicit ScriptsModel(QObject *parent = nullptr);
    ~ScriptsModel();

    QJsonArray toJson() const;
    void fromJson(const QJsonArray &json);

};



#endif // SCRIPTSMODEL_H
