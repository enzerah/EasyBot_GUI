#include "WalkerView.h"
#include <iostream>
#include <QButtonGroup>


WalkerView::WalkerView(QWidget *parent)
    : ui(new Ui::Walker_View) {

    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    ui->waypoints_tableWidget->setColumnCount(5);
    ui->waypoints_tableWidget->setHorizontalHeaderLabels({"Option", "Dir", "X", "Y", "Z"});
    ui->waypoints_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->waypoints_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->waypoints_tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->waypoints_tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->waypoints_tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    auto direction_buttonGroup = new QButtonGroup();
    direction_buttonGroup->addButton(ui->n_radioButton, 0);
    direction_buttonGroup->addButton(ui->e_radioButton, 1);
    direction_buttonGroup->addButton(ui->s_radioButton, 2);
    direction_buttonGroup->addButton(ui->w_radioButton, 3);
    direction_buttonGroup->addButton(ui->ne_radioButton, 4);
    direction_buttonGroup->addButton(ui->se_radioButton, 5);
    direction_buttonGroup->addButton(ui->sw_radioButton, 6);
    direction_buttonGroup->addButton(ui->nw_radioButton, 7);
    direction_buttonGroup->addButton(ui->c_radioButton, 8);

    auto option_buttonGroup = new QButtonGroup();
    option_buttonGroup->addButton(ui->stand_radioButton);
    option_buttonGroup->addButton(ui->node_radioButton );
    option_buttonGroup->addButton(ui->lure_radioButton);
    option_buttonGroup->addButton(ui->label_radioButton);
    option_buttonGroup->addButton(ui->action_radioButton);
    option_buttonGroup->addButton(ui->use_radioButton);

    connect(ui->add_pushButton, &QPushButton::clicked, this, [this, direction_buttonGroup, option_buttonGroup]() {
        auto direction = direction_buttonGroup->checkedButton()->text();
        auto option = option_buttonGroup->checkedButton()->text();
        auto action = ui->action_textEdit->toPlainText().trimmed();
        emit addItem_signal(direction, option, action);
        ui->action_textEdit->clear();
    });

    connect(option_buttonGroup, &QButtonGroup::buttonClicked, this, [this, option_buttonGroup]() {
        auto option = option_buttonGroup->checkedButton()->text().trimmed().toStdString();
        bool enable = (option == "Action" || option == "Label" || option == "Use");
        ui->action_textEdit->setEnabled(enable);
    });

    connect(ui->record_checkBox, &QCheckBox::toggled, this, [this, direction_buttonGroup, option_buttonGroup]() {
        auto state = ui->record_checkBox->isChecked();
        auto sqmDist = ui->record_lineEdit->text().toInt();
        auto direction = direction_buttonGroup->checkedButton()->text();
        auto option = option_buttonGroup->checkedButton()->text();
        emit recordWaypoints_signal(state, sqmDist, direction, option);
    });

    connect(ui->waypoints_tableWidget, &QTableWidget::cellDoubleClicked, this, [this](int row, int column){
        ui->waypoints_tableWidget->removeRow(row);
        emit deleteItem_signal(row);
    });

    connect(ui->clear_pushButton, &QPushButton::clicked, this, [this]() {
        emit clearListWidget_signal();
        ui->waypoints_tableWidget->setRowCount(0);
    });
}

WalkerView::~WalkerView() {
    delete ui;
}

void WalkerView::addItem(const QString &option, const QString &direction, uint32_t x, uint32_t y, uint16_t z) {
    int row = ui->waypoints_tableWidget->rowCount();
    ui->waypoints_tableWidget->insertRow(row);
    QStringList values = {option, direction, QString::number(x), QString::number(y), QString::number(z)};
    for(int col = 0; col < values.size(); ++col) {
        QTableWidgetItem *item = new QTableWidgetItem(values[col]);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->waypoints_tableWidget->setItem(row, col, item);
    }
}

void WalkerView::indexUpdate(int index) {
    ui->waypoints_tableWidget->selectRow(index);
    ui->waypoints_tableWidget->setFocus();
}

void WalkerView::clearTableWidget() {
    ui->waypoints_tableWidget->setRowCount(0);
}
