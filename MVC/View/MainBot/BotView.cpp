#include "BotView.h"



#include <QMenu>

#include "MainBot/BotController.h"

BotView::BotView(QWidget *parent)
    : ui(new Ui::Bot_View) {

    ui->setupUi(this);
    setWindowTitle(" ");
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    connect(ui->walker_pushButton, &QPushButton::clicked, this, &BotView::openWalkerView);
    connect(ui->targeting_pushButton, &QPushButton::clicked, this, &BotView::openTargetingViewRequested);
    connect(ui->looting_pushButton, &QPushButton::clicked, this, &BotView::openLootingViewRequested);
    connect(ui->healing_pushButton, &QPushButton::clicked, this, &BotView::openHealingViewRequested);
    connect(ui->spells_pushButton, &QPushButton::clicked, this, &BotView::openSpellsViewRequested);
    connect(ui->scripts_pushButton, &QPushButton::clicked, this, &BotView::openScriptsViewRequested);
    connect(ui->ai_pushButton, &QPushButton::clicked, this, &BotView::openAgentAIViewRequested);
    connect(ui->alarms_pushButton, &QPushButton::clicked, this, &BotView::openAlarmsViewRequested);
    connect(ui->miscellaneous_pushButton, &QPushButton::clicked, this, &BotView::openMiscellaneousViewRequested);
    connect(ui->navigation_pushButton, &QPushButton::clicked, this, &BotView::openNavigationViewRequested);


    connect(ui->all_checkBox, &QCheckBox::toggled, this, &BotView::startAll_signal);
    connect(ui->walker_checkBox, &QCheckBox::toggled, this, &BotView::startWalker_signal);
    connect(ui->targeting_checkBox, &QCheckBox::toggled, this, &BotView::startTargeting_signal);
    connect(ui->looting_checkBox, &QCheckBox::toggled, this, &BotView::startLooting_signal);
    connect(ui->healing_checkBox, &QCheckBox::toggled, this, &BotView::startHealing_signal);
    connect(ui->spells_checkBox, &QCheckBox::toggled, this, &BotView::startSpells_signal);
    connect(ui->scripts_checkBox, &QCheckBox::toggled, this, &BotView::startScripts_signal);
    connect(ui->ai_checkBox, &QCheckBox::toggled, this, &BotView::startAgent_signal);

    connect(ui->save_pushButton, &QPushButton::clicked, this, &BotView::onSaveClicked);
    connect(ui->load_pushButton, &QPushButton::clicked, this, &BotView::onLoadClicked);

    for (const auto& entry : std::filesystem::directory_iterator("Save")) {
        const std::filesystem::path& p = entry.path();
        if (p.extension() == ".json") {
            ui->profiles_listWidget->addItem(p.stem().string().c_str());
        }
    }

    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        m_tray = new QSystemTrayIcon(this);
        m_tray->setIcon(QIcon(":/Icon.jpg"));

        m_trayMenu = new QMenu(this);
        m_restoreAction = m_trayMenu->addAction(tr("Restore"));
        m_quitAction = m_trayMenu->addAction(tr("Exit"));

        connect(m_restoreAction, &QAction::triggered, this, &BotView::restoreFromTray);
        connect(m_quitAction, &QAction::triggered, this, &BotView::quitApp);
        connect(m_tray, &QSystemTrayIcon::activated, this, &BotView::iconActivated);

        connect(ui->actionAbout_QT, &QAction::triggered, this, &BotView::showAboutQt);

        m_tray->setContextMenu(m_trayMenu);
        m_tray->show();
    }

}

BotView::~BotView() {
    delete ui;
}


void BotView::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            restoreFromTray();
            break;
        default:
            break;
    }
}

void BotView::restoreFromTray() {
    show();
    raise();
    activateWindow();
}

void BotView::quitApp() {
    if (m_tray) {
        m_tray->hide();
    }
    qApp->quit();
}

void BotView::onSaveClicked() {
    emit saveRequested(ui->name_lineEdit->text(), ui->saveLoad_comboBox->currentText());
    ui->profiles_listWidget->addItem(ui->name_lineEdit->text());
    ui->name_lineEdit->clear();
}

void BotView::onLoadClicked() {
    if (ui->profiles_listWidget->currentItem()) {
        emit loadRequested(ui->profiles_listWidget->currentItem()->text(), ui->saveLoad_comboBox->currentText());
    }
}

void BotView::showAboutQt() {
    QApplication::aboutQt();
}

void BotView::setWalkerChecked(bool state) {
    ui->walker_checkBox->setChecked(state);
}

void BotView::setTargetingChecked(bool state) {
    ui->targeting_checkBox->setChecked(state);
}

void BotView::setLootingChecked(bool state) {
    ui->looting_checkBox->setChecked(state);
}

void BotView::setHealingChecked(bool state) {
    ui->healing_checkBox->setChecked(state);
}

void BotView::setSpellsChecked(bool state) {
    ui->spells_checkBox->setChecked(state);
}

void BotView::setScriptsChecked(bool state) {
    ui->scripts_checkBox->setChecked(state);
}

void BotView::setAgentAIChecked(bool state) {
    ui->ai_checkBox->setChecked(state);
}




