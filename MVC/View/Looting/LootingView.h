#ifndef LOOTINGVIEW_H
#define LOOTINGVIEW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class Looting_View; }
QT_END_NAMESPACE

class LootingView : public QMainWindow {
Q_OBJECT

public:
    explicit LootingView(QWidget *parent = nullptr);
    ~LootingView() override;

    void addItem(const QString &srcItem, const QString &dstItem, const QString &delay);
    void clearTableWidget();

    signals:
    void addItem_signal(const QString &srcItem, const QString &dstItem, const QString &delay);
    void deleteItem_signal(const int &index);
    void clearTableWidget_signal();


private:
    Ui::Looting_View *ui;
};


#endif //LOOTINGVIEW_H
