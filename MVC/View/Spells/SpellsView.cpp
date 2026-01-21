#include "SpellsView.h"


SpellsView::SpellsView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Spells_View) {

    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    ui->spells_tableWidget->setColumnCount(7);
    ui->spells_tableWidget->setHorizontalHeaderLabels({"Target", "Action", "Count", "Dist", "HP%", "Cost", "Priority"});
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);

    connect(ui->add_pushButton, &QPushButton::clicked, this,[this] {
        auto target = ui->targetName_lineEdit->text();
        auto option = ui->option_comboBox->currentIndex(); // Say or Use
        auto spellName = ui->spellName_lineEdit->text();
        auto count = ui->count_comboBox->currentIndex() + 1;
        auto dist = ui->dist_comboBox->currentIndex() + 1;
        auto minHp = ui->minHp_lineEdit->text().toInt();
        auto costMp = ui->costMp_lineEdit->text().toInt();
        auto priority = ui->priority_lineEdit->text().toInt();
        emit addItem_signal(target, option, spellName, count, dist, minHp, costMp, priority);

        ui->spellName_lineEdit->clear();
        ui->targetName_lineEdit->clear();
        ui->minHp_lineEdit->clear();
        ui->costMp_lineEdit->clear();
        ui->priority_lineEdit->clear();
        ui->count_comboBox->setCurrentIndex(0);
        ui->option_comboBox->setCurrentIndex(0);
        ui->dist_comboBox->setCurrentIndex(0);
    });

    connect(ui->spells_tableWidget, &QTableWidget::cellDoubleClicked, this, [this](int row, int column){
        ui->spells_tableWidget->removeRow(row);
        emit deleteItem_signal(row);
    });


    connect(ui->clear_pushButton, &QPushButton::clicked, this, [this]() {
        emit clearTableWidget();
        ui->spells_tableWidget->setRowCount(0);
    });

}

SpellsView::~SpellsView() {
    delete ui;
}

void SpellsView::addItem(const QString &target, const int &option, const QString &spellName, const int &count,
    const int &dist, const int &minHp, const int &costMp, int priority) {
    int row = ui->spells_tableWidget->rowCount();
    ui->spells_tableWidget->insertRow(row);
    QString action = option ? "Rune" : "Say";
    action += " \'" + spellName + "\'";
    QStringList values = {target, action, QString::number(count), QString::number(dist), QString::number(minHp), QString::number(costMp), QString::number(priority)};
    for(int col = 0; col < values.size(); ++col) {
        QTableWidgetItem *item = new QTableWidgetItem(values[col]);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->spells_tableWidget->setItem(row, col, item);
    }
}


void SpellsView::clearTableWidget() {
    ui->spells_tableWidget->setRowCount(0);
}
