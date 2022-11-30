//
// Created by shach on 11/22/2022.
//

#ifndef MIVNEWET1_PLAYER_H
#define MIVNEWET1_PLAYER_H


#include "AVL_trees.h"

class Player {
public:
    explicit Player(const int ID, const int teamID, int gamesPlayed, int goals, int cards, const bool gkFlag);
    ~Player() =default;
    Player(const Player& p) =default; ///cannot copy players, each player unique
    Player& operator=(const Player& p) = default;

    ///-----getters--------
    int getPlayerId() const;
    int getTeamId() const;
    int getGoals() const;
    int getCards() const;
    int getGames() const;
    bool isGk() const;

    ///------setters-------
    void setExtraGoals(int goals);
    void setExtraCards(int cards);
    void setExtraGames(int games);

    bool compareIdPlayers(const Player& p1, const Player& p2) const; ///check if p1 id is greater than p2
    int compareGoals(const Player& p1, const Player& p2) const; ///check if p1 goals is greater than p2
    int compareCards(const Player& p1, const Player& p2) const;
    bool operator>(const Player& p2) const;
    bool operator!=(const Player& p2) const;

private:
    int m_playerId;
    int m_teamId;
    int m_gamesPlayed;
    int m_goals;
    int m_cards;
    bool m_gkFlag;
};

static bool sortFantasy(const Player& p1, const Player& p2){
        return (p1 > p2);
}

#endif //MIVNEWET1_PLAYER_H
