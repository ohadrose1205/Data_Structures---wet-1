//
// Created by ohadr on 11/26/2022.
//

#include "Player.h"

Player::Player(const int ID, const int teamID, int gamesPlayed, int goals, int cards, const bool gkFlag)  :
m_playerId(ID), m_gamesPlayed(gamesPlayed), m_goals(goals), m_cards(cards), m_gkFlag(gkFlag){}

int Player::getPlayerId() const {
    return m_playerId;
}

int Player::getGoals() const {
    return m_goals;
}

int Player::getCards() const {
    return m_cards;
}

bool Player::isGk() const {
    return m_gkFlag;
}

int Player::compareGoals(const Player &p1, const Player &p2) const {
    if(p1.getGoals() < p2.getGoals()){
        return 1;
    }else if(p1.getGoals() == p2.getGoals())
        return 0;
    else return -1;
}

 bool Player::compareIdPlayers(const Player &p1, const Player &p2) const {
    if(p1.getPlayerId() < p2.getPlayerId()){
        return true;
    }else if(p1.getPlayerId() > p2.getPlayerId())
        return false;
    else{
        ///exeption
    }
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
//
//bool sortFantasy(const Player &p1, const Player &p2){
//    return (p1 > p2);
//}
