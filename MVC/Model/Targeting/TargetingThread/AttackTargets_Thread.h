#ifndef ATTACKCREATURES_THREAD_H
#define ATTACKCREATURES_THREAD_H
#include <Qthread>
#include "../../const.h"
#include "../../proto_functions_client.h"


class AttackTargets_Thread : public QThread {
    Q_OBJECT
    public:
    explicit AttackTargets_Thread(const std::vector<Target> &targets, bool reachableSate, bool shootableState, int stayAwayDist, const std::vector<Position> &blockedTiles,QObject *parent = nullptr)
        : QThread(parent), m_targets(targets), m_reachableState(reachableSate), m_shootableState(shootableState), m_stayAwayDistance(stayAwayDist), m_blockedTiles(blockedTiles) {}
public slots:
    void shootableStateChange(bool state);
    void reachableStateChange(bool state);
    void stayAwayDistChange(int currentDist);
    void updateData(std::vector<Target>);
protected:
    void run() override;
private:
    std::vector<Target> m_targets;
    bool m_shootableState = false;
    bool m_reachableState = false;
    int m_stayAwayDistance = 1;
    std::vector<Position> m_blockedTiles{};

    struct MonsterCandidate {
        int dist;
        uintptr_t id;
        Position truePos;
        Target target;
    };

    MonsterCandidate currentTarget{};


    void desiredStance(uintptr_t localPlayer, Position playerPos, Position spectatorPos);

    bool isReachable(Position playerPos, Position spectatorPos);
    bool isShootable(uintptr_t spectator, int dist);
};



#endif //ATTACKCREATURES_THREAD_H