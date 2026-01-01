#ifndef AGENTAICONTROLLER_H
#define AGENTAICONTROLLER_H
#include <QObject>
#include "AgentAI/AgentAIView.h"
#include "AgentAI/AgentAIModel.h"


class AgentAIController : public QObject {
    Q_OBJECT
public:
    explicit AgentAIController(QObject *parent = nullptr);
    ~AgentAIController();

    void showView();
    QJsonArray saveSettings() const;
    void loadSettings(const QJsonArray &json);



private:
    AgentAIView *m_view;
    AgentAIModel *m_model;
};


#endif //AGENTAICONTROLLER_H
