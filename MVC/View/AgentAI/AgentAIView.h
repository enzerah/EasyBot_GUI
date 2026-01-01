#ifndef AGENTAI_H
#define AGENTAI_H

#include <QMainWindow>
#include "ui_AgentAIView.h"


QT_BEGIN_NAMESPACE
namespace Ui { class AgentAI_View; }
QT_END_NAMESPACE

class AgentAIView : public QMainWindow {
Q_OBJECT

public:
    explicit AgentAIView(QWidget *parent = nullptr);
    ~AgentAIView() override;

private:
    Ui::AgentAI_View *ui;
};


#endif //AGENTAI_H
