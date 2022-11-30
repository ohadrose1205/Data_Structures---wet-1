//
// Created by ohadr on 11/26/2022.
//

#include "Team.h"

Team::Team(int id, int initPoints): m_playersTree(AVLTree<Player, int>()){
//    if(id <= 0 || initPoints < 0){
//        throw;
//    }
    m_teamId = id;
    m_points = initPoints;
    m_gamesPlayedAsTeam = 0;
    m_playersNum = 0;
    m_topScorer = nullptr;
    m_gkInclude = false;
}

const bool Team::isTeamValid() {
    return (m_playersNum > 10 && m_gkInclude);
}

const int Team::getTeamId() const{
    return m_teamId;
}

const int Team::teamValue() {
    return (m_points + this->goalsCount() - this->cardCount());
}

const int Team::goalsCount() {

}

const int Team::cardCount() {

}

bool Team::compareIdTeam(const Team& t1, const Team& t2) const{
    if(t1.getTeamId() < t2.getTeamId()){
        return true;
    }else if(t1.getTeamId() > t2.getTeamId()){
        return false;
    }else {
        ///exeption;
    }
}

