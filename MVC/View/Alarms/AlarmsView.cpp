//
// Created by Wojciech on 08.01.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AlarmsView.h" resolved

#include "AlarmsView.h"
#include "ui_AlarmsView.h"


AlarmsView::AlarmsView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Alarms_View) {
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    connect(ui->lowHealth_checkBox, &QCheckBox::stateChanged, this, &AlarmsView::settingsChanged);
    connect(ui->lowHealth_lineEdit, &QLineEdit::textChanged, this, &AlarmsView::settingsChanged);
    connect(ui->lowMana_checkBox, &QCheckBox::stateChanged, this, &AlarmsView::settingsChanged);
    connect(ui->lowMana_lineEdit, &QLineEdit::textChanged, this, &AlarmsView::settingsChanged);
    connect(ui->defaultMessage_checkBox, &QCheckBox::stateChanged, this, &AlarmsView::settingsChanged);
    connect(ui->privateMessage_checkBox, &QCheckBox::stateChanged, this, &AlarmsView::settingsChanged);
    connect(ui->disconnect_checkBox, &QCheckBox::stateChanged, this, &AlarmsView::settingsChanged);
    
    connect(ui->creatureBlackList_radioButton, &QRadioButton::toggled, this, &AlarmsView::settingsChanged);
    connect(ui->creatureWhite_List_radioButton, &QRadioButton::toggled, this, &AlarmsView::settingsChanged);
    connect(ui->creatureBlackList_textEdit, &QTextEdit::textChanged, this, &AlarmsView::settingsChanged);
    connect(ui->creatureWhiteList_textEdit, &QTextEdit::textChanged, this, &AlarmsView::settingsChanged);
    connect(ui->creatureDetected_checkBox, &QCheckBox::stateChanged, this, &AlarmsView::settingsChanged);
}

AlarmsView::~AlarmsView() {
    delete ui;
}

PlayAlarm_Thread::AlarmSettings AlarmsView::getSettings() const {
    PlayAlarm_Thread::AlarmSettings settings;
    settings.lowHealthEnabled = ui->lowHealth_checkBox->isChecked();
    settings.lowHealthValue = ui->lowHealth_lineEdit->text().toInt();
    if (settings.lowHealthValue <= 0) settings.lowHealthValue = ui->lowHealth_lineEdit->placeholderText().toInt();

    settings.lowManaEnabled = ui->lowMana_checkBox->isChecked();
    settings.lowManaValue = ui->lowMana_lineEdit->text().toInt();
    if (settings.lowManaValue <= 0) settings.lowManaValue = ui->lowMana_lineEdit->placeholderText().toInt();

    settings.defaultMessageEnabled = ui->defaultMessage_checkBox->isChecked();
    settings.privateMessageEnabled = ui->privateMessage_checkBox->isChecked();
    settings.disconnectEnabled = ui->disconnect_checkBox->isChecked();

    settings.creatureBlackListMode = ui->creatureBlackList_radioButton->isChecked();
    settings.creatureDetectedEnabled = ui->creatureDetected_checkBox->isChecked();

    QString listText = settings.creatureBlackListMode ? ui->creatureBlackList_textEdit->toPlainText() : ui->creatureWhiteList_textEdit->toPlainText();

    return settings;
}

