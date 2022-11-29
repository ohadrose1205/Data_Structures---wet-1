//
// Created by ohadr on 11/26/2022.
//

#include "Player.h"

Player::Player(const int ID, const int teamID, int gamesPlayed, int goals, int cards, const bool gkFlag)  :
playerId(ID), gamesPlayed(gamesPlayed), goals(goals), cards(cards), gkFlag(gkFlag){}

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

int Player::compareGoals(const Player &p1, const Player &p2) const {
    if(p1.getGoals() < p2.getGoals()){
        return 1;
    }else if(p1.getGoals() == p2.getGoals())
        return 0;
    else return -1;
}

int Player::compareIdPlayers(const Player &p1, const Player &p2) const {
    if(p1.getPlayerId() < p2.getPlayerId()){
        return 1;
    }else if(p1.getPlayerId() == p2.getPlayerId())
        return 0;
    else return -1;
}

int compareCards(const Player& p1, const Player& p2){
    if(p1.getCards() > p2.getCards()){
        return 1;
    }if(p1.getCards() == p2.getCards()){
        return 0;
    }else return -1;
}


bool Player::operator>(const Player &p2) const {
    if(compareGoals(*this, p2) != 0){ ///option 1: if p1 has more goals
        if(compareGoals(*this, p2) == 1){
            return true;
        }else{
            return false;
        }
    }else{
        if(compareCards(*this, p2) != 0) { ///option 2: if p1 has less cards
            if (compareCards(*this, p2) == -1) {
                return true;
            } else {
                return false;
            }
        }else{
            if(compareIdPlayers(*this,p2) == 1){ ///option 3: if p1 has greater ID
                return true;
            }else{
                return false;
            }
        }
    }
}

bool Player::sortFantasy(const Player &p1, const Player &p2) const {
    return (p1 > p2);
}
