//
// Created by ohadr on 11/26/2022.
//

#ifndef AVL_TREES_H_TEAM_H
#define AVL_TREES_H_TEAM_H
#include "Player.h"
#include "AVL_trees.h"

class Team{
private:
    int teamId;
    AVLTree<Player> playersTree;
    int gamesPlayedAsTeam;
    int points;
    int playersNum;
    Player* topScorer;
    bool gkInclude;
public:
    Team(int id, int initPoints);
    ~Team(); ///Should be default? because AVL destroyer
    Team(const Team& t) =delete; ///cannot copy a team, each team unique
    Team& operator=(const Team& t);

    const int teamValue();
    const int getTeamId() const;
    const bool isTeamValid();
    const int goalsCount();
    const int cardCount();

    Team* compareIdTeam(const Team& t1, const Team& t2) const; ///check if t1 id is greater than t2
    AVLStatus updateTopScorer();
};

#endif //AVL_TREES_H_TEAM_H
