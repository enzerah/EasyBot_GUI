//
// Created by blazz on 12.10.2025.
//

#ifndef TARGETINGMODEL_H
#define TARGETINGMODEL_H
#include <QObject>
#include "../../const.h"
#include "TargetingThread/AttackTargets_Thread.h"
#include <QJsonObject>
#include <QJsonArray>


class TargetingModel : public QObject {
    Q_OBJECT
    public:
    explicit TargetingModel(QObject *parent = nullptr);
    ~TargetingModel();

    void addItem(const QString &targetName, const int &dist, const int &count, const QString &desiredStance, const QString &monstersAttacks);
    void shootableState(bool state);
    void reachableState(bool state);
    void openCorpseState(bool state);
    void stayAwayDist(int currentDist);
    void startTargeting(bool state);
    void addBlockedTile(const int &x, const int &y, const int &z);
    void deleteBlockedTile(const int &index);

    QJsonArray toJson() const;
    void fromJson(const QJsonArray &json);

    void deleteItem(const int &index);
    void clearListWidget();



    signals:
    void addItem_signal(const QString &targetName, const QString &dist, const QString &count, const QString &desiredStance, const QString &monstersAttacks);
    void shootableStateChanged_signal(bool state);
    void reachableStateChanged_signal(bool state);
    void openCorpseStateChanged_signal(bool state);
    void stayAwayDistChanged_signal(int currentDist);
    void clearListWidget_signal();
    void addBlockedTile_signal(const QString &tile);
    void clearBlockedTilesListWidget_signal();

private:
    std::vector<Target> targets;
    std::vector<Position> blockedTiles;
    AttackTargets_Thread *attackTargetsThread = nullptr;
    bool m_shootable = false;
    bool m_reachable = false;
    bool m_openCorpse = false;
    int m_stayAwayDist = 2;

};



#endif //TARGETINGMODEL_H
