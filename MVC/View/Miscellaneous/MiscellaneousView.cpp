#include "MiscellaneousView.h"
#include "ui_MiscellaneousView.h"


MiscellaneousView::MiscellaneousView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Miscellaneous_View) {

    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    connect(ui->haste_checkBox, &QCheckBox::toggled, this, [this](bool checked) {
        emit startCondition_signal(ui->hasteName_lineEdit->text(), ui->hasteMana_lineEdit->text().toInt(), "Haste", ui->haste_checkBox->checkState());
        ui->hasteName_lineEdit->setEnabled(!checked);
        ui->hasteMana_lineEdit->setEnabled(!checked);
    });
    connect(ui->shield_checkBox, &QCheckBox::toggled, this, [this](bool checked) {
        emit startCondition_signal(ui->shieldName_lineEdit->text(), ui->shieldMana_lineEdit->text().toInt(), "ManaShield", ui->shield_checkBox->checkState());
        ui->shieldName_lineEdit->setEnabled(!checked);
        ui->shieldMana_lineEdit->setEnabled(!checked);
    });
    connect(ui->buff_checkBox, &QCheckBox::toggled, this, [this](bool checked) {
        emit startCondition_signal(ui->buffName_lineEdit->text(), ui->buffMana_lineEdit->text().toInt(), "Buff", ui->buff_checkBox->checkState());
        ui->buffName_lineEdit->setEnabled(!checked);
        ui->buffMana_lineEdit->setEnabled(!checked);
    });
    connect(ui->paral_checkBox, &QCheckBox::toggled, this, [this](bool checked) {
        emit startCondition_signal(ui->paralName_lineEdit->text(), ui->paralMana_lineEdit->text().toInt(), "AntyParal", ui->paral_checkBox->checkState());
        ui->paralName_lineEdit->setEnabled(!checked);
        ui->paralMana_lineEdit->setEnabled(!checked);
    });
}

MiscellaneousView::~MiscellaneousView() {
    delete ui;
}
