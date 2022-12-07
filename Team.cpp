//
// Created by ohadr on 11/26/2022.
//

#include "Team.h"

Team::Team(int id, int initPoints){
    if(id <= 0 || initPoints < 0){
        throw std::exception();
    }
    m_teamId = id;
    m_points = initPoints;
    m_gamesPlayedAsTeam = 0;
    m_topScorer = nullptr;
    m_goalKeepers = 0;
    m_totalGoals = 0;
    m_totalCards = 0;
}
//Team::Team(const Team &t) :  m_teamId(id), m_playersTree( players), m_playersByGoalsTree(),m_gamesPlayedAsTeam {
//
//}

Team::Team(int id, int initPoints, const AVLTree<Player, int>& playersById, int GK, int totalGoals, int totalCards,
           const Player *topScorer) : m_playersByIdTree(playersById){
    m_teamId = id;
    m_points = initPoints;
    m_goalKeepers = GK;
    m_totalGoals = totalGoals;
    m_totalCards = totalCards;
    m_topScorer = topScorer;
}

Team::Team(const Team &t) : m_playersByIdTree(t.getPlayersByIdTree()){
    m_teamId = t.getTeamId();
    m_points = t.getPoints();
    m_gamesPlayedAsTeam = t.getTeamGames();
    m_goalKeepers = t.getGK();
    m_totalGoals = t.getTotalGoals();
    m_totalCards = t.getTotalsCards();
}

Team &Team::operator=(const Team &t) {
    if(&t == this){
        return *this;
    }
    m_teamId = t.getTeamId();
    m_points = t.getPoints();
    m_playersByIdTree = t.getPlayersByIdTree();
    m_gamesPlayedAsTeam = t.getTeamGames();
    m_topScorer = &t.getTopScorer();
    m_goalKeepers = t.getGK();
    m_totalGoals = t.getTotalGoals();
    m_totalCards = t.getTotalsCards();
}

bool Team::isTeamValid() const{
    return (getSize() > 10 && getGK() > 0);
}

int Team::getPoints() const {
    return m_points;
}

const int Team::getTeamId() const{
    return m_teamId;
}
const AVLTree<Player, int>& Team::getPlayersByIdTree() const{
    return (m_playersByIdTree);
}

int Team::getGK() const {
    return m_goalKeepers;
}

int Team::getTeamGames() const {
    return m_gamesPlayedAsTeam;
}

int Team::getNumPlayers() const{
    return m_playersByIdTree.size();
}

int Team::getTotalGoals() const {
    return m_totalGoals;
}

int Team::getTotalsCards() const {
    return m_totalCards;
}
int Team::getSize() const {
    return m_playersByIdTree.size();
}

void Team::changeGK(bool isAdded) {
    if(isAdded){
        m_goalKeepers++;
    }
    else{
        m_goalKeepers--;
    }
}

int Team::teamValue() const{
    return (m_points + m_totalGoals - m_totalCards);
}



bool Team::insertPlayer(const Player& playerToInsert) {
//    if(playerToInsert == nullptr){
//        return false;
//    }
    try{
        if(!m_playersByIdTree.find(playerToInsert.getTeamId()).empty()){
            AVLStatus checker1 = m_playersByIdTree.insert(playerToInsert, playerToInsert.getTeamId());
            m_totalGoals += playerToInsert.getGoals();
            m_totalCards += playerToInsert.getCards();
            if(checker1 == AVLStatus::AVL_Success){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
    }
    catch (...){
        return false;
    }
}

std::ostream& operator<<(std::ostream& out, const Team* team) {
    out << team->getTeamId() << std::endl;
    return out;
}


bool Team::removePlayer(int playerId) {
    if(playerId <= 0 ){
        return false;
    }
    try{
        Pair<Player,int> playerFound = m_playersByIdTree.find(playerId);
        if(!playerFound.empty()){
            Player playerToRemove = playerFound.data();
            m_totalGoals -= playerToRemove.getGoals();
            m_totalCards -= playerToRemove.getCards();
            AVLStatus checker1 = m_playersByIdTree.remove(playerId);
            if(checker1 == AVLStatus::AVL_Success){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }

    }
    catch (...){
        return false;
    }
}

const Player& Team::getTopScorer() const {
    return *m_topScorer;
}

bool Team::updateTopScorer(Player *messi) {
    if(messi == nullptr){
        return false;
    }
    if(*messi > *m_topScorer) {
        m_topScorer = messi;
        return true;
    }
    else{
        return false;
    }
}
void Team::loseGame() {
    m_gamesPlayedAsTeam += 1;
}

void Team::tieGame() {
    m_gamesPlayedAsTeam += 1;
    m_points += 1;
}

void Team::winGame() {
    m_gamesPlayedAsTeam += 1;
    m_points += 3;
}

//AVLStatus Team::updateTeam(AVLTree<Player, int> &playerTree, int goals, int cards, int GK, Player* topScorer) {
//    if(goals < 0 || cards < 0 || GK < 0 || topScorer == nullptr){
//        return AVLStatus::AVL_Fail;
//    }
//    m_playersTree = playerTree;
//    m_totalGoals = goals;
//    m_totalCards = cards;
//    m_goalKeepers = GK;
//    m_topScorer = topScorer;
//    return AVLStatus::AVL_Success;
//}

bool Team::empty() const{
    return m_playersByIdTree.empty();
}


int compareIdTeam(const Team& t1, const Team& t2){
    if((t1.getTeamId() == t2.getTeamId())){
        return 0;
    }
    if(t1.getTeamId() < t2.getTeamId()){
        return 1;
    }else {
        return -1;
    }
}

Team mergeTeams(Team& team1, Team& team2, int uniteId){
    AVLTree<Player, int>* unitePlayersById = mergeTrees(team1.getPlayersByIdTree(), team2.getPlayersByIdTree());
    int uniteGK = team1.getGK() + team2.getGK();
    int totalGoals = team1.getTotalGoals() + team2.getTotalGoals();
    int totalCards = team1.getTotalsCards() + team2.getTotalsCards();
    const Player* uniteTopScorer =
            team1.getTopScorer() > team2.getTopScorer() ? &team1.getTopScorer() : &team2.getTopScorer();
    Team united = Team(uniteId, team1.getPoints() + team2.getPoints(), *unitePlayersById, uniteGK, totalGoals, totalCards, uniteTopScorer);
    return united;
}