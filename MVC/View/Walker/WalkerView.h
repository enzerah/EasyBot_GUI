#ifndef WALKERVIEW_H
#define WALKERVIEW_H

#include "ui_WalkerView.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Walker_View; }
QT_END_NAMESPACE

class WalkerView : public QMainWindow {
Q_OBJECT

public:
    explicit WalkerView(QWidget *parent = nullptr);
    ~WalkerView() override;

    void addItem(const QString &option, const QString &direction, uint32_t x, uint32_t y, uint16_t z);
    void indexUpdate(int index);
    void clearTableWidget();

signals:
    void addItem_signal(const QString &direction, const QString &option, const QString &action);
    void recordWaypoints_signal(bool state, int sqmDist, const QString &direction, const QString &option);
    void deleteItem_signal(const int &index);
    void clearListWidget_signal();

private:
    Ui::Walker_View *ui;
};


#endif //WALKERVIEW_H
