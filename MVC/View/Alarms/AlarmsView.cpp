#include "AlarmsView.h"

#include <QRegularExpression>

#include "ui_AlarmsView.h"


AlarmsView::AlarmsView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Alarms_View) {
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    connect(ui->lowHealth_checkBox, &QCheckBox::clicked, this, [this](bool checked){
        ui->lowHealth_lineEdit->setEnabled(!checked);
        emit addItem_signal("Low Health", ui->lowHealth_lineEdit->text().toInt(), {""});
    });

    connect(ui->lowMana_checkBox, &QCheckBox::clicked, this, [this](bool checked){
        ui->lowMana_lineEdit->setEnabled(!checked);
        emit addItem_signal("Low Mana", ui->lowMana_lineEdit->text().toInt(), {""});
    });

    connect(ui->creatureDetected_checkBox, &QCheckBox::clicked, this, [this](bool checked){
        ui->groupBox->setEnabled(!checked);
        std::vector<QString> names{};
        if (ui->creatureBlackList_radioButton->isChecked()) {
            auto plainText = ui->creatureBlackList_textEdit->toPlainText();
            QStringList lines = plainText.split(QRegularExpression("[\n]+"),Qt::SkipEmptyParts);
            for (auto name : lines) {
                names.push_back(name);
            }
            emit addItem_signal("Creature Detected", 0, names);
        } else {
            auto plainText = ui->creatureWhiteList_textEdit->toPlainText();
            QStringList lines = plainText.split(QRegularExpression("[\n]+"),Qt::SkipEmptyParts);
            for (auto name : lines) {
                names.push_back(name);
            }
            emit addItem_signal("Creature Detected", 1, names);
        }
    });

    connect(ui->privateMessage_checkBox, &QCheckBox::clicked, this, [this](bool checked) {
        emit addItem_signal("Private Message", 0, {""});
    });

    connect(ui->defaultMessage_checkBox, &QCheckBox::clicked, this, [this](bool checked) {
        emit addItem_signal("Default Message", 0, {""});
    });

    connect(ui->disconnect_checkBox, &QCheckBox::clicked, this, [this](bool checked) {
        emit addItem_signal("Disconnect", 0, {""});
    });

}

AlarmsView::~AlarmsView() {
    delete ui;
}

