#include "AlarmsView.h"
#include "ui_AlarmsView.h"


AlarmsView::AlarmsView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Alarms_View) {
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    connect(ui->lowHealth_checkBox, &QCheckBox::clicked, this, [this](bool checked){
        ui->lowHealth_lineEdit->setEnabled(!checked);
    });

    connect(ui->lowMana_checkBox, &QCheckBox::clicked, this, [this](bool checked){
        ui->lowMana_lineEdit->setEnabled(!checked);
    });

    connect(ui->creatureDetected_checkBox, &QCheckBox::clicked, this, [this](bool checked){
        ui->groupBox->setEnabled(!checked);
    });

}

AlarmsView::~AlarmsView() {
    delete ui;
}

