//
// Created by ohadr on 11/26/2022.
//

#include "Player.h"

Player::Player(int id, int initGamesPlayed, int initGoals, int initCards, bool gk) :
playerId(id), gamesPlayed(initGamesPlayed), goals(initGoals), cards(initCards), gkFlag(gk){}

int Player::getPlayerId() const {
    return playerId;
}

int Player::getGoals() const {
    return goals;
}

int Player::getCards() const {
    return cards;
}

bool Player::isGk() const {
    return gkFlag;
}

bool Player::compareGoals(const Player &p1, const Player &p2) const {
    return (p1.getGoals() > p2.getGoals());
}

bool Player::compareIdPlayers(const Player &p1, const Player &p2) const {
    return (p1.getPlayerId() > p2.getPlayerId());
}