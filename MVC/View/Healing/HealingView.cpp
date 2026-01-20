#include "HealingView.h"

#include <iostream>


HealingView::HealingView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Healing_View) {
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    ui->healing_tableWidget->setColumnCount(4);
    ui->healing_tableWidget->setHorizontalHeaderLabels({"Action", "Type", "Below", "Above"});
    ui->healing_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->healing_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->healing_tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->healing_tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    connect(ui->add_pushButton, &QPushButton::clicked, this, [this]() {
        auto action = ui->action_comboBox->currentText();
        auto heal = ui->heal_lineEdit->text();
        auto condition = ui->condition_comboBox->currentText();
        auto random = ui->random_lineEdit->text().toInt();
        auto below = ui->belowHp_lineEdit->text().toInt();
        auto above = ui->aboveHp_lineEdit->text().toInt();
        auto minimum = ui->minimum_lineEdit->text().toInt();
        emit addItem_signal(action, heal, condition, random, below, above, minimum);
        ui->action_comboBox->setCurrentIndex(0);
        ui->condition_comboBox->setCurrentIndex(0);
        ui->heal_lineEdit->clear();
        ui->random_lineEdit->clear();
        ui->belowHp_lineEdit->clear();
        ui->aboveHp_lineEdit->clear();
        ui->minimum_lineEdit->clear();
    });

    connect(ui->healing_tableWidget, &QTableWidget::cellDoubleClicked, this, [this](int row, int column){
        ui->healing_tableWidget->removeRow(row);
        emit deleteItem_signal(row);
    });


    connect(ui->clear_pushButton, &QPushButton::clicked, this, [this]() {
        emit clearTableWidget();
        ui->healing_tableWidget->setRowCount(0);
    });
}

HealingView::~HealingView() {
    delete ui;
}

void HealingView::addItem(const QString &action, const QString &heal, const QString &condition, const int &random,
    const int &below, const int &above, const int &minimum) {
    int row = ui->healing_tableWidget->rowCount();
    ui->healing_tableWidget->insertRow(row);
    QString actionString = action + " \'" + heal + "\'";
    QStringList values = {actionString, condition,QString::number(below), QString::number(above)};
    for(int col = 0; col < values.size(); ++col) {
        QTableWidgetItem *item = new QTableWidgetItem(values[col]);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->healing_tableWidget->setItem(row, col, item);
    }
}

void HealingView::clearTableWidget() {
    ui->healing_tableWidget->setRowCount(0);
}
