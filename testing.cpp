//
// Created by ohadr on 12/5/2022.
//

#include "worldcup23a1.h"

int main(){

    world_cup_t Qatar = world_cup_t();
    Qatar.add_team(1, 0);
    Qatar.add_team(2, 2);
    Qatar.add_team(3, 3);
    Qatar.add_player(10, 1, 1, 0, 0, true);
    Qatar.add_player(20, 2, 2, 0, 0, true);
    for(int i = 1; i < 12; i++){
        Qatar.add_player(10+i, 1, 0, 0, 0, false);
        Qatar.add_player(20+i, 2, 0, 0, 0, false);
    }
    Qatar.add_player(200, 2, 2, 0, 0, true);
    Qatar.update_player_stats(11, 0, 1, 0);
    Qatar.play_match(1,2);

    StatusType checkAddBad = Qatar.add_team(2,10);
    StatusType checkRemoveGood = Qatar.remove_team(3);
    StatusType checkRemoveBad = Qatar.remove_team(2);
    output_t<int> checkPlayedGames = Qatar.get_num_played_games(20); //=3
    output_t<int> checkPlayersNumTeam = Qatar.get_all_players_count(2); //=13
    output_t<int> checkPlayersNumAll = Qatar.get_all_players_count(-4); //=25
    output_t<int> checkGetClosest = Qatar.get_closest_player(20, 2); //=3

    return 0;
}