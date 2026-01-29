#include "MainBot/BotController.h"
#include <QStyleFactory>
#include <QDir>
#include <QProcess>
#include <QRandomGenerator>
#include <QFileInfo>
#include <Windows.h>



int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QString appPath = QCoreApplication::applicationFilePath();
    QFileInfo appInfo(appPath);
    QString appName = appInfo.fileName();

    if (appName.compare("EasyBot.exe", Qt::CaseInsensitive) == 0) {
        QDir dir = appInfo.absoluteDir();
        QStringList filters;
        filters << "[0-9][0-9][0-9][0-9][0-9].exe";
        dir.setNameFilters(filters);
        QStringList oldFiles = dir.entryList(QDir::Files | QDir::Hidden);
        for (const QString& file : oldFiles) {
            dir.remove(file);
        }

        // 2. Generate new random 5-digit name
        int randomId = QRandomGenerator::global()->bounded(10000, 99999);
        QString newName = QString::number(randomId) + ".exe";
        QString newPath = dir.absoluteFilePath(newName);

        // 3. Copy self to new path
        if (QFile::copy(appPath, newPath)) {
            // Hide the new file so it doesn't clutter the view
            SetFileAttributesW(newPath.toStdWString().c_str(), FILE_ATTRIBUTE_HIDDEN);

            // 4. Launch the new executable
            // We pass the same arguments if needed
            if (QProcess::startDetached(newPath, QCoreApplication::arguments().mid(1))) {
                return 0; // Exit this process
            }
        }
    }
    // ---------------------------------------------------------
    std::filesystem::create_directory("Save/");
    QApplication::setQuitOnLastWindowClosed(false);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::WindowText, Qt::black);
    lightPalette.setColor(QPalette::Base, Qt::white);
    lightPalette.setColor(QPalette::AlternateBase, QColor(233, 233, 233));
    lightPalette.setColor(QPalette::ToolTipBase, Qt::white);
    lightPalette.setColor(QPalette::ToolTipText, Qt::black);
    lightPalette.setColor(QPalette::Text, Qt::black);
    lightPalette.setColor(QPalette::Button, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::ButtonText, Qt::black);
    lightPalette.setColor(QPalette::BrightText, Qt::red);
    lightPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    lightPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    lightPalette.setColor(QPalette::HighlightedText, Qt::white);
    QApplication::setPalette(lightPalette);

    BotView w;
    BotController controller(&w);
    w.show();
    return QApplication::exec();
}
