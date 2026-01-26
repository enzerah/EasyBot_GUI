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
    connect(ui->playerBlackList_radioButton, &QRadioButton::toggled, this, &AlarmsView::settingsChanged);
    connect(ui->playerWhite_List_radioButton, &QRadioButton::toggled, this, &AlarmsView::settingsChanged);
    connect(ui->playerBlackList_textEdit, &QTextEdit::textChanged, this, &AlarmsView::settingsChanged);
    connect(ui->playerWhiteList_textEdit, &QTextEdit::textChanged, this, &AlarmsView::settingsChanged);
}

AlarmsView::~AlarmsView() {
    delete ui;
}

PlayAlarm_Thread::AlarmSettings AlarmsView::getSettings() const {
    PlayAlarm_Thread::AlarmSettings settings;
    settings.lowHealthEnabled = ui->lowHealth_checkBox->isChecked();
    settings.lowHealthValue = ui->lowHealth_lineEdit->text().toInt();
    if (settings.lowHealthValue == 0) settings.lowHealthValue = ui->lowHealth_lineEdit->placeholderText().toInt();

    settings.lowManaEnabled = ui->lowMana_checkBox->isChecked();
    settings.lowManaValue = ui->lowMana_lineEdit->text().toInt();
    if (settings.lowManaValue == 0) settings.lowManaValue = ui->lowMana_lineEdit->placeholderText().toInt();

    settings.playerBlackListMode = ui->playerBlackList_radioButton->isChecked();
    QStringList players;
    if (settings.playerBlackListMode) {
        players = ui->playerBlackList_textEdit->toPlainText().split('\n', Qt::SkipEmptyParts);
    } else {
        players = ui->playerWhiteList_textEdit->toPlainText().split('\n', Qt::SkipEmptyParts);
    }
    for (const QString& name : players) {
        settings.playerList.insert(name.trimmed().toStdString());
    }



    // Enable detection if either black list has content or white list mode is active
    settings.playerDetectedEnabled = !settings.playerBlackListMode || !settings.playerList.empty();
    settings.creatureDetectedEnabled = !settings.creatureBlackListMode || !settings.creatureList.empty();

    return settings;
}

