#ifndef SCRIPTSVIEW_H
#define SCRIPTSVIEW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class Scripts_View; }
QT_END_NAMESPACE

class ScriptsView : public QMainWindow {
Q_OBJECT

public:
    explicit ScriptsView(QWidget *parent = nullptr);
    ~ScriptsView() override;

private:
    Ui::Scripts_View *ui;
};


#endif //SCRIPTSVIEW_H
