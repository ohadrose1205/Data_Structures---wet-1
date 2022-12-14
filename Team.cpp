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
    m_totalGames = 0;
    m_topScorer = nullptr;
    m_totalGKs = 0;
    m_totalGoals = 0;
    m_totalCards = 0;
}
//Team::Team(const Team &t) :  m_teamId(id), m_playersTree( players), m_playersByGoalsTree(),m_totalGames {
//
//}

Team::Team(int id, const AVLTree<Player*,int>& players,const AVLTree<Player*,Player>& scoreBoard, int totalGames, int totalGoals, int totalCards, int totalGKs, int points, const Player *topScorer) : m_playersByIdTree(players), m_playersByGoals(scoreBoard){
    m_teamId = id;
    m_totalGames = totalGames;
    m_totalGoals = totalGoals;
    m_totalCards = totalCards;
    m_totalGKs = totalGKs;
    m_points = points;
    m_topScorer = topScorer;
}

bool Team::isTeamValid() const{
    return (getTeamSize() >= 11 && getNumOfGks() > 0);
}

bool Team::empty() const{
    return m_playersByIdTree.empty();
}

const Player* Team::getTopScorer() const {
    return m_topScorer;
}

int Team::getPoints() const {
    return m_points;
}

int Team::getTeamId() const{
    return m_teamId;
}

const AVLTree<Player*, int>& Team::getPlayersIdTree() const{
    return m_playersByIdTree;
}

const AVLTree<Player *, Player> &Team::getScoreBoard() const {
    return m_playersByGoals;
}

int Team::getNumOfGks() const {
    return m_totalGKs;
}

int Team::getTeamGames() const {
    return m_totalGames;
}

int Team::getTotalGoals() const {
    return m_totalGoals;
}

int Team::getTotalsCards() const {
    return m_totalCards;
}

int Team::getTeamSize() const {
    return m_playersByIdTree.size();
}

int Team::teamValue() const{
    return (m_points + m_totalGoals - m_totalCards);
}

StatusType Team::insertPlayer(Player* playerToInsert, int playerId) {
    if(!playerToInsert)
        return StatusType::INVALID_INPUT;
    StatusType checker1 = m_playersByIdTree.insert(playerToInsert, playerId);
    if(checker1 == StatusType::SUCCESS){
        StatusType checker2 = m_playersByGoals.insert(playerToInsert, *playerToInsert);
        if(checker2 == StatusType::SUCCESS){
            if(playerToInsert->isGk())
                addGk();
            m_totalGoals += playerToInsert->getGoals();
            m_totalCards += playerToInsert->getCards();
            playerToInsert->setTeam(this);
            updateTopScorer();
        }
        return checker2;
    }
    return checker1;
}

StatusType Team::removePlayer(int playerId) {
    Pair<Player *, int>* playerFound = m_playersByIdTree.find(playerId);
    if(!playerFound){
        return StatusType::FAILURE;
    }
    Player toDelete = *playerFound->data();
    m_playersByGoals.remove(toDelete);
    m_playersByIdTree.remove(playerId);
    m_totalGoals -= toDelete.getGoals();
    m_totalCards -= toDelete.getCards();
    if(toDelete.isGk())
        removeGk();
    updateTopScorer();
    return StatusType::SUCCESS;
}

void Team::updateTopScorer() {
    if(!m_playersByGoals.empty())
        m_topScorer = m_playersByGoals.select(m_playersByGoals.size())->data();
    else
        m_topScorer = nullptr;
}

void Team::loseGame() {
    m_totalGames++;
}

void Team::tieGame() {
    m_totalGames++;
    m_points++;
}

void Team::winGame() {
    m_totalGames++;
    m_points += 3;
}

void Team::addGk() {
    m_totalGKs++;
}

void Team::removeGk() {
    m_totalGKs = m_totalGKs <= 0 ? 0 : m_totalGKs - 1;
}

void Team::updateAllPlayerTeamTag(){
    Pair<Player*,int>** arr = nullptr;
    if(!m_playersByIdTree.inOrderScanToArray(arr))
        return;
    for(int i = 0; i < m_playersByIdTree.size(); i++){
        arr[i]->data()->updateGamesPlayed();
        arr[i]->data()->setTeam(this);
    }
    delete[] arr;
}

//AVLStatus Team::updateTeam(AVLTree<Player, int> &playerTree, int goals, int cards, int GK, Player* topScorer) {
//    if(goals < 0 || cards < 0 || GK < 0 || topScorer == nullptr){
//        return AVLStatus::AVL_Fail;
//    }
//    m_playersTree = playerTree;
//    m_totalGoals = goals;
//    m_totalCards = cards;
//    m_totalGKs = GK;
//    m_topScorer = topScorer;
//    return AVLStatus::AVL_Success;
//}

std::ostream& operator<<(std::ostream& out, const Team* team) {
    out << team->getTeamId() << std::endl;
    return out;
}


Player *Team::findPlayer(int playerId) {
    Pair<Player*,int>* playerPair = m_playersByIdTree.find(playerId);
    if(playerPair){
        return playerPair->data();
    }
    else{
        return nullptr;
    }
}

Pair<Player*, Player>** Team::arrByGoals() {
    Pair<Player*, Player>** arr = nullptr;
    this->m_playersByGoals.inOrderScanToArray(arr);
    return arr;
}

Team* mergeTeams(const Team& team1,const Team& team2, int uniteId) {
    AVLTree<Player*, int>* unitePlayersById = mergeTrees(team1.m_playersByIdTree, team2.m_playersByIdTree);
    if(!unitePlayersById)
        return nullptr;
    AVLTree<Player*, Player>* unitePlayersByGoals = mergeTrees(team1.m_playersByGoals, team2.m_playersByGoals);
    if(!unitePlayersByGoals){
        delete unitePlayersById;
        return nullptr;
    }
    int totalGK = team1.getNumOfGks() + team2.getNumOfGks();
    int totalGoals = team1.getTotalGoals() + team2.getTotalGoals();
    int totalCards = team1.getTotalsCards() + team2.getTotalsCards();
    int totalPoints = team1.getPoints() + team2.getPoints();
    const Player* uniteTopScorer;
    if(!team1.getTopScorer()){
        uniteTopScorer = team2.getTopScorer();
    }else if(!team2.getTopScorer()){
        uniteTopScorer = team1.getTopScorer();
    }else{
        uniteTopScorer = *team1.getTopScorer() > *team2.getTopScorer() ? team1.getTopScorer() : team2.getTopScorer();
    }
    Team* uniteTeam;
    try{
        uniteTeam = new Team(uniteId, *unitePlayersById, *unitePlayersByGoals, 0, totalGoals, totalCards,totalGK, totalPoints, uniteTopScorer);
    }
    catch(...){
        uniteTeam = nullptr;
    }
    delete unitePlayersById;
    delete unitePlayersByGoals;
    unitePlayersById = nullptr;
    unitePlayersByGoals = nullptr;
    return uniteTeam;
}