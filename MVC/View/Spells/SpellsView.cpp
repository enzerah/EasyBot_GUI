#include "SpellsView.h"


SpellsView::SpellsView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Spells_View) {

    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    ui->spells_tableWidget->setColumnCount(4);
    ui->spells_tableWidget->setHorizontalHeaderLabels({"Target", "Action", "Count", "Dist"});
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->spells_tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    connect(ui->add_pushButton, &QPushButton::clicked, this,[this] {
        auto target_name = ui->name_lineEdit->text();
        auto dist = ui->dist_comboBox->currentIndex();
        auto count = ui->count_comboBox->currentIndex() + 1;
        auto spell = ui->spell_comboBox->currentText();
        auto spell_name = ui->spell_lineEdit->text();
        auto type = ui->type_comboBox->currentIndex();
        auto minHp = ui->playerHp_lineEdit->text().toInt();
        auto minMp = ui->playerMp_lineEdit->text().toInt();

        emit addItem_signal(target_name, dist, count, spell, spell_name,type,minHp, minMp);

        ui->name_lineEdit->clear();
        ui->dist_comboBox->setCurrentIndex(0);
        ui->count_comboBox->setCurrentIndex(0);
        ui->spell_comboBox->setCurrentIndex(0);
        ui->spell_lineEdit->clear();
        ui->type_comboBox->setCurrentIndex(0);
        ui->playerHp_lineEdit->clear();
        ui->playerMp_lineEdit->clear();
    });
    connect(ui->spells_listWidget, &QListWidget::itemDoubleClicked, this, [this](){
        auto currentIndex = ui->spells_listWidget->currentRow();
        ui->spells_listWidget->takeItem(currentIndex);
        emit deleteItem_signal(currentIndex);
    });

    connect(ui->clear_pushButton, &QPushButton::clicked, this, [this]() {
        emit clearListWidget();
    });
}

SpellsView::~SpellsView() {
    delete ui;
}

void SpellsView::addItem(const QString& item) {
    ui->spells_listWidget->addItem(item);
}

void SpellsView::clearListWidget() {
    ui->spells_listWidget->clear();
}
