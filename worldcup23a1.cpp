#include "worldcup23a1.h"

world_cup_t::world_cup_t() : m_teamsTree(AVLTree<Team, int>()),
                             m_validTeamsTree(AVLTree<Team, int>()),
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
    Team* teamPtr = m_teamsTree.find(teamId)->data();
    if(teamPtr != nullptr){
        return StatusType::FAILURE;
    }
	// TODO: Your code goes here
    try {
        Team* newTeam = new Team(teamId, points);
        AVLStatus status = m_teamsTree.insert(*newTeam,teamId);
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
    Team* teamToDelete = m_teamsTree.find(teamId)->data();
    if(teamToDelete->getNumPlayers() == 0){
        return StatusType::FAILURE;
    }
    else{
        try {
            AVLStatus checker = m_teamsTree.remove(teamId);
            delete(teamToDelete);
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
        Team* playerTeam = m_teamsTree.find(teamId)->data();
        Player newPlayer = Player(playerId, playerTeam->getTeamId(), gamesPlayed, goals, cards, goalKeeper);
        AVLStatus checker1 = m_playersById.insert(newPlayer, playerId);
        AVLStatus checker2 = m_playersByGoals.insert(newPlayer, newPlayer);
        AVLStatus checker3 = playerTeam->insertPlayer(&newPlayer);
        if(checker1 == AVLStatus::AVL_Success && checker2 == AVLStatus::AVL_Success && checker3 == AVLStatus::AVL_Success){
            if(goalKeeper){
                playerTeam->changeGK(true);
            }
            if(playerTeam->isTeamValid() && m_validTeamsTree.find(playerTeam->getTeamId()) == nullptr){
                m_validTeamsTree.insert(*playerTeam, playerTeam->getTeamId());
            }
            return StatusType::SUCCESS;
        } else{
            return StatusType::ALLOCATION_ERROR;
        }
    }
    catch (...){
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType world_cup_t::remove_player(int playerId)
{
	// TODO: Your code goes here

    try{
        if(playerId <= 0){
            return StatusType::INVALID_INPUT;
        }
        Player* playerInId = m_playersById.find(playerId)->data();
        bool isGK = playerInId->isGk();
        if(playerInId == nullptr){
            return StatusType::FAILURE;
        }
        Team* playerTeam = m_teamsTree.find(playerInId->getTeamId())->data();
        Player* playerInGoals = m_playersByGoals.find(*playerInId)->data();
        AVLStatus checker1 = m_playersByGoals.remove(*playerInId);
        AVLStatus checker2 = m_playersById.remove(playerId);
        AVLStatus checker3 = playerTeam->removePlayer(playerId);
        if(checker1 == AVLStatus::AVL_Success && checker2 == AVLStatus::AVL_Success && checker3 == AVLStatus::AVL_Success){
            if(isGK){
                playerTeam->changeGK(false);
            }
            delete(playerInId);
            return StatusType::SUCCESS;
        }else{
            return StatusType::ALLOCATION_ERROR;
        }
    }
    catch (...){
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
	// TODO: Your code goes here
    if(playerId <= 0 || gamesPlayed <= 0 || scoredGoals <= 0 || cardsReceived <= 0){
        return StatusType::INVALID_INPUT;
    }
    Player* playerPtr = m_playersById.find(playerId)->data();
    if(playerPtr == nullptr){
        return StatusType::FAILURE;
    }
    AVLStatus checker = m_playersByGoals.remove(*playerPtr);
    if(checker != AVLStatus::AVL_Success){
        return StatusType::ALLOCATION_ERROR;
    }
    playerPtr->setExtraGames(gamesPlayed);
    playerPtr->setExtraGoals(scoredGoals);
    playerPtr->setExtraCards(cardsReceived);
    if(playerPtr->getGoals() > m_topScorer->getGoals()){
        this->updateTopScorer(playerPtr);
    }
    checker = m_playersByGoals.insert(*playerPtr, *playerPtr);
    if(checker != AVLStatus::AVL_Success){
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	// TODO: Your code goes here
    if(teamId1 <= 0 || teamId2 <= 0 || teamId2 == teamId1){
        return StatusType::FAILURE;
    }
    Team* team1 = m_teamsTree.find(teamId1)->data();
    Team* team2 = m_teamsTree.find(teamId2)->data();
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
    Player* playerPtr = m_playersById.find(playerId)->data();
    if(playerPtr == nullptr){
        return output_t<int>(StatusType::FAILURE);
    }
    Team* teamPlayer = m_teamsTree.find(playerPtr->getTeamId())->data();
	return output_t<int>(playerPtr->getIndividualGames() + teamPlayer->getTeamGames());
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	// TODO: Your code goes here
    if(teamId <= 0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    Team* teamPtr = m_teamsTree.find(teamId)->data();
    if(teamPtr == nullptr){
        return output_t<int>(StatusType::FAILURE);
    }
    return output_t<int>( teamPtr->getPoints());
}


/////-------------help function of unite----------------
//StatusType removeAndAdd(world_cup_t* tournament, Player* player, int newTeamId){
//    int playerId = player->getPlayerId();
//    int playerGames = player->getGames();
//    int playerGoals = player->getGoals();
//    int playerCards = player->getCards();
//    bool playerGK = player->isGk();
//    StatusType result = tournament->remove_player(playerId);
//    if(result == StatusType::SUCCESS){
//        result = tournament->add_player(playerId, newTeamId, playerGames, playerGoals, playerCards, playerGames);
//        return result;
//    }
//    else{
//        return StatusType::ALLOCATION_ERROR;
//    }
//}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	// TODO: Your code goes here
    if(teamId1 <= 0 || teamId2 <= 0 || teamId2 == teamId1 || newTeamId <= 0){
        return StatusType::INVALID_INPUT;
    }
    Team* team1 = m_teamsTree.find(teamId1)->data();
    Team* team2 = m_teamsTree.find(teamId2)->data();
    if(team1 == nullptr || team2 == nullptr || m_teamsTree.find(newTeamId) != nullptr){
        return StatusType::FAILURE;
    }
    add_team(newTeamId, team1->getPoints() + team2->getPoints());
    AVLTree<Player, int>* unitePlayers = mergeTrees(team1->getPlayersTree(), team2->getPlayersTree());
    ///UPDATE EACH PLAYER TEAM ID
    Team* uniteTeamPtr = m_teamsTree.find(newTeamId)->data();
    int uniteGK = team1->getGK() + team2->getGK();
    int totalGoals = team1->getTotalGoals() + team2->getTotalGoals();
    int totalCards = team1->getTotalsCards() + team2->getTotalsCards();
    Player* uniteTopScorer = *team1->getTopScorer() > *team2->getTopScorer() ? team1->getTopScorer() : team2->getTopScorer();
    uniteTeamPtr->updateTeam(*unitePlayers, totalGoals, totalCards, uniteGK, uniteTopScorer);
    return StatusType::SUCCESS;
//    Pair<Player, int>* arr1 =team1->getPlayersTree().inOrderScanToArray();
//    Pair<Player, int>* arr2 = team2->getPlayersTree().inOrderScanToArray();
//    int ptrArr1 = 0;
//    int ptrArr2 = 0;
//    for(;ptrArr1 < team1->getNumPlayers() || ptrArr2 < team2->getNumPlayers();){
//        StatusType result;
//        if(arr1[ptrArr1].data() > arr2[ptrArr2].data()){
//            result = removeAndAdd(this, arr1[ptrArr1].data(), newTeamId);
//            ptrArr1++;
//        }
//        else{
//            result = removeAndAdd(this, arr2[ptrArr2].data(), newTeamId);
//            ptrArr2++;
//        }
//        if(result != StatusType::SUCCESS){
//            return result;
//        }
    }
//    AVLTree<Player, int> team1Players = team1->getPlayersTree();
//    AVLTree<Player, int> team2Players = team2->getPlayersTree();
//    AVLTree<Player, int>* newUniteTeam = mergeTreesByKeyGenFunc(team1Players, team2Players);
//    int uniteTeamPoints = team1->getPoints() + team2->getPoints();
//    Team* uniteTeam = new Team(newTeamId, uniteTeamPoints, *newUniteTeam, uniteGK, totalGoals, totalCards, uniteTopScorer);
//    Pair<Team, int> unitePair = Pair<Team, int>(*uniteTeam, newTeamId);

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
            Team* teamPtr = m_teamsTree.find(teamId)->data();
            if(teamPtr == NULL || teamPtr->getNumPlayers() == 0){
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
        Team* teamPtr = m_teamsTree.find(teamId)->data();
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
    if(teamId == 0 || output == nullptr){
        return StatusType::INVALID_INPUT;
    }
    if(teamId < 0){
        if(m_numPlayers <= 0){
            return StatusType::FAILURE;
        }
        Pair<Player, Player>* arrOfPlayers = m_playersByGoals.inOrderScanToArray();
        for(int i = m_numPlayers - 1; i >= 0 ; i--){ //because demand the biggest first
            output[i] = arrOfPlayers[i].data()->getPlayerId();
        }
        return StatusType::SUCCESS;
    }
    else{
        Team* teamPtr = m_teamsTree.find(teamId)->data();
        if(teamPtr == nullptr || teamPtr->getNumPlayers() <= 0) {
            return StatusType::FAILURE;
        }
        Pair<Player, Player>* arrOfPlayers = teamPtr->arrByGoals();
        for(int i = teamPtr->getNumPlayers() - 1; i >= 0 ; i--){ //because demand the biggest first
            output[i] = arrOfPlayers[i].data()->getPlayerId();
        }
        return StatusType::SUCCESS;
    }
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	// TODO: Your code goes here
    if(teamId <= 0 || playerId <= 0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    Team* teamPtr = m_teamsTree.find(teamId)->data();
    Player* playerPtr = m_playersById.find(playerId)->data();
    if(teamPtr == nullptr || playerPtr == nullptr) {
        return output_t<int>(StatusType::FAILURE);
    }
    int rankOfPlayer = m_playersByGoals.rank(*playerPtr);
    Player* option1 = m_playersByGoals.select(rankOfPlayer+1).data();
    Player* option2 = m_playersByGoals.select(rankOfPlayer-1).data();
    if(*option1 > *option2){
        return output_t<int>(option1->getPlayerId());
    }
    if(*option2 > *option1){
        return output_t<int>(option2->getPlayerId());
    }
    else{
        return output_t<int>(StatusType::FAILURE);
    }
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
    if(minTeamId <= 0 || maxTeamId <= 0 || maxTeamId < minTeamId){
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    int sizeArr = 0;
    Pair<Team, int>* arrayOfLegalTeam = m_validTeamsTree.conditionArr(&sizeArr, minTeamId, maxTeamId);
    if(sizeArr == 0){
        return output_t<int>(StatusType::FAILURE);
    }
    try{
        Pair<int, int>* teamValuesAndId = new Pair<int, int>[sizeArr]();
        for(int i = 0; i < sizeArr; i++){ //good complexity cause sizeArr equal to r valid teams
            teamValuesAndId[i] = Pair<int, int>(arrayOfLegalTeam[i].data()->teamValue(), arrayOfLegalTeam[i].data()->getTeamId());
        }
        while(sizeArr > 1){
            int ptrArrTeam1 = 0;
            int ptrArrTeam2 = 1;
            int nextRoundSize;
            if(sizeArr % 2 != 0){
                nextRoundSize = (sizeArr / 2) + 1;
            }
            else{
                nextRoundSize = (sizeArr / 2);
            }
            Pair<int, int>* nextRoundTeam = new Pair<int, int>[sizeArr / 2]();
            for(int i = 0; ptrArrTeam1 < sizeArr && ptrArrTeam2 < sizeArr && i < nextRoundSize; ptrArrTeam1+2, ptrArrTeam2+2, i++){
                int idWinner;
                if(teamValuesAndId[ptrArrTeam1].data() > teamValuesAndId[ptrArrTeam2].data()){
                    idWinner = *(teamValuesAndId[ptrArrTeam1].key());
                }
                else if(teamValuesAndId[ptrArrTeam1].data() < teamValuesAndId[ptrArrTeam2].data()){
                    idWinner = *(teamValuesAndId[ptrArrTeam2].key());
                }
                else{
                    idWinner = teamValuesAndId[ptrArrTeam1].data() > teamValuesAndId[ptrArrTeam2].data() ? *(teamValuesAndId[ptrArrTeam1].key()) : *(teamValuesAndId[ptrArrTeam2].key());
                }
                int valuesWinner = *(teamValuesAndId[ptrArrTeam1].data()) + *(teamValuesAndId[ptrArrTeam1].data()) + 3;
                nextRoundTeam[i] = Pair<int, int>(valuesWinner, idWinner);
            }
            if(ptrArrTeam1 == sizeArr - 1){
                nextRoundTeam[nextRoundSize-1] = teamValuesAndId[ptrArrTeam1];
            }
            teamValuesAndId = nextRoundTeam;
            sizeArr = nextRoundSize;

        }
        return output_t<int>(*(teamValuesAndId[0].key()));
    }
    catch(bad_alloc){
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}
