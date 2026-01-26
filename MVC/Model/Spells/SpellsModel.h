#ifndef SPELLSMODEL_H
#define SPELLSMODEL_H
#include <QObject>
#include <QVariant>
#include "../../const.h"
#include "SpellsThread/UseSpell_Thread.h"

class SpellsModel : public QObject {
    Q_OBJECT
public:
    explicit SpellsModel(QObject *parent = nullptr);
    ~SpellsModel();

    void addItem(const QString &target, const int &option, const QString &spellName, const int &count, const int &dist, const int &minHp, const int &costMp, int priority, bool requiresTarget, bool playerProtection);
    void startSpells(bool state);

    QJsonArray toJson() const;
    void fromJson(const QJsonArray &json);

    void deleteItem(const int &index);
    void clearTableWidget();

    signals:
    void addItem_signal(const QString &target, const int &option, const QString &spellName, const int &count, const int &dist, const int &minHp, const int &costMp, int priority, bool requiresTarget, bool playerProtection);
    void clearTableWidget_signal();



private:
    std::vector<Spell> spells;
    UseSpell_Thread *useSpellThread = nullptr;

};



#endif //SPELLSMODEL_H
