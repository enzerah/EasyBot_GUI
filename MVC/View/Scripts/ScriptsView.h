#ifndef SCRIPTSVIEW_H
#define SCRIPTSVIEW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class Scripts_View; }
QT_END_NAMESPACE

#include <vector>
#include "Scripts/ScriptsModel.h"

class ScriptsView : public QMainWindow {
Q_OBJECT

public:
    explicit ScriptsView(QWidget *parent = nullptr);
    ~ScriptsView() override;

    void addItem(bool state, const QString &name, const QString &script_text);
    void clearTableWidget();

    signals:
    void addItem_signal(const int &id, bool state, const QString &name, const QString &script_text);
    void deleteItem_signal(const int &index);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::Scripts_View *ui;
};


#endif //SCRIPTSVIEW_H
