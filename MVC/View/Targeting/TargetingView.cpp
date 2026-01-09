#include "TargetingView.h"


TargetingView::TargetingView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Targeting_View) {

    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    connect(ui->add_pushButton, &QPushButton::clicked, this, [this]() {
        QString targetName = ui->name_lineEdit->text();
        int dist = ui->dist_comboBox->currentIndex();
        int count = ui->count_comboBox->currentIndex() + 1;
        QString desiredStance = ui->stance_comboBox->currentText();
        QString monstersAttacks = ui->monsterAttacks_comboBox->currentText();
        emit addItem_signal(targetName, dist, count, desiredStance, monstersAttacks);
        ui->name_lineEdit->clear();
        ui->dist_comboBox->setCurrentIndex(0);
        ui->stance_comboBox->setCurrentIndex(0);
        ui->count_comboBox->setCurrentIndex(0);
        ui->monsterAttacks_comboBox->setCurrentIndex(0);
    });

    connect(ui->shootable_checkBox, &QCheckBox::toggled, this, &TargetingView::shootableState_signal);
    connect(ui->reachable_checkBox, &QCheckBox::toggled, this, &TargetingView::reachableState_signal);
    connect(ui->open_checkBox, &QCheckBox::toggled, this, &TargetingView::openCorpseState_signal);
    connect(ui->dist_horizontalSlider, &QSlider::valueChanged, this, &TargetingView::stayAwayDist_signal);

    connect(ui->targets_listWidget, &QListWidget::itemDoubleClicked, this, [this](){
        auto currentIndex = ui->targets_listWidget->currentRow();
        ui->targets_listWidget->takeItem(currentIndex);
        emit deleteItem_signal(currentIndex);
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

void TargetingView::addItem(const QString &item) {
    ui->targets_listWidget->addItem(item);
}

void TargetingView::clearListWidget() {
    ui->targets_listWidget->clear();
}

void TargetingView::addBlockedTile(const QString &tile) {
    ui->blockedTiles_listWidget->addItem(tile);
}

void TargetingView::clearBlockedTilesListWidget() {
    ui->blockedTiles_listWidget->clear();
}
