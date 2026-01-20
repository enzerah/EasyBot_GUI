#ifndef SPELLSCONTROLLER_H
#define SPELLSCONTROLLER_H
#include <QObject>
#include "Spells/SpellsModel.h"
#include "Spells/SpellsView.h"
#include <QJsonArray>


class SpellsController : public QObject {
    Q_OBJECT
public:
    explicit SpellsController(QObject *parent = nullptr);
    ~SpellsController();

    void showView();

    QJsonArray saveSettings() const;
    void loadSettings(const QJsonArray &json);



public slots:
    void startSpells_slot(bool state);

private:
    SpellsView *m_view;
    SpellsModel *m_model;
};


#endif //SPELLSCONTROLLER_H
