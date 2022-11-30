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
    m_totalGoals = 0;
    m_totalCards = 0;
}

const bool Team::isTeamValid() {
    return (m_playersNum > 10 && m_gkInclude);
}

const int Team::getPoints() const {
    return m_points;
}

const int Team::getTeamId() const{
    return m_teamId;
}

const int Team::getNumPlayers() {
    return m_playersNum;
}

const int Team::teamValue() {
    return (m_points + m_totalGoals - m_totalCards);
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

AVLStatus Team::insertPlayer(Player* playerToInsert) {
    if(playerToInsert == nullptr){
        return AVLStatus::AVL_Fail;
    }
    try{
        if(m_playersTree.find(playerToInsert->getTeamId()) != nullptr){
            AVLStatus checker = m_playersTree.insert(*playerToInsert, playerToInsert->getTeamId());
            m_totalGoals += playerToInsert->getGoals();
            m_totalCards += playerToInsert->getCards();
            m_playersNum += 1;
            return checker;
        }

    }
    catch (...){

    }
}


AVLStatus Team::removePlayer(int playerId) {
    if(playerId <= 0 ){
        return AVLStatus::AVL_Fail;
    }
    try{
        Player* playerToRemove = m_playersTree.find(playerId)->Data();
        if(playerToRemove != nullptr){
            m_totalGoals -= playerToRemove->getGoals();
            m_totalCards -= playerToRemove->getCards();
            AVLStatus checker = m_playersTree.remove(playerId);
            m_playersNum -= 1;
            return checker;
        }

    }
    catch (...){

    }
}

const Player *Team::getTopScorer() const {
    return m_topScorer;
}

AVLStatus Team::updateTopScorer(Player *messi) {
    if(messi == nullptr){
        return AVLStatus::AVL_Fail;
    }
    if(*messi > *m_topScorer) {
        m_topScorer = messi;
        return AVLStatus::AVL_Success;
    }
}

