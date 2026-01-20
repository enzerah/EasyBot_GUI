#include "SpellsView.h"


SpellsView::SpellsView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Spells_View) {

    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    ui->spells_tableWidget->setColumnCount(6);
    ui->spells_tableWidget->setHorizontalHeaderLabels({"Target", "Action", "Count", "Dist", "HP%", "Cost"});
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);

    connect(ui->add_pushButton, &QPushButton::clicked, this,[this] {
        auto target = ui->targetName_lineEdit->text();
        auto option = ui->option_comboBox->currentIndex(); // Say or Use
        auto spellName = ui->spellName_lineEdit->text();
        auto count = ui->count_comboBox->currentIndex() + 1;
        auto dist = ui->dist_comboBox->currentIndex() + 1;
        auto minHp = ui->minHp_lineEdit->text().toInt();
        auto costMp = ui->costMp_lineEdit->text().toInt();
        bool requiresTarget = ui->requiresTarget_checkBox->isChecked();
        emit addItem_signal(target, option, spellName, count, dist, minHp, costMp, requiresTarget);

        ui->spellName_lineEdit->clear();
        ui->targetName_lineEdit->clear();
        ui->minHp_lineEdit->clear();
        ui->costMp_lineEdit->clear();
        ui->count_comboBox->setCurrentIndex(0);
        ui->option_comboBox->setCurrentIndex(0);
        ui->dist_comboBox->setCurrentIndex(0);
        ui->requiresTarget_checkBox->setChecked(false);
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
    const int &dist, const int &minHp, const int &costMp, bool requiresTarget) {
    int row = ui->spells_tableWidget->rowCount();
    ui->spells_tableWidget->insertRow(row);
    QString action = option ? "Rune" : "Say";
    action += " \'" + spellName + "\'";
    QStringList values = {target, action, QString::number(count), QString::number(dist), QString::number(minHp), QString::number(costMp)};
    for(int col = 0; col < values.size(); ++col) {
        QTableWidgetItem *item = new QTableWidgetItem(values[col]);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->spells_tableWidget->setItem(row, col, item);
    }
}


void SpellsView::clearTableWidget() {
    ui->spells_tableWidget->setRowCount(0);
}
