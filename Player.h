//
// Created by shach on 11/22/2022.
//

#ifndef MIVNEWET1_PLAYER_H
#define MIVNEWET1_PLAYER_H


class Player {
public:
    explicit Player(const int ID, const int teamID, int gamesPlayed, int goals, int cards, const bool gkFlag);
    ~Player() =default;
    Player(const Player& p) =default; ///cannot copy players, each player unique
    Player& operator=(const Player& p) = default;

    int getPlayerId() const;
    int getGoals() const;
    int getCards() const;
    bool isGk() const;

    int compareIdPlayers(const Player& p1, const Player& p2) const; ///check if p1 id is greater than p2
    int compareGoals(const Player& p1, const Player& p2) const; ///check if p1 goals is greater than p2
    int compareCards(const Player& p1, const Player& p2) const;
    bool sortFantasy(const Player& p1, const Player& p2) const;

private:
    int playerId;
    int teamId;
    int gamesPlayed;
    int goals;
    int cards;
    bool gkFlag;
};

bool operator>(const Player& p1, const Player& p2);

#endif //MIVNEWET1_PLAYER_H
