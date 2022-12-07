//
// Created by ohadr on 11/26/2022.
//

#ifndef AVL_TREES_H_TEAM_H
#define AVL_TREES_H_TEAM_H
#include "Player.h"
#include "AVL_Map.h"

class Team{
private:
    int m_teamId;
    AVLTree<Player, int> m_playersByIdTree;
//    AVLTree<Player, Player> m_playersByGoalsTree;
    int m_gamesPlayedAsTeam;
    int m_points;
    const Player* m_topScorer;
    int m_goalKeepers;
    int m_totalGoals;
    int m_totalCards;

public:
    Team(int id, int initPoints);
    Team(int id, int initPoints, const AVLTree<Player,int>& players, int GK, int totalGoals, int totalCards,
         const Player *topScorer);
    ~Team()=default;
    Team(const Team& t); ///cannot copy a team, each team unique
    Team& operator=(const Team& t);


    ///-------getters-------
    int teamValue() const;
    const int getTeamId() const;
    int getPoints() const;
    bool isTeamValid() const;
    int getNumPlayers() const;
    int getGK() const;
    const Player& getTopScorer() const;
    int getTotalGoals() const;
    int getTotalsCards() const;
    int getTeamGames() const;
    int getSize() const;
    bool empty() const;
    ///--------setters-------
    bool updateTopScorer(Player* messi);
    bool insertPlayer(const Player& newPlayer);
    bool removePlayer(int playerId);
    void winGame();
    void loseGame();
    void tieGame();
    void changeGK(bool isAdded);
    bool updateTeam(AVLTree<Player, int>& playerTree, int goals, int cards, int GK, Player* topScorer);

    Pair<Player, Player>* arrByGoals();

    const AVLTree<Player, int> &getPlayersByIdTree() const;
    const AVLTree<Player, Player> &getPlayersByGoalsTree() const;
};

int compareIdTeam(const Team& t1, const Team& t2); ///check if t1 id is greater than t2

Team mergeTeams(Team& team1, Team& team2, int uniteId);

std::ostream operator<<(std::ostream out, const Team& team);
#endif //AVL_TREES_H_TEAM_H