#ifndef TARGETINGVIEW_H
#define TARGETINGVIEW_H
#include <QMainWindow>
#include "ui_TargetingView.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Targeting_View; }
QT_END_NAMESPACE

class TargetingView : public QMainWindow {
Q_OBJECT

public:
    explicit TargetingView(QWidget *parent = nullptr);
    ~TargetingView() override;

    void addItem(const QString &targetName, const QString &dist, const QString &count, const QString &desiredStance, bool openCorpse);
    void clearTableWidget();
    void addBlockedTile(const QString &tile);
    void clearBlockedTilesListWidget();

    signals:
    void addItem_signal(const QString &targetName, const int &dist, const int &count, const QString &desiredStance, bool openCorpse);
    void addBlockedTile_signal(const int &x, const int &y, const int &z);
    void clearTableWidget_signal();

    void shootableState_signal(bool state);
    void reachableState_signal(bool state);
    void stayAwayDist_signal(int distance);
    void deleteItem_signal(const int &index);
    void deleteBlockedTile_signal(const int &index);

private:
    Ui::Targeting_View *ui;
};


#endif //TARGETINGVIEW_H
