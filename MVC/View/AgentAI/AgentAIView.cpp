#include "AgentAIView.h"


AgentAIView::AgentAIView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::AgentAI_View) {
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

AgentAIView::~AgentAIView() {
    delete ui;
}
