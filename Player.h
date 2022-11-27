//
// Created by ohadr on 11/26/2022.
//

#ifndef AVL_TREES_H_PLAYER_H
#define AVL_TREES_H_PLAYER_H


class Player{
private:
    int playerId;
    int goals;
    int cards;
    int gamesPlayed; ///should be different from the teams gamesPlayedAsTeam
    bool gkFlag;
public:
    Player(int id, int initGamesPlayed, int initGoals, int initCards, bool gk);
    ~Player() =default;
    Player(const Player& p) =delete; ///cannot copy players, each player unique
    Player& operator=(const Player& p) =default;

    int getPlayerId() const;
    int getGoals() const;
    int getCards() const;
    bool isGk() const;

    bool compareIdPlayers(const Player& p1, const Player& p2) const; ///check if p1 id is greater than p2
    bool compareGoals(const Player& p1, const Player& p2) const; ///check if p1 goals is greater than p2
};

#endif //AVL_TREES_H_PLAYER_H
