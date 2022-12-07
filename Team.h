//
// Created by ohadr on 11/26/2022.
//

#ifndef AVL_TREES_H_TEAM_H
#define AVL_TREES_H_TEAM_H
#include "Player.h"
#include "AVL_Map.h"
class Player;
class Team{
private:
    int m_teamId;
    AVLTree<Player*, int> m_playersByIdTree;
    AVLTree<Player*, Player> m_playersByGoals;
    int m_totalGames;
    int m_totalGoals;
    int m_totalCards;
    int m_totalGKs;
    int m_points;
    const Player* m_topScorer;

public:
    Team(int id, int initPoints);
    Team(int id, const AVLTree<Player*,int>& players,const AVLTree<Player*,Player>& scoreBoard, int totalGames, int totalGoals, int totalCards, int totalGKs, int points, const Player *topScorer);
    ~Team()=default;
    Team(const Team& t) = default; ///cannot copy a team, each team unique
    Team& operator=(const Team& t)=default;


    ///-------getters-------
    bool isTeamValid() const;
    bool empty() const;
    int teamValue() const;
    int getTeamId() const;
    int getPoints() const;
    int getNumOfGks() const;
    int getTotalGoals() const;
    int getTotalsCards() const;
    int getTeamGames() const;
    int getTeamSize() const;
    const Player* getTopScorer() const;
    const AVLTree<Player*, int>& getPlayersIdTree() const;
    const AVLTree<Player*, Player>& getScoreBoard() const;

    ///--------setters-------

    StatusType insertPlayer(Player* newPlayer, int playerId);
    StatusType removePlayer(int playerId);
    void updateTopScorer();
    void winGame();
    void loseGame();
    void tieGame();
    void addGk();
    void removeGk();
    void updatePlayerTeamTag();

    Player* findPlayer(int playerId);

    friend Team* mergeTeams(const Team& team1, const Team& team2, int uniteId);

    Pair<Player*, Player>** arrByGoals();
};
std::ostream operator<<(std::ostream out, const Team& team);


Team* mergeTeams(const Team& team1,const Team& team2, int uniteId);



#endif //AVL_TREES_H_TEAM_H