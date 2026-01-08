#include "ScriptsView.h"

#include <iostream>
#include <QCheckBox>
#include <QTextEdit>
#include <QEvent>
#include "ScriptEditorDialog.h"

#include "Highlighter.h"
#include "ui_ScriptsView.h"


ScriptsView::ScriptsView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Scripts_View) {
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    ui->setupUi(this);


    ui->scripts_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->scripts_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    connect(ui->remove_pushButton, &QPushButton::clicked, this, [this]() {
        int row = ui->scripts_tableWidget->currentRow();
        if (row != -1) {
            ui->scripts_tableWidget->removeRow(row);
            emit deleteItem_signal(row);
        }
    });

    connect(ui->add_pushButton, &QPushButton::clicked, this, [this]() {
        int row = ui->scripts_tableWidget->rowCount();
        addItem(false, "New Script", "");
        emit addItem_signal(row, false, "New Script", "");
    });

    connect(ui->scripts_tableWidget, &QTableWidget::itemDoubleClicked, this, [this](QTableWidgetItem *item) {
        if (item->column() == 0 && (item->flags() & Qt::ItemIsEnabled)) {
            QString currentName = item->text();
            QString currentScript = item->data(Qt::UserRole).toString();

            ScriptEditorDialog dialog(currentName, currentScript, this);
            if (dialog.exec() == QDialog::Accepted) {
                item->setText(dialog.getName());
                item->setData(Qt::UserRole, dialog.getScript());

                int row = item->row();
                QWidget *cellWidget = ui->scripts_tableWidget->cellWidget(row, 1);
                if (cellWidget) {
                    QCheckBox *checkBox = cellWidget->findChild<QCheckBox *>();
                    bool isChecked = checkBox ? checkBox->isChecked() : false;
                    emit addItem_signal(row, isChecked, dialog.getName(), dialog.getScript());
                }
            }
        }
    });
}

void ScriptsView::addItem(bool state, const QString &name, const QString &script_text) {
    int row = ui->scripts_tableWidget->rowCount();
    ui->scripts_tableWidget->insertRow(row);

    auto *nameItem = new QTableWidgetItem(name);
    nameItem->setFlags(nameItem->flags() ^ Qt::ItemIsEditable);
    nameItem->setData(Qt::UserRole, script_text);
    ui->scripts_tableWidget->setItem(row, 0, nameItem);

    auto *checkBox = new QCheckBox();
    checkBox->setChecked(state);

    connect(checkBox, &QCheckBox::toggled, this, [this, checkBox](bool checked) {
        QWidget *checkBoxWidget = checkBox->parentWidget();
        int currentRow = -1;
        for (int i = 0; i < ui->scripts_tableWidget->rowCount(); ++i) {
            if (ui->scripts_tableWidget->cellWidget(i, 1) == checkBoxWidget) {
                currentRow = i;
                break;
            }
        }

        if (currentRow == -1) return;

        QTableWidgetItem *nameItem = ui->scripts_tableWidget->item(currentRow, 0);
        QString name = nameItem->text();
        QString scriptText = nameItem->data(Qt::UserRole).toString();

        if (checked) {
            nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEnabled);
            emit addItem_signal(currentRow, checked, name, scriptText);
        } else {
            nameItem->setFlags(nameItem->flags() | Qt::ItemIsEnabled);
            emit addItem_signal(currentRow, checked, name, scriptText);
        }
    });

    QWidget *checkBoxWidget = new QWidget(ui->scripts_tableWidget);
    QHBoxLayout *layout = new QHBoxLayout(checkBoxWidget);
    layout->addWidget(checkBox);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    ui->scripts_tableWidget->setCellWidget(row, 1, checkBoxWidget);

    // Initial state handling
    if (state) {
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEnabled);
    }
}

void ScriptsView::clearTableWidget() {
    ui->scripts_tableWidget->setRowCount(0);
}

ScriptsView::~ScriptsView() {
    delete ui;
}

bool ScriptsView::eventFilter(QObject *obj, QEvent *event) {
    return QMainWindow::eventFilter(obj, event);
}

