//
// Created by ohadr on 11/26/2022.
//

#include "Player.h"

Player::Player(const int ID, int teamId, int gamesPlayed, int goals, int cards, const bool gkFlag, const int gamesMissed) :
        m_playerId(ID), m_playerTeamId(teamId), m_gamesPlayedAlone(gamesPlayed), m_goals(goals), m_cards(cards), m_gkFlag(gkFlag), m_teamGamesWithOut(gamesMissed){
    m_closestAbove = nullptr;
    m_closestBelow = nullptr;
}

int Player::getPlayerId() const {
    return m_playerId;
}

int Player::getTeamId() const {
    return m_playerTeamId;
}

void Player::setTeamId(int newTeamId) {
    m_playerTeamId = newTeamId;
}

int Player::getIndividualGames() const {
    return m_gamesPlayedAlone;
}

int Player::getGoals() const {
    return m_goals;
}

void Player::updatePlayer(int gamesPlayed, int goals, int cards) {
    m_gamesPlayedAlone += gamesPlayed;
    m_goals += goals;
    m_cards += cards;
}

int Player::getCards() const {
    return m_cards;
}


bool Player::isGk() const {
    return m_gkFlag;
}

void Player::setClosestAbove(Player *closest) {
    m_closestAbove = closest;
}

void Player::setClosestBelow(Player *closest) {
    m_closestBelow = closest;
}

Player *Player::getClosest() {
    if(!m_closestAbove && !m_closestBelow){
        return nullptr;
    }
    else if(!m_closestAbove){
        return m_closestBelow;
    }
    else if(!m_closestBelow){
        return m_closestAbove;
    }
    if(compareGoals(*m_closestAbove, *m_closestBelow) != 0){ ///option 1: if p1 has more goals
        if(compareGoals(*m_closestAbove, *m_closestBelow) == 1){
            return m_closestAbove;
        }else{
            return m_closestBelow;
        }
    }else{
        if(compareCards(*m_closestAbove, *m_closestBelow) != 0) { ///option 2: if p1 has less cards
            if (compareCards(*m_closestAbove, *m_closestBelow) == -1) {
                return m_closestAbove;
            } else {
                return m_closestBelow;
            }
        }else{
            if(abs(m_playerId - m_closestAbove->getPlayerId()) != abs(m_playerId - m_closestBelow->getPlayerId())){
                if(abs(m_playerId - m_closestAbove->getPlayerId()) < abs(m_playerId - m_closestBelow->getPlayerId())){
                    return m_closestAbove;
                } else{
                    return m_closestBelow;
                }
            } else{
                return m_closestAbove->getPlayerId() > m_closestBelow->getPlayerId() ? m_closestAbove : m_closestBelow;
            }
        }
    }
}


int Player::compareGoals(const Player &p1, const Player &p2) const {
    if(p1.getGoals() < p2.getGoals()){
        return 1;
    }else if(p1.getGoals() == p2.getGoals())
        return 0;
    else return -1;
}

bool Player::compareIdPlayers(const Player &p1, const Player &p2) const {
    if(p1.getPlayerId() == p2.getPlayerId()){
        throw AVLStatus::AVL_Fail;
    }
    if(p1.getPlayerId() < p2.getPlayerId()){
        return true;
    }else{
        return false;
    }
}

int Player::compareCards(const Player& p1, const Player& p2) const{
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

bool Player::operator!=(const Player &p2) const {
    if(!(*this > p2) && !(p2 > *this)){
        return false;
    }else{
        return true;
    }
}

bool Player::operator<(const Player &p2) const {
    if(p2 > *this && p2 != *this){
        return true;
    }
    else{
        return false;
    }
}

bool Player::operator==(const Player &p2) const {
    if(!(p2 < *this) && !(p2 > *this) && !(p2 != *this)){
        return true;
    }
    else{
        return false;
    }
}

bool Player::operator<=(const Player &p2) const {
    if(*this < p2 && *this == p2){
        return true;
    }
    else{
        return false;
    }
}