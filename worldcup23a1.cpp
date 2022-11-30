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
	return StatusType::FAILURE;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
	// TODO: Your code goes here
    shared_ptr<Player> playerPtr = m_playersById.find(playerId);
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
	// TODO: Your code goes here
	return 22;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	// TODO: Your code goes here
	return 30003;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	// TODO: Your code goes here
	return 2008;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
	// TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	// TODO: Your code goes here
    output[0] = 4001;
    output[1] = 4002;
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	// TODO: Your code goes here
	return 1006;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}

