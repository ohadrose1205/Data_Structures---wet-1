//
// Created by ohadr on 11/26/2022.
//

#ifndef AVL_TREES_H_TEAM_H
#define AVL_TREES_H_TEAM_H
#include "Player.h"
#include "AVL_trees.h"

class Team{
private:
    int m_teamId;
    AVLTree<Player, int> m_playersTree;
    int m_gamesPlayedAsTeam;
    int m_points;
    int m_playersNum;
    Player* m_topScorer;
    bool m_gkInclude;
    int m_totalGoals;
    int m_totalCards;

public:
    Team(int id, int initPoints);
    ~Team(); ///Should be default? because AVL destroyer
    Team(const Team& t) =default; ///cannot copy a team, each team unique
    Team& operator=(const Team& t);

    ///-------getters-------
    const int teamValue();
    const int getTeamId() const;
    const int getPoints() const;
    const int getNumPlayers();
    const bool isTeamValid();
    const Player* getTopScorer() const;

    ///--------setters-------
    bool compareIdTeam(const Team& t1, const Team& t2) const; ///check if t1 id is greater than t2
    AVLStatus updateTopScorer(Player* messi);
    AVLStatus insertPlayer(Player* newPlayer);
    AVLStatus removePlayer(int playerId);
    AVLStatus playGame(char victoryFlag){
        if(victoryFlag != 'W' && victoryFlag != 'L' && victoryFlag != 'T'){
            return AVLStatus::AVL_Fail;
        }
        m_gamesPlayedAsTeam += 1;
        if(victoryFlag == 'W'){ //Win
            m_points += 3;
        } else if(victoryFlag == 'T'){ //Tie
            m_points += 1;
            return AVLStatus::AVL_Success;
        }else{ //Lose
            return AVLStatus::AVL_Success;
        }
    }
};

#endif //AVL_TREES_H_TEAM_H
