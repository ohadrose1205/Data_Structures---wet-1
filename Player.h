//
// Created by shach on 11/22/2022.
//

#ifndef MIVNEWET1_PLAYER_H
#define MIVNEWET1_PLAYER_H

#include "AVL_Map.h"
#include "Team.h"
class Team;
class Player {
public:
    Player(const int ID, Team* team, int gamesPlayed, int goals, int cards, const bool gkFlag);
    ~Player() =default;
    Player(const Player& p) =default;
    Player& operator=(const Player& p) = default;

    ///-----getters--------
    int getPlayerId() const;
    Team* getTeam() const;
    int getGoals() const;
    int getCards() const;
    int getIndividualGames() const; //games that player played AS AN UPDATE and not in the team
    bool isGk() const;
    Player *getClosest() const;
    Player* getClosestAbove() const;
    Player* getClosestBelow() const;

    ///------setters-------
    void updatePlayer(int gamesPlayed, int goals, int cards);
//    void setExtraGoals(int goals);
//    void setExtraCards(int cards);
//    void setExtraGames(int games);
    void setTeam(Team *pTeam);
    void setClosestAbove(Player* closest);
    void setClosestBelow(Player* closest);
    void updateGamesPlayed();

    bool compareIdPlayers(const Player& p1, const Player& p2) const; ///check if p1 id is greater than p2
    int compareGoals(const Player& p1, const Player& p2) const; ///check if p1 goals is greater than p2
    int compareCards(const Player& p1, const Player& p2) const;
    bool operator>(const Player& p2) const;
    bool operator<(const Player& p2) const;
    bool operator!=(const Player& p2) const;
    bool operator==(const Player& p2) const;
    bool operator<=(const Player& p2) const;


private:
    int m_playerId;
    Team* m_team;
    int m_gamesPlayed;
    int m_teamsGamesWithout;
    int m_goals;
    int m_cards;
    bool m_gkFlag;
    Player* m_closestAbove;
    Player* m_closestBelow;
};

#endif //MIVNEWET1_PLAYER_H