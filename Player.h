//
// Created by shach on 11/22/2022.
//

#ifndef MIVNEWET1_PLAYER_H
#define MIVNEWET1_PLAYER_H


class Player {
public:
    Player(const int ID, const int teamID, int gamesPlayed, int goals, int cards, const bool gkFlag);
    ~Player() =default;
    Player(const Player& p) =delete; ///cannot copy players, each player unique
    Player& operator=(const Player& p) = delete;

    int getPlayerId() const;
    int getGoals() const;
    int getCards() const;
    bool isGk() const;

    bool compareIdPlayers(const Player& p1, const Player& p2) const; ///check if p1 id is greater than p2
    bool compareGoals(const Player& p1, const Player& p2) const; ///check if p1 goals is greater than p2

private:
    int playerId;
    int teamId;
    int gamesPlayed;
    int goals;
    int cards;
    bool gkFlag;
};


#endif //MIVNEWET1_PLAYER_H
