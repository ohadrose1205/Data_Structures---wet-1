//
// Created by ohadr on 11/26/2022.
//

#include "Team.h"

Team::Team(int id, int initPoints): m_playersTree(AVLTree<Player, int>()), m_playersByGoalsTree(AVLTree<Player, Player>()){
//    if(id <= 0 || initPoints < 0){
//        throw;
//    }
    m_teamId = id;
    m_points = initPoints;
    m_gamesPlayedAsTeam = 0;
    m_playersNum = 0;
    m_topScorer = nullptr;
    m_goalKeepers = 0;
    m_totalGoals = 0;
    m_totalCards = 0;
}

const bool Team::isTeamValid() {
    return (m_playersNum > 10 && getGK() > 0);
}

const int Team::getPoints() const {
    return m_points;
}

const int Team::getTeamId() const{
    return m_teamId;
}

const AVLTree<Player, int>* Team::getPlayersTree() const {
    return &m_playersTree;
}

const int Team::getNumPlayers() {
    return m_playersNum;
}

const int Team::getGK() const {
    return m_goalKeepers;
}

const int Team::getTeamGames() const {
    return m_gamesPlayedAsTeam;
}

const int Team::getTotalGoals() const {
    return m_totalGoals;
}

const int Team::getTotalsCards() const {
    return m_totalCards;
}

void Team::changeGK(bool isAdded) {
    if(isAdded){
        m_goalKeepers++;
    }
    else{
        m_goalKeepers--;
    }
}

const int Team::teamValue() {
    return (m_points + m_totalGoals - m_totalCards);
}

bool Team::compareIdTeam(const Team& t1, const Team& t2) const{
    if((t1.getTeamId() == t2.getTeamId())){
        throw AVLStatus::AVL_Fail;
    }
    if(t1.getTeamId() < t2.getTeamId()){
        return true;
    }else {
        return false;
    }
}

AVLStatus Team::insertPlayer(Player* playerToInsert) {
    if(playerToInsert == nullptr){
        return AVLStatus::AVL_Fail;
    }
    try{
        if(m_playersTree.find(playerToInsert->getTeamId()) != nullptr){
            AVLStatus checker1 = m_playersTree.insert(*playerToInsert, playerToInsert->getTeamId());
            AVLStatus checker2 = m_playersByGoalsTree.insert(*playerToInsert, *playerToInsert);
            m_totalGoals += playerToInsert->getGoals();
            m_totalCards += playerToInsert->getCards();
            m_playersNum += 1;
            if(checker1 == AVLStatus::AVL_Success && checker2 == AVLStatus::AVL_Success){
                return AVLStatus::AVL_Success;
            }
            else{
                return AVLStatus::AVL_Fail;
            }
        }
        else{
            return AVLStatus::AVL_Fail;
        }
    }
    catch (...){
        return AVLStatus::AVL_Alloc;
    }
}

std::ostream operator<<(std::ostream out, const Team* team) {
    out << team->getTeamId() << std::endl;
}


AVLStatus Team::removePlayer(int playerId) {
    if(playerId <= 0 ){
        return AVLStatus::AVL_Fail;
    }
    try{
        Player* playerToRemove = m_playersTree.find(playerId)->data();
        if(playerToRemove != nullptr){
            m_totalGoals -= playerToRemove->getGoals();
            m_totalCards -= playerToRemove->getCards();
            AVLStatus checker1 = m_playersTree.remove(playerId);
            AVLStatus checker2 = m_playersByGoalsTree.remove(*playerToRemove);
            m_playersNum -= 1;
            if(checker1 == AVLStatus::AVL_Success && checker2 == AVLStatus::AVL_Success){
                return AVLStatus::AVL_Success;
            }
            else{
                return AVLStatus::AVL_Fail;
            }
        }
        else{
            return AVLStatus::AVL_Not_Found;
        }

    }
    catch (...){
        return AVLStatus::AVL_Alloc;
    }
}

Player *Team::getTopScorer() const {
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
    else{
        return AVLStatus::AVL_Fail;
    }
}

AVLStatus Team::playGame(char victoryFlag) {
        if(victoryFlag != 'W' && victoryFlag != 'L' && victoryFlag != 'T'){
            return AVLStatus::AVL_Fail;
        }
        m_gamesPlayedAsTeam += 1;
        if(victoryFlag == 'W'){ //Win
            m_points += 3;
            return AVLStatus::AVL_Success;
        } else if(victoryFlag == 'T'){ //Tie
            m_points += 1;
            return AVLStatus::AVL_Success;
        }else{ //Lose
            return AVLStatus::AVL_Success;
        }
    }

AVLStatus Team::updateTeam(AVLTree<Player, int> &playerTree, int goals, int cards, int GK, Player* topScorer) {
    if(goals < 0 || cards < 0 || GK < 0 || topScorer == nullptr){
        return AVLStatus::AVL_Fail;
    }
    m_playersNum = playerTree.size();
    m_playersTree = playerTree;
    m_totalGoals = goals;
    m_totalCards = cards;
    m_goalKeepers = GK;
    m_topScorer = topScorer;
    return AVLStatus::AVL_Success;
}

Pair<Player, Player>* Team::arrByGoals() {
    return m_playersByGoalsTree.inOrderScanToArray();
}