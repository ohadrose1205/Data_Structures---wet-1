#include "worldcup23a1.h"

world_cup_t::world_cup_t() : m_teamsTree(),
                             m_validTeamsTree(),
                             m_playersById(),
                             m_playersByGoals()
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

void world_cup_t::updateOnInsert_CP_SB(const Player &newPlayer) {
    Player& sbPlayer = m_playersByGoals.find(newPlayer).data();
    int scoreBoardRank = m_playersByGoals.rank(sbPlayer);
    Pair<Player,Player> sbPlayerPairAbove =  m_playersByGoals.select(scoreBoardRank+1);
    Pair<Player,Player> sbPlayerPairBelow =  m_playersByGoals.select(scoreBoardRank-1);
    if(sbPlayerPairAbove.empty()){
        sbPlayer.setClosestAbove(nullptr);
    }else{
        sbPlayer.setClosestAbove(&sbPlayerPairAbove.data());
        sbPlayerPairAbove.data().setClosestBelow(&sbPlayer);
    }
    if(sbPlayerPairBelow.empty()){
        sbPlayer.setClosestBelow(nullptr);
    }else{
        sbPlayer.setClosestBelow(&sbPlayerPairBelow.data());
        sbPlayerPairBelow.data().setClosestAbove(&sbPlayer);
    }
}

void world_cup_t::updateOnRemove_CP_SB(const Player& retiredPlayer){
    Player& sbRetiredPlayer = m_playersByGoals.find(retiredPlayer).data();
    int scoreBoardRank = m_playersByGoals.rank(sbRetiredPlayer);
    Pair<Player,Player> sbPlayerPairAbove =  m_playersByGoals.select(scoreBoardRank+1);
    Pair<Player,Player> sbPlayerPairBelow =  m_playersByGoals.select(scoreBoardRank-1);
    if(!sbPlayerPairAbove.empty() && !sbPlayerPairBelow.empty()){
        sbPlayerPairAbove.data().setClosestBelow(&sbPlayerPairBelow.data());
        sbPlayerPairBelow.data().setClosestAbove(&sbPlayerPairAbove.data());
    }else if(sbPlayerPairAbove.empty()){
        sbPlayerPairBelow.data().setClosestAbove(nullptr);
    }
    else{
        sbPlayerPairAbove.data().setClosestBelow(nullptr);
    }
}



StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,int scoredGoals, int cardsReceived)
{
    // TODO: Your code goes here
    if(playerId <= 0 || gamesPlayed < 0 || scoredGoals < 0 || cardsReceived < 0){
        return StatusType::INVALID_INPUT;
    }
    Pair<Player,int>& playerPair = m_playersById.find(playerId);
    if(playerPair.empty()){
        return StatusType::FAILURE;
    }
    updateOnRemove_CP_SB(playerPair.data());
    AVLStatus checker = m_playersByGoals.remove(playerPair.data());
    if(checker != AVLStatus::AVL_Success){
        return StatusType::ALLOCATION_ERROR;
    }
    Player playerToUpdate = playerPair.data();
    playerToUpdate.updatePlayer(gamesPlayed, scoredGoals, cardsReceived);
    if(playerToUpdate.getGoals() > m_topScorer->getGoals()){
        this->updateTopScorer(&playerToUpdate);
    }
    checker = m_playersByGoals.insert(playerToUpdate, playerToUpdate);
    if(checker != AVLStatus::AVL_Success){
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
    // TODO: Your code goes here
    if(playerId <= 0){
        return (StatusType::INVALID_INPUT);
    }
    Pair<Player,int> playerPair = m_playersById.find(playerId);
    if(playerPair.empty()){
        return (StatusType::FAILURE);
    }
    Player& player = playerPair.data();
//    Team& teamPlayer = m_teamsTree.find(player.getTeamId()).data();
    return (playerPair.data().getIndividualGames());
}
///////////////////////////////////////////////////////////

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

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId) {
    // TODO: Your code goes here
    if (teamId1 <= 0 || teamId2 <= 0 || teamId2 == teamId1 || newTeamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    Pair<Team,int> teamPair1 = m_teamsTree.find(teamId1);
    Pair<Team,int> teamPair2 = m_teamsTree.find(teamId2);
    if (teamPair1.empty() || teamPair2.empty() || !m_teamsTree.find(newTeamId).empty()) {
        return StatusType::FAILURE;
    }
    Team& team1 = teamPair1.data();
    Team& team2 = teamPair2.data();
//    add_team(newTeamId, team1->getPoints() + team2->getPoints());
    AVLTree<Player, int> *unitePlayersById = mergeTrees(teamPair1.data().getPlayersByIdTree(), teamPair2.data().getPlayersByIdTree());
//    AVLTree<Player, int> *unitePlayersByGoals = mergeTrees(teamPair1.data().getPlayersTree(), teamPair2.data().getPlayersTree());

//    Pair<Player, int> **arrPlayersById = unitePlayers->inOrderScanToArray();
//    for (int i = 0; i < unitePlayers->size(); i++) {
//        arr[i]->data()->setTeamId(newTeamId);
//    }
//    Pair<Player, int> **arrPlayerByGoals = unitePlayers->inOrderScanToArray();
//    for (int i = 0; i < unitePlayers->size(); i++) {
//        arr[i]->data()->setTeamId(newTeamId);
//    }
//    Team* uniteTeamPtr = m_teamsTree.find(newTeamId)->data();
    Team united = mergeTeams(team1, team2, newTeamId);
    m_teamsTree.remove(teamId1);
    m_teamsTree.remove(teamId2);
    m_teamsTree.insert(united, newTeamId);
    //    uniteTeamPtr->updateTeam(*unitePlayers, totalGoals, totalCards, uniteGK, uniteTopScorer);
    return StatusType::SUCCESS;
}


output_t<int> world_cup_t::get_top_scorer(int teamId)
{
    // TODO: Your code goes here
    if(teamId == 0){
        return (StatusType::INVALID_INPUT);
    }
    if(m_numPlayers != 0){
        if(teamId < 0){
            return (m_topScorer->getPlayerId());
        }
        else{
            Pair<Team, int>& teamPair = m_teamsTree.find(teamId);
            if(teamPair.empty() || teamPair.data().getNumPlayers() == 0){
                return (StatusType::FAILURE);
            } else{
                return (teamPair.data().getTopScorer().getPlayerId());
            }
        }
    }else{
        return (StatusType::FAILURE);
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
        Pair<Player, Player>** arrOfPlayers = m_playersByGoals.inOrderScanToArray();
        for(int i = m_numPlayers - 1; i >= 0 ; i--){ //because demand the biggest first
            output[i] = arrOfPlayers[i]->data().getPlayerId();
        }
        return StatusType::SUCCESS;
    }
    else{
        Pair<Team, int> teamPair = m_teamsTree.find(teamId);
        if(teamPair.empty() || teamPair.data().getNumPlayers() <= 0){
            return StatusType::FAILURE;
        }
        Team teamPtr = teamPair.data();
        Pair<Player, Player>* arrOfPlayers = teamPtr.arrByGoals();
        for(int i = teamPtr.getNumPlayers() - 1; i >= 0 ; i--){ //because demand the biggest first
            output[i] = arrOfPlayers[i].data().getPlayerId();
        }
        return StatusType::SUCCESS;
    }
}




//////////////////////////////////////////////////////////////////
StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,int goals, int cards, bool goalKeeper)
{
    // TODO: Your code goes here
    if(playerId < 0 || teamId < 0 || goals < 0 || cards < 0 || gamesPlayed < 0 || ((goals > 0 || cards > 0) && gamesPlayed <=0)){
        return StatusType::INVALID_INPUT;
    }
    Pair<Team,int>& playerTeamPair = m_teamsTree.find(teamId);
    if(!m_playersById.find(playerId).empty() || playerTeamPair.empty()){
        return StatusType::FAILURE;
    }
    try{
        Player newPlayer = Player(playerId, teamId, gamesPlayed, goals, cards, goalKeeper, playerTeamPair.data().getTeamGames());
        AVLStatus checker1 = m_playersById.insert(newPlayer, playerId);
        AVLStatus checker2 = m_playersByGoals.insert(newPlayer, newPlayer);
        updateClosestPLayerOnScoreBoard(newPlayer);
        bool checker3 = playerTeamPair.data().insertPlayer(newPlayer);
        if(checker1 == AVLStatus::AVL_Success && checker2 == AVLStatus::AVL_Success && checker3){
            if(goalKeeper){
                playerTeamPair.data().changeGK(true);
            }
            if(playerTeamPair.data().isTeamValid()){
                Pair<Team, int>& teamPair = m_validTeamsTree.find(teamId);
                if(teamPair.empty())
                    m_validTeamsTree.insert(playerTeamPair.data(),  teamId);
                else{
                    teamPair.data().insertPlayer(newPlayer);
                }
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
        const Pair<Player, int> foundPlayer = m_playersById.find(playerId);
        if(foundPlayer.empty()){
            return StatusType::FAILURE;
        }
        Team& playerTeam = m_teamsTree.find(foundPlayer.data().getTeamId()).data();
//        Player* playerInGoals = m_playersByGoals.find(*playerInId)->data();
        AVLStatus checker2 = m_playersById.remove(playerId);
        bool checker3 = playerTeam.removePlayer(playerId);
        updateOnRemove_CP_SB(foundPlayer.data());
        AVLStatus checker1 = m_playersByGoals.remove(foundPlayer.data());
        if(foundPlayer.data().isGk()){
            playerTeam.changeGK(false);
        }
        if(!playerTeam.isTeamValid()){
            m_validTeamsTree.remove(playerTeam.getTeamId());
        }else{
            Team& validTeam = m_validTeamsTree.find(playerTeam.getTeamId()).data();
            validTeam.changeGK(false);
            validTeam.removePlayer(playerId);
        }
    }
    catch (...){
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
    if(teamId1 <= 0 || teamId2 <=0 || teamId1 == teamId2){
        return StatusType::INVALID_INPUT;
    }
    Pair<Team, int> team1Pair = m_teamsTree.find(teamId1);
    Pair<Team, int> team2Pair = m_teamsTree.find(teamId2);
    if(team1Pair.empty() || team2Pair.empty() || !team1Pair.data().isTeamValid() ||  !team2Pair.data().isTeamValid())
        return StatusType::FAILURE;
    Team& team1 = team1Pair.data();
    Team& team2 = team2Pair.data();
    if(team1.teamValue() > team2.teamValue()){
        team1.winGame();
        team2.loseGame();
    } else if(team1.teamValue() < team2.teamValue()){
        team1.loseGame();
        team2.winGame();
    }else{
        team1.tieGame();
        team2.tieGame();
    }
    return StatusType::SUCCESS;
}

StatusType world_cup_t::add_team(int teamId, int points)
{
    if(teamId <= 0 || points < 0){
        return StatusType::INVALID_INPUT;
    }
    if(!m_teamsTree.find(teamId).empty()){
        return StatusType::FAILURE;
    }
    // TODO: Your code goes here
    try {
        if(m_teamsTree.insert(Team(teamId, points),teamId) != AVLStatus::AVL_Success)
            return StatusType::ALLOCATION_ERROR;
        return StatusType::SUCCESS;
    }catch(...){
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType world_cup_t::remove_team(int teamId)
{
    // TODO: Your code goes here
    if(teamId <= 0){
        return StatusType::INVALID_INPUT;
    }
    Pair<Team,int>& teamToDelete = m_teamsTree.find(teamId);
    if(teamToDelete.empty() || !teamToDelete.data().empty()){
        return StatusType::FAILURE;
    }
    m_teamsTree.remove(teamId);
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    // TODO: Your code goes here
    if(teamId <= 0){
        return StatusType::INVALID_INPUT;
    }
    Pair<Team,int> teamPair = m_teamsTree.find(teamId);
    if(teamPair.empty()){
        return StatusType::FAILURE;
    }
    return teamPair.data().getPoints();
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
    // TODO: Your code goes here
    if(teamId == 0){
        return StatusType::INVALID_INPUT;
    }
    if(teamId < 0){
        return m_playersById.size();
    }else{
        Pair<Team,int>& teamPtr = m_teamsTree.find(teamId);
        if(teamPtr.empty()){
            return StatusType::FAILURE;
        } else{
            return teamPtr.data().getSize();
        }
    }
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
    // TODO: Your code goes here
    if(teamId <= 0 || playerId <= 0){
        return StatusType::INVALID_INPUT;
    }
    Pair<Team,int>& teamPair = m_teamsTree.find(teamId);
    Pair<Player,int>& playerPair = m_playersById.find(playerId);
    if(teamPair.empty() || playerPair.empty()) {
        return StatusType::FAILURE;
    }
//    int rankOfPlayer = m_playersByGoals.rank(playerPair.data());
//    Pair<Player,Player> option1 = m_playersByGoals.select(rankOfPlayer+1);
//    Pair<Player,Player> option2 = m_playersByGoals.select(rankOfPlayer-1);
//    if(option1.empty() && option2.empty()){
//        return StatusType::FAILURE;
//    }else if(option1.empty())
//        return option2.data().getPlayerId();
//    else if(option2.empty())
//        return option1.data().getPlayerId();
//    else if(option1.data().getPlayerId() > option2.data().getPlayerId()){
//        return option1.data().getPlayerId();
//    }else{
//        return option2.data().getPlayerId();
//    }
    Player* closest = playerPair.data().getClosest();
    return closest->getPlayerId();
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
    // TODO: Your code goes here
    if(minTeamId <= 0 || maxTeamId <= 0 || maxTeamId < minTeamId){
        return StatusType::INVALID_INPUT;
    }
    Pair<Team, int> **arrayOfLegalTeam;
    Pair<int, int> *teamValuesAndId;
    int sizeArr;
    try {
        arrayOfLegalTeam = m_validTeamsTree.inOrderScanToArrayFromTo(minTeamId, maxTeamId);
        if (!arrayOfLegalTeam) {
            return StatusType::FAILURE;
        }
        sizeArr = m_validTeamsTree.rank(maxTeamId) - m_validTeamsTree.rank(minTeamId) + 1;
        teamValuesAndId = new Pair<int, int>[sizeArr]();
    }catch(...){
        return StatusType::ALLOCATION_ERROR;
    }
    for(int i = 0; i < sizeArr; i++){ //good complexity cause sizeArr equal to r valid teams
        teamValuesAndId[i] = Pair<int, int>(arrayOfLegalTeam[i]->data().teamValue(), arrayOfLegalTeam[i]->key());
    }
    int roundSize = sizeArr;
    while(roundSize > 1) {
        int nextRoundTopIndex = 0;
        int nextRoundSize = 0;
        for (int j = 0; j < roundSize; j += 2) {
            if (j + 1 == roundSize) {
                teamValuesAndId[nextRoundTopIndex] = teamValuesAndId[j];
                nextRoundSize++;
                nextRoundTopIndex++;
                continue;
            }
            int winnerKey;
            if (teamValuesAndId[j].data() > teamValuesAndId[j + 1].data()) {
                winnerKey = teamValuesAndId[j].key();
            } else {
                winnerKey = teamValuesAndId[j + 1].key();
            }
            teamValuesAndId[nextRoundTopIndex] = Pair<int, int>(teamValuesAndId[j].data() + teamValuesAndId[j + 1].data() + 3, winnerKey);
            nextRoundSize++;
            nextRoundTopIndex++;
        }
        roundSize = nextRoundSize;
    }
    return (teamValuesAndId[0].key());
}