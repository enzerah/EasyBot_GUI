#include "ScriptsView.h"
#include "ui_ScriptsView.h"


ScriptsView::ScriptsView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Scripts_View) {
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

ScriptsView::~ScriptsView() {
    delete ui;
}
