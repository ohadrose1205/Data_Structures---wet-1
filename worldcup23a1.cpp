#include "worldcup23a1.h"
#include "Team.h"

world_cup_t::world_cup_t() : m_teamsTree(),
                             m_validTeamsTree(),
                             m_playersById(),
                             m_playersByGoals()
{
    // TODO: Your code goes here
    m_topScorer = nullptr;
    m_totalGames = 0;

}

world_cup_t::~world_cup_t()
{
    // TODO: Your code goes here
    ///as default
}
void world_cup_t::updateOnInsert_CP_SB(const Player& keyPlayer){
    Player* sbPlayer = m_playersByGoals.find(keyPlayer).data();
    int scoreBoardRank = m_playersByGoals.rank(*sbPlayer);
    Pair<Player*,Player> sbPlayerPairAbove =  m_playersByGoals.select(scoreBoardRank+1);
    Pair<Player*,Player> sbPlayerPairBelow =  m_playersByGoals.select(scoreBoardRank-1);
    if(sbPlayerPairAbove.empty()){
        sbPlayer->setClosestAbove(nullptr);
        m_topScorer = sbPlayer;
    }else{
        sbPlayer->setClosestAbove(sbPlayerPairAbove.data());
        sbPlayerPairAbove.data()->setClosestBelow(sbPlayer);
    }
    if(sbPlayerPairBelow.empty()){
        sbPlayer->setClosestBelow(nullptr);
    }else{
        sbPlayer->setClosestBelow(sbPlayerPairBelow.data());
        sbPlayerPairBelow.data()->setClosestAbove(sbPlayer);
    }
}

void world_cup_t::updateOnRemove_CP_SB(const Player& keyPlayer){
    Player* betterPlayer = keyPlayer.getClosestAbove();
    Player* worsePlayer = keyPlayer.getClosestBelow();
    if(!betterPlayer && !worsePlayer){
        m_topScorer = nullptr;
        return;
    }else if(!betterPlayer){
        worsePlayer->setClosestAbove(nullptr);
        m_topScorer = worsePlayer;
    }else if(!worsePlayer){
        betterPlayer->setClosestBelow(nullptr);
    }else{
        betterPlayer->setClosestBelow(worsePlayer);
        worsePlayer->setClosestAbove(betterPlayer);
    }
}




/////////////////////////////////////////////////////////////////////////////////////

StatusType world_cup_t::add_team(int teamId, int points)
{
    if(teamId <= 0 || points < 0){
        return StatusType::INVALID_INPUT;
    }
    return m_teamsTree.insert(Team(teamId, points),teamId);
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

StatusType world_cup_t::add_playerAUX(const Player& newPlayer){
    StatusType checker1 = m_playersById.insert(newPlayer, newPlayer.getPlayerId());
    if (checker1 == StatusType::SUCCESS) {
        Player* newPlayerPtr = &m_playersById.find(newPlayer.getPlayerId()).data();
        StatusType checker2 = m_playersByGoals.insert(newPlayerPtr, newPlayer);
        if (checker2 == StatusType::SUCCESS) {
            StatusType checker3 = newPlayer.getTeam()->insertPlayer(newPlayerPtr, newPlayer.getPlayerId());
            if(checker3 == StatusType::SUCCESS){
                updateOnInsert_CP_SB(newPlayer);
            }
            return checker3;
        }
        return checker2;
    }
    return checker1;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,int goals, int cards, bool goalKeeper)
{

    // TODO: Your code goes here
    if(playerId <= 0 || teamId <= 0 || goals < 0 || cards < 0 || gamesPlayed < 0 || ((goals > 0 || cards > 0) && gamesPlayed <=0)){
        return StatusType::INVALID_INPUT;
    }
    Pair<Team,int>& playerTeamPair = m_teamsTree.find(teamId);
    if(playerTeamPair.empty()){
        return StatusType::FAILURE;
    }
    StatusType checker1 = StatusType::SUCCESS;
    Player newPlayer = Player(playerId, &playerTeamPair.data(), gamesPlayed, goals, cards, goalKeeper);
    checker1 = add_playerAUX(newPlayer);
    if (checker1 == StatusType::SUCCESS) {
        if(playerTeamPair.data().isTeamValid()){
            m_validTeamsTree.insert(&playerTeamPair.data(), teamId);
        }
    }
    return checker1;
}

StatusType world_cup_t::remove_player(int playerId){
    if(playerId <= 0){
        return StatusType::INVALID_INPUT;
    }
    const Pair<Player, int> foundPlayer = m_playersById.find(playerId);
    if(foundPlayer.empty()){
        return StatusType::FAILURE;
    }
    StatusType checker1 = remove_playerAUX(foundPlayer.data());
    if(checker1 == StatusType::SUCCESS) {
        updateOnRemove_CP_SB(foundPlayer.data());
        if (!foundPlayer.data().getTeam()->isTeamValid()) {
            m_validTeamsTree.remove(foundPlayer.data().getTeam()->getTeamId());
        }
    }
    return checker1;
}
StatusType world_cup_t::remove_playerAUX(const Player& pUpdate){
    StatusType checker1 = pUpdate.getTeam()->removePlayer(pUpdate.getPlayerId());
    if(checker1 == StatusType::SUCCESS){
        updateOnRemove_CP_SB(pUpdate);
        StatusType checker2 = m_playersByGoals.remove(pUpdate);
        if(checker2 == StatusType::SUCCESS){
            StatusType checker3 = m_playersById.remove(pUpdate.getPlayerId());
            if(checker3 ==  StatusType::SUCCESS) {
                return StatusType::SUCCESS;
            }
            return checker3;
        }
        return checker2;
    }
    return checker1;
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
    Player playerBeforeUpdate = playerPair.data();
    StatusType checker1 = remove_playerAUX(playerBeforeUpdate);
    if(checker1 != StatusType::SUCCESS){
        add_playerAUX(playerBeforeUpdate);
        return StatusType::ALLOCATION_ERROR;
    }
    Player playerAfterUpdate = playerBeforeUpdate;
    playerAfterUpdate.updatePlayer(gamesPlayed, scoredGoals, cardsReceived);
    checker1 = add_playerAUX(playerAfterUpdate);
    if(checker1 == StatusType::SUCCESS){
        return StatusType::SUCCESS;
    }else{
        add_playerAUX(playerBeforeUpdate);
        return checker1;
    }

}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
    if(teamId1 <= 0 || teamId2 <=0 || teamId1 == teamId2){
        return StatusType::INVALID_INPUT;
    }
    Pair<Team*, int> team1Pair = m_validTeamsTree.find(teamId1);
    Pair<Team*, int> team2Pair = m_validTeamsTree.find(teamId2);
    if(team1Pair.empty() || team2Pair.empty())
        return StatusType::FAILURE;
    Team& team1 = *team1Pair.data();
    Team& team2 = *team2Pair.data();
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
    return (playerPair.data().getIndividualGames());
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    // TODO: Your code goes here
    if(teamId <= 0){
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    Pair<Team,int> teamPair = m_teamsTree.find(teamId);
    if(teamPair.empty()){
        return output_t<int>(StatusType::FAILURE);
    }
    return output_t<int>( teamPair.data().getPoints());
}
void destroyTeam(Pair<Team,int>& team){

}
StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId) {
    // TODO: Your code goes here
    if (teamId1 <= 0 || teamId2 <= 0 || teamId2 == teamId1 || newTeamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    Pair<Team,int> teamPair1 = m_teamsTree.find(teamId1);
    Pair<Team,int> teamPair2 = m_teamsTree.find(teamId2);
    if (teamPair1.empty() || teamPair2.empty() || (newTeamId != teamId1 && newTeamId != teamId2 && !m_teamsTree.find(newTeamId).empty())){
        return StatusType::FAILURE;
    }
    Team* newTeam = ::mergeTeams(teamPair1.data(),teamPair2.data(),newTeamId);
    if(newTeam){
        m_teamsTree.remove(teamId1);
        m_teamsTree.remove(teamId2);
        m_validTeamsTree.remove(teamId1);
        m_validTeamsTree.remove(teamId2);
        StatusType checker1 = m_teamsTree.insert(*newTeam, newTeamId);
        if (checker1 == StatusType::SUCCESS) {
            if(newTeam->isTeamValid()){
                StatusType checker2 = m_validTeamsTree.insert(&m_teamsTree.find(newTeamId).data(), newTeamId);
                if(checker2 == StatusType::SUCCESS){
                    delete newTeam;
                    newTeam = nullptr;
                    return StatusType::SUCCESS;
                }
                delete newTeam;
                newTeam = nullptr;
                return checker2;
            }
        }
        delete newTeam;
        newTeam = nullptr;
        return checker1;
    }
    return StatusType::ALLOCATION_ERROR;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
    // TODO: Your code goes here
    if(teamId == 0){
        return StatusType::INVALID_INPUT;
    }
    if(teamId > 0) {
        Pair<Team,int> teamPair = m_teamsTree.find(teamId);
        if(teamPair.empty() || teamPair.data().empty())
            return StatusType::FAILURE;
        return teamPair.data().getTopScorer()->getPlayerId();
    }else {
        if(m_playersById.empty())
            return StatusType::FAILURE;
        return m_topScorer->getPlayerId();
    }
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
            return teamPtr.data().getTeamSize();
        }
    }
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
    if(teamId == 0 || output == nullptr){
        return StatusType::INVALID_INPUT;
    }
    Pair<Team,int>& teamPair = m_teamsTree.find(teamId);
    if((teamId < 0 && m_playersById.empty()) || (teamId > 0 && (teamPair.empty() || teamPair.data().empty())))
        return StatusType::FAILURE;
    if(teamId < 0 ){
        Pair<Player*, Player>** arrOfPlayers = m_playersByGoals.inOrderScanToArray();
        for(int i = m_playersByGoals.size() - 1; i >= 0 ; i--){ //because demand the biggest first
            output[i] = arrOfPlayers[i]->data()->getPlayerId();
        }
        delete[] arrOfPlayers;
        return StatusType::SUCCESS;
    }else{
        Pair<Player*, Player>** arrOfPlayers = teamPair.data().arrByGoals();
        for(int i = teamPair.data().getTeamSize() - 1; i >= 0 ; i--){ //because demand the biggest first
            output[i] = arrOfPlayers[i]->data()->getPlayerId();
        }
        delete[] arrOfPlayers;
        return StatusType::SUCCESS;
    }
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
    // TODO: Your code goes here
    if(teamId <= 0 || playerId <= 0){
        return StatusType::INVALID_INPUT;
    }
    Pair<Team,int>& teamPair = m_teamsTree.find(teamId);
    if(teamPair.empty() || m_playersById.size() <= 1) {
        return StatusType::FAILURE;
    }
    Player* player = teamPair.data().findPlayer(playerId);
    if(!player){
        return StatusType::FAILURE;
    }
    int winner = player->getClosest()->getPlayerId();
    return winner;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
    // TODO: Your code goes here
    if(minTeamId < 0 || maxTeamId < 0 || maxTeamId < minTeamId){
        return StatusType::INVALID_INPUT;
    }
    Pair<Team*, int> **arrayOfLegalTeam;
    Pair<int, int> *teamValuesAndId;
    int sizeArr = 0;
    try {
        arrayOfLegalTeam = m_validTeamsTree.inOrderScanToArrayFromTo(sizeArr, minTeamId, maxTeamId);
        if (!arrayOfLegalTeam) {
            return StatusType::FAILURE;
        }
        teamValuesAndId = new Pair<int, int>[sizeArr]();
    }catch(...){
        return StatusType::ALLOCATION_ERROR;
    }
    for(int i = 0; i < sizeArr; i++){ //good complexity cause sizeArr equal to r valid teams
        teamValuesAndId[i] = Pair<int, int>(arrayOfLegalTeam[i]->data()->teamValue(), arrayOfLegalTeam[i]->key());
    }
    delete[]arrayOfLegalTeam;
    int roundSize = sizeArr;
    while(roundSize > 1) {
        int nextRoundSize = 0;
        for (int j = 0; j < roundSize; j += 2) {
            if (j + 1 == roundSize) {
                teamValuesAndId[nextRoundSize] = teamValuesAndId[j];
                nextRoundSize++;
                continue;
            }
            int winnerKey;
            if (teamValuesAndId[j].data() > teamValuesAndId[j + 1].data()) {
                winnerKey = teamValuesAndId[j].key();
            } else {
                winnerKey = teamValuesAndId[j + 1].key();
            }
            teamValuesAndId[nextRoundSize] = Pair<int, int>(teamValuesAndId[j].data() + teamValuesAndId[j + 1].data() + 3, winnerKey);
            nextRoundSize++;
        }
        roundSize = nextRoundSize;
    }
    int winner = teamValuesAndId[0].key();
    delete[]teamValuesAndId;
    return (winner);
}

