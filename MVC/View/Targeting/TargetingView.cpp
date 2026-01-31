#include "TargetingView.h"
TargetingView::TargetingView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Targeting_View) {

    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    ui->targets_tableWidget->setColumnCount(5);
    ui->targets_tableWidget->setHorizontalHeaderLabels({"Name", "Dist", "Count", "Stance", "Open"});
    ui->targets_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->targets_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->targets_tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->targets_tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->targets_tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    connect(ui->add_pushButton, &QPushButton::clicked, this, [this]() {
        QString targetName = ui->name_lineEdit->text();
        int dist = ui->dist_comboBox->currentIndex();
        int count = ui->count_comboBox->currentIndex() + 1;
        QString desiredStance = ui->stance_comboBox->currentText();
        bool openCorpse = ui->open_checkBox->isChecked();
        emit addItem_signal(targetName, dist, count, desiredStance, openCorpse);
        ui->name_lineEdit->clear();
        ui->dist_comboBox->setCurrentIndex(0);
        ui->stance_comboBox->setCurrentIndex(0);
        ui->count_comboBox->setCurrentIndex(0);
        ui->open_checkBox->setChecked(false);
    });

    connect(ui->shootable_checkBox, &QCheckBox::toggled, this, &TargetingView::shootableState_signal);
    connect(ui->reachable_checkBox, &QCheckBox::toggled, this, &TargetingView::reachableState_signal);
    connect(ui->dist_horizontalSlider, &QSlider::valueChanged, this, &TargetingView::stayAwayDist_signal);

    connect(ui->targets_tableWidget, &QTableWidget::cellDoubleClicked, this, [this](int row, int column){
        ui->targets_tableWidget->removeRow(row);
        emit deleteItem_signal(row);
    });

    connect(ui->clear_pushButton, &QPushButton::clicked, this, [this]() {
       emit clearTableWidget_signal();
        ui->targets_tableWidget->setRowCount(0);
    });

    connect(ui->addTile_pushButton, &QPushButton::clicked, this, [this]() {
        emit addBlockedTile_signal(ui->x_lineEdit->text().toInt(), ui->y_lineEdit->text().toInt(), ui->z_lineEdit->text().toInt());
        ui->x_lineEdit->clear();
        ui->y_lineEdit->clear();
        ui->z_lineEdit->clear();
    });

    connect(ui->blockedTiles_listWidget, &QListWidget::itemDoubleClicked, this, [this](){
        auto currentIndex = ui->blockedTiles_listWidget->currentRow();
        ui->blockedTiles_listWidget->takeItem(currentIndex);
        emit deleteBlockedTile_signal(currentIndex);
    });
}

TargetingView::~TargetingView() {
    delete ui;
}

void TargetingView::addItem(const QString &targetName, const QString &dist, const QString &count, const QString &desiredStance, bool openCorpse) {
    int row = ui->targets_tableWidget->rowCount();
    ui->targets_tableWidget->insertRow(row);
    QString open = openCorpse ? "Yes" : "No";
    QStringList values = {targetName, dist, count, desiredStance, open};
    for(int col = 0; col < values.size(); ++col) {
        QTableWidgetItem *item = new QTableWidgetItem(values[col]);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->targets_tableWidget->setItem(row, col, item);
    }
}

void TargetingView::clearTableWidget() {
    ui->targets_tableWidget->setRowCount(0);
}

void TargetingView::addBlockedTile(const QString &tile) {
    ui->blockedTiles_listWidget->addItem(tile);
}

void TargetingView::clearBlockedTilesListWidget() {
    ui->blockedTiles_listWidget->clear();
}
