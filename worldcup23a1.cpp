#include "worldcup23a1.h"

world_cup_t::world_cup_t() : m_teamsTree(AVLTree<Team, int>()),
                             m_playersById(AVLTree<Player, int>()),
                             m_playersByGoals(AVLTree<Player, Player>())
{
	// TODO: Your code goes here
    m_numPlayers = 0;
    m_numTeams = 0;
    m_topScorer = nullptr;
    m_totalGames = 0;

}

world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
    ///as default
}


StatusType world_cup_t::add_team(int teamId, int points)
{
    if(teamId <= 0 || points < 0){
        return StatusType::INVALID_INPUT;
    }
    if(m_teamsTree.find(teamId) != nullptr){
        return StatusType::FAILURE;
    }
	// TODO: Your code goes here
    try {
        Team newTeam = Team(teamId, points);
        AVLStatus status = m_teamsTree.insert(newTeam,teamId);
        if(status == AVLStatus::AVL_Fail){
            return StatusType::ALLOCATION_ERROR;
        }else{
            return StatusType::SUCCESS;
        }
    }catch(...){
        throw;
    }
}

StatusType world_cup_t::remove_team(int teamId)
{
	// TODO: Your code goes here
    if(teamId < 0){
        return StatusType::FAILURE;
    }
    Team* teamToDelete = m_teamsTree.find(teamId)->Data();
    if(teamToDelete->getNumPlayers() == 0){
        return StatusType::FAILURE;
    }
    else{
        try {
            AVLStatus checker = m_teamsTree.remove(teamId);
            if(checker == AVLStatus::AVL_Success){
                return StatusType::SUCCESS;
            }
            else{
                return StatusType::FAILURE;
            }
        }
        catch(...){
            throw;
        }

    }
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
	// TODO: Your code goes here
    if(playerId < 0 || teamId < 0 || goals < 0 || cards < 0 || gamesPlayed < 0){
        return StatusType::INVALID_INPUT;
    }
    if(m_playersById.find(playerId) != nullptr){
        return StatusType::FAILURE;
    }
    try{
        Player newPlayer = Player(playerId, teamId, gamesPlayed, goals, cards, goalKeeper);
        Team* playerTeam = m_teamsTree.find(teamId)->Data();
        AVLStatus checker1 = m_playersById.insert(newPlayer, playerId);
        AVLStatus checker2 = m_playersByGoals.insert(newPlayer, newPlayer); ///CHECK THAT
        AVLStatus checker3 = playerTeam->insertPlayer(&newPlayer);
        if(checker1 == AVLStatus::AVL_Success && checker2 == AVLStatus::AVL_Success && checker3 == AVLStatus::AVL_Success){
            return StatusType::SUCCESS;
        } else{
            return StatusType::ALLOCATION_ERROR;
        }
    }
    catch (...){
        throw;
    }
}

StatusType world_cup_t::remove_player(int playerId)
{
	// TODO: Your code goes here

    try{
        Player* ptrInId = m_playersById.find(playerId)->Data();
        if(ptrInId == nullptr){
            return StatusType::FAILURE;
        }
        Team* playerTeam = m_teamsTree.find(ptrInId->getTeamId())->Data();
        Player* playerInGoals = m_playersByGoals.find(*ptrInId)->Data();
        AVLStatus checker1 = m_playersByGoals.remove(*ptrInId);
        AVLStatus checker2 = m_playersById.remove(playerId);
        AVLStatus checker3 = playerTeam->removePlayer(playerId);
        if(checker1 == AVLStatus::AVL_Success && checker2 == AVLStatus::AVL_Success && checker3 == AVLStatus::AVL_Success){
            return StatusType::SUCCESS;
        }else{
            return StatusType::ALLOCATION_ERROR;
        }
    }
    catch (...){

    }
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
	// TODO: Your code goes here
    if(playerId <= 0 || gamesPlayed <= 0 || scoredGoals <= 0 || cardsReceived <= 0){
        return StatusType::INVALID_INPUT;
    }
    Player* playerPtr = m_playersById.find(playerId)->Data();
    if(playerPtr == nullptr){
        return StatusType::FAILURE;
    }
    playerPtr->setExtraGames(gamesPlayed);
    playerPtr->setExtraGoals(scoredGoals);
    playerPtr->setExtraCards(cardsReceived);
    if(playerPtr->getGoals() > m_topScorer->getGoals()){
        this->updateTopScorer(playerPtr);
    }
    ///should be function of AVL_tree class that update the tree from outside
    ///option: remove and re-insert...COPMLEXITY
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	// TODO: Your code goes here
    if(teamId1 <= 0 || teamId2 <= 0 || teamId2 == teamId1){
        return StatusType::FAILURE;
    }
    Team* team1 = m_teamsTree.find(teamId1)->Data();
    Team* team2 = m_teamsTree.find(teamId2)->Data();
    if(team1 != nullptr && team2 != nullptr){
        if(!team1->isTeamValid() || !team2->isTeamValid()){
            return StatusType::FAILURE;
        }
    }else{
        return StatusType::FAILURE;
    }
    if(team1->teamValue() > team2->teamValue()){
        team1->playGame('W');
        team2->playGame('L');
    } else if(team1->teamValue() < team2->teamValue()){
        team2->playGame('W');
        team1->playGame('L');
    }else{
        team1->playGame('T');
        team2->playGame('T');
    }
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
	// TODO: Your code goes here
    if(playerId <= 0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    Player* playerPtr = m_playersById.find(playerId)->Data();
    if(playerPtr == nullptr){
        return output_t<int>(StatusType::FAILURE);
    }
	return output_t<int>(playerPtr->getGames());;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	// TODO: Your code goes here
    if(teamId <= 0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    Team* teamPtr = m_teamsTree.find(teamId)->Data();
    if(teamPtr == nullptr){
        return output_t<int>(StatusType::FAILURE);
    }
    return output_t<int>( teamPtr->getPoints());
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	// TODO: Your code goes here
    if(teamId1 <= 0 || teamId2 <= 0 || teamId2 == teamId1 || newTeamId <= 0){
        return StatusType::INVALID_INPUT;
    }
    Team* team1 = m_teamsTree.find(teamId1)->Data();
    Team* team2 = m_teamsTree.find(teamId2)->Data();
    if(team1 == nullptr || team2 == nullptr || m_teamsTree.find(newTeamId) != nullptr){
        return StatusType::FAILURE;
    }
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	// TODO: Your code goes here
    if(teamId == 0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    if(m_numPlayers != 0){
        if(teamId < 0){
            return output_t<int>(m_topScorer->getPlayerId());
        }
        else{
            Team* teamPtr = m_teamsTree.find(teamId)->Data();
            if(teamPtr == nullptr || teamPtr->getNumPlayers() == 0){
                return output_t<int>(StatusType::FAILURE);
            } else{
                return output_t<int>(teamPtr->getTopScorer()->getPlayerId());
            }
        }
    }else{
        return output_t<int>(StatusType::FAILURE);
    }
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
	// TODO: Your code goes here
    if(teamId == 0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    if(teamId < 0){
        return m_numPlayers;
    }else{
        Team* teamPtr = m_teamsTree.find(teamId)->Data();
        if(teamPtr == nullptr){
            return output_t<int>(StatusType::FAILURE);
        } else{
            return output_t<int>(teamPtr->getNumPlayers());
        }
    }
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	// TODO: Your code goes here
    if(teamId == 0 || output == NULL){
        return StatusType::INVALID_INPUT;
    }
    Team* teamPtr = m_teamsTree.find(teamId)->Data();
    if(teamPtr == nullptr) {
        return StatusType::FAILURE;
    }
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	// TODO: Your code goes here
    if(teamId <= 0 || playerId <= 0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    Team* teamPtr = m_teamsTree.find(teamId)->Data();
    Player* playerPtr = m_playersById.find(playerId)->Data();
    if(teamPtr == nullptr || playerPtr == nullptr) {
        return output_t<int>(StatusType::FAILURE);
    }

}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
    if(minTeamId <= 0 || maxTeamId <= 0 || maxTeamId < minTeamId){
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    Team* minTeamPtr = m_teamsTree.find(minTeamId)->Data();
    Team* maxTeamPtr = m_teamsTree.find(minTeamId)->Data();
    if(minTeamPtr == nullptr || maxTeamPtr == nullptr) {
        return output_t<int>(StatusType::FAILURE);
    }
}

