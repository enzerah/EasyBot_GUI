#include "NavigationView.h"
#include "ui_NavigationView.h"
#include <QTableWidgetItem>
#include <QHeaderView>


NavigationView::NavigationView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Navigation_View) {
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    ui->setupUi(this);

    ui->navigation_tableWidget->setColumnCount(2);
    ui->navigation_tableWidget->setHorizontalHeaderLabels({"Client", "Status"});
    ui->navigation_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->navigation_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);


    connect(ui->refresh_pushButton, &QPushButton::clicked, this, [this]() {
        ui->navigation_tableWidget->setRowCount(0);
        emit refreshSevers_signal();
    });
}




NavigationView::~NavigationView() {
    delete ui;
}

void NavigationView::addItem(const QString &clientName, bool status) {
    int row = ui->navigation_tableWidget->rowCount();
    ui->navigation_tableWidget->insertRow(row);
    ui->navigation_tableWidget->setItem(row, 0, new QTableWidgetItem(clientName));
    QString statusText = status ? "Connected" : "Disconnected";
    QTableWidgetItem *statusItem = new QTableWidgetItem(statusText);
    if (status) {
        statusItem->setForeground(Qt::green);
    } else {
        statusItem->setForeground(Qt::red);
    }
    QFont font = statusItem->font();
    font.setBold(true);
    statusItem->setFont(font);
    ui->navigation_tableWidget->setItem(row, 1, statusItem);
}
