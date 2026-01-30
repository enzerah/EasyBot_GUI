#include "AlarmsView.h"

#include <QRegularExpression>

#include "ui_AlarmsView.h"


AlarmsView::AlarmsView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Alarms_View) {
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    connect(ui->lowHealth_checkBox, &QCheckBox::clicked, this, [this](bool checked){
        ui->lowHealth_lineEdit->setEnabled(!checked);
        emit addItem_signal("Low Health", ui->lowHealth_lineEdit->text().toInt(), {""}, checked);
    });

    connect(ui->lowMana_checkBox, &QCheckBox::clicked, this, [this](bool checked){
        ui->lowMana_lineEdit->setEnabled(!checked);
        emit addItem_signal("Low Mana", ui->lowMana_lineEdit->text().toInt(), {""}, checked);
    });

    auto updateCreatureDetected = [this](){
        bool checked = ui->creatureDetected_checkBox->isChecked();
        ui->groupBox->setEnabled(!checked);
        std::vector<QString> names{};
        int value = 0; // Blacklist
        QString plainText;
        if (ui->creatureBlackList_radioButton->isChecked()) {
            plainText = ui->creatureBlackList_textEdit->toPlainText();
            value = 0;
        } else {
            plainText = ui->creatureWhiteList_textEdit->toPlainText();
            value = 1; // Whitelist
        }
        QStringList lines = plainText.split(QRegularExpression("[\n]+"), Qt::SkipEmptyParts);
        for (const auto& name : lines) {
            names.push_back(name);
        }
        emit addItem_signal("Creature Detected", value, names, checked);
    };

    connect(ui->creatureDetected_checkBox, &QCheckBox::clicked, this, updateCreatureDetected);
    connect(ui->creatureBlackList_radioButton, &QRadioButton::clicked, this, updateCreatureDetected);
    connect(ui->creatureWhiteList_radioButton, &QRadioButton::clicked, this, updateCreatureDetected);
    connect(ui->creatureBlackList_textEdit, &QTextEdit::textChanged, this, updateCreatureDetected);
    connect(ui->creatureWhiteList_textEdit, &QTextEdit::textChanged, this, updateCreatureDetected);

    connect(ui->disconnect_checkBox, &QCheckBox::clicked, this, [this](bool checked) {
        emit addItem_signal("Disconnect", 0, {""}, checked);
    });

}

AlarmsView::~AlarmsView() {
    delete ui;
}

