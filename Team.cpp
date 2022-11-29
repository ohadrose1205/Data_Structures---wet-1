//
// Created by ohadr on 11/26/2022.
//

#include "Team.h"

Team::Team(int id, int initPoints){
    if(id > 0 || initPoints >= 0){
        throw;
    }
    teamId = id;
    points = initPoints;
    gamesPlayedAsTeam = 0;
    playersNum = 0;
    playersTree = AVLTree<Player, Player>(Player::compareIdPlayers);
    topScorer = nullptr;
    gkInclude = false;
}

const bool Team::isTeamValid() {
    return (playersNum > 10 && gkInclude);
}

const int Team::getTeamId() const{
    return teamId;
}

const int Team::teamValue() {
    return (points + this->goalsCount() - this->cardCount());
}

const int Team::goalsCount() {

}

const int Team::cardCount() {

}

Team* Team::compareIdTeam(const Team& t1, const Team& t2) const{
    if(t1.getTeamId() < t2.getTeamId()){
        return 1;
    }else if(t1.getTeamId() == t2.getTeamId()){
        return 0;
    }else return -1;
}

