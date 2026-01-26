#ifndef SPELLSVIEW_H
#define SPELLSVIEW_H

#include <QMainWindow>
#include "ui_SpellsView.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Spells_View; }
QT_END_NAMESPACE

class SpellsView : public QMainWindow {
Q_OBJECT

public:
    explicit SpellsView(QWidget *parent = nullptr);
    ~SpellsView() override;

    void addItem(const QString &target, const int &option, const QString &spellName, const int &count, const int &dist, const int &minHp, const int &costMp, int priority, bool requiresTarget, bool playerProtection);
    void clearTableWidget();

    signals:
    void addItem_signal(const QString &target, const int &option, const QString &spellName, const int &count, const int &dist, const int &minHp, const int &costMp, int priority, bool requiresTarget, bool playerProtection);
    void deleteItem_signal(const int &index);
    void clearTableWidget_signal();

private:
    Ui::Spells_View *ui;
};


#endif //SPELLSVIEW_H
