//
// Created by shach on 11/22/2022.
//

#ifndef MIVNEWET1_PLAYER_H
#define MIVNEWET1_PLAYER_H

#include "AVL_trees.h"

class Player {
public:
    explicit Player(const int ID, int teamId, int gamesPlayed, int goals, int cards, const bool gkFlag, const int gamesMissed);
    ~Player() =default;
    Player(const Player& p) =default;
    Player& operator=(const Player& p) = default;

    ///-----getters--------
    int getPlayerId() const;
    int getTeamId() const;
    int getGoals() const;
    int getCards() const;
    int getIndividualGames() const; //games that player played AS AN UPDATE and not in the team
    bool isGk() const;
    Player* getClosest();

    ///------setters-------
    void updatePlayer(int gamesPlayed, int goals, int cards);
    void setTeamId(int newTeamId);
    void setClosestAbove(Player* closest);
    void setClosestBelow(Player* closest);


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
    int m_playerTeamId;
    int m_gamesPlayedAlone;
    const int m_teamGamesWithOut;
    int m_goals;
    int m_cards;
    bool m_gkFlag;
    Player* m_closestAbove;
    Player* m_closestBelow;
};

#endif //MIVNEWET1_PLAYER_H