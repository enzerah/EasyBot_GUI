#include "LootingView.h"
#include "ui_LootingView.h"


LootingView::LootingView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Looting_View) {

    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    ui->loot_tableWidget->setColumnCount(3);
    ui->loot_tableWidget->setHorizontalHeaderLabels({"ItemID", "Destination", "Delay"});
    ui->loot_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->loot_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->loot_tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);


    connect(ui->add_pushButton, &QPushButton::clicked, this, [this]() {
        auto itemID = ui->source_lineEdit->text();
        auto destination = ui->dest_lineEdit->text();
        auto delay = ui->speedlineEdit->text();
        emit addItem_signal(itemID, destination, delay);
        ui->source_lineEdit->clear();
        ui->dest_lineEdit->clear();
        ui->speedlineEdit->clear();
    });

    connect(ui->loot_tableWidget, &QTableWidget::cellDoubleClicked, this, [this](int row, int column){
        ui->loot_tableWidget->removeRow(row);
        emit deleteItem_signal(row);
    });


    connect(ui->clear_pushButton, &QPushButton::clicked, this, [this]() {
        emit clearTableWidget();
        ui->loot_tableWidget->setRowCount(0);
    });
}

LootingView::~LootingView() {
    delete ui;
}

void LootingView::addItem(const QString &srcItem, const QString &dstItem, const QString &delay) {
    int row = ui->loot_tableWidget->rowCount();
    ui->loot_tableWidget->insertRow(row);
    QStringList values = {srcItem, dstItem, delay};
    for(int col = 0; col < values.size(); ++col) {
        QTableWidgetItem *item = new QTableWidgetItem(values[col]);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->loot_tableWidget->setItem(row, col, item);
    }
}

void LootingView::clearTableWidget() {
    ui->loot_tableWidget->setRowCount(0);
}
