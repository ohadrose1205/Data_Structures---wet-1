//
// Created by ohadr on 11/26/2022.
//

#ifndef AVL_TREES_H_TEAM_H
#define AVL_TREES_H_TEAM_H
#include "Player.h"
#include "AVL_trees.h"

class Team{
private:
    int m_teamId;
    AVLTree<Player, int> m_playersTree;
    AVLTree<Player, Player> m_playersByGoalsTree;
    int m_gamesPlayedAsTeam;
    int m_points;
    int m_playersNum;
    Player* m_topScorer;
    int m_goalKeepers;
    int m_totalGoals;
    int m_totalCards;

public:
    Team(int id, int initPoints);
    Team(int id, int initPoints, AVLTree<Player, int>& players, int GK, int totalGoals, int totalCards, Player* topScorer);
    ~Team(); ///Should be default? because AVL destroyer
    Team(const Team& t) =default; ///cannot copy a team, each team unique
    Team& operator=(const Team& t);

    ///-------getters-------
    const int teamValue();
    const AVLTree<Player, int>* getPlayersTree() const;
    const int getTeamId() const;
    const int getPoints() const;
    const int getNumPlayers();
    const bool isTeamValid();
    const int getGK() const;
    Player* getTopScorer() const;
    const int getTotalGoals() const;
    const int getTotalsCards() const;
    const int getTeamGames() const;

    ///--------setters-------
    bool compareIdTeam(const Team& t1, const Team& t2) const; ///check if t1 id is greater than t2
    AVLStatus updateTopScorer(Player* messi);
    AVLStatus insertPlayer(Player* newPlayer);
    AVLStatus removePlayer(int playerId);
    AVLStatus playGame(char victoryFlag);
    void changeGK(bool isAdded);
    AVLStatus updateTeam(AVLTree<Player, int>& playerTree, int goals, int cards, int GK, Player* topScorer);

    Pair<Player, Player>* arrByGoals();
};

std::ostream operator<<(std::ostream out, const Team& team);
#endif //AVL_TREES_H_TEAM_H
