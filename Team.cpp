//
// Created by ohadr on 11/26/2022.
//

#include "Team.h"

Team::Team(int id, int initPoints) : teamId(id), points(initPoints){
    gamesPlayedAsTeam = 0;
    playersNum = 0;
    playersTree = nullptr;
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

bool Team::compareIdTeam(const Team& t1, const Team& t2) const{
    return (t1.getTeamId() > t2.getTeamId());
}

