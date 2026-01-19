#include "MiscellaneousView.h"
#include "ui_MiscellaneousView.h"


MiscellaneousView::MiscellaneousView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Miscellaneous_View) {

    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    // Conditions
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

    // Others
    connect(ui->refilAmmo_checkBox, &QCheckBox::toggled, this, [this](bool checked) {
        emit startOthers_signal("Ammo", ui->ammoID_lineEdit->text().toInt(), 0, 0, checked);
        ui->ammoID_lineEdit->setEnabled(!checked);
    });
    connect(ui->autoRing_checkBox, &QCheckBox::toggled, this, [this](bool checked) {
        emit startOthers_signal("Ring", ui->ringID_lineEdit->text().toInt(), ui->ringOption_comboBox->currentIndex(), ui->ringValue_lineEdit->text().toInt(), checked);
        ui->ringID_lineEdit->setEnabled(!checked);
        ui->ringOption_comboBox->setEnabled(!checked);
        ui->ringValue_lineEdit->setEnabled(!checked);
    });
    connect(ui->autoAmulet_checkBox, &QCheckBox::toggled, this, [this](bool checked) {
        emit startOthers_signal("Amulet", ui->amuletID_lineEdit->text().toInt(), ui->amuletOption_comboBox->currentIndex(), ui->amuletValue_lineEdit->text().toInt(), checked);
        ui->amuletID_lineEdit->setEnabled(!checked);
        ui->amuletOption_comboBox->setEnabled(!checked);
        ui->amuletValue_lineEdit->setEnabled(!checked);
    });
}

MiscellaneousView::~MiscellaneousView() {
    delete ui;
}

