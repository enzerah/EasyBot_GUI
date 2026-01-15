#include "MainBot/BotController.h"
#include <QStyleFactory>



int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    std::filesystem::create_directory("Save/");
    QApplication::setQuitOnLastWindowClosed(false);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    BotView w;
    BotController controller(&w);
    w.show();
    return QApplication::exec();
}
