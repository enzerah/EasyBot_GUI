#ifndef AGENTAIMODEL_H
#define AGENTAIMODEL_H
#include <QObject>
#include <QVariant>
#include "../../const.h"


class AgentAIModel : public QObject {
    Q_OBJECT
public:
    explicit AgentAIModel(QObject *parent = nullptr);
    ~AgentAIModel();

    QJsonArray toJson() const;
    void fromJson(const QJsonArray &json);

};



#endif //AGENTAIMODEL_H
