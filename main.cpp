//
// 234218 Data Structures 1.
// Semester: 2023A (winter).
// Wet Exercise #1.
//
// Recommended TAB size to view this file: 8.
//
// The following main file is necessary to link and run your code.
// This file is READ ONLY: even if you submit something else, the compiler ..
// .. WILL use our file.
//

#include "worldcup23a1.h"
#include <string>
#include <iostream>
#include "AVL_Map.h"

using namespace std;
//
//void print(string cmd, StatusType res);
//bool printelems(const string& a){
//  //  cout << "printelems:" << a << endl;
//    return true;
//}
//void print(string cmd, output_t<int> res);
//void query_get_all_players(string cmd, world_cup_t *obj, int teamID);
#define REQUIRE std::cout<<
#define SECTION printf
#define TEST_CASE printf
int main() {
    SECTION("simple update player stats\n");
    {
        world_cup_t *obj = new world_cup_t();
        StatusType res = obj->add_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, 3, 3, 3, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->update_player_stats(1, 1, 1, 1);
        REQUIRE(res == StatusType::SUCCESS);
        delete obj;
    }

    SECTION("\nupdate player stats with negative id\n");
    {
        world_cup_t *obj = new world_cup_t();
        StatusType res = obj->update_player_stats(-1, 1, 1, 1);
        REQUIRE(res == StatusType::INVALID_INPUT);
        delete obj;
    }

    SECTION("\nupdate player stats with 0 id\n");
    {
        world_cup_t *obj = new world_cup_t();
        StatusType res = obj->update_player_stats(0, 1, 1, 1);
        REQUIRE(res == StatusType::INVALID_INPUT);
        delete obj;
    }

    SECTION("\nupdate player stats with negative games\n");
    {
        world_cup_t *obj = new world_cup_t();
        StatusType res = obj->update_player_stats(1, -1, 1, 1);
        REQUIRE(res == StatusType::INVALID_INPUT);
        delete obj;
    }

    SECTION("\nupdate player stats with negative goals\n");
    {
        world_cup_t *obj = new world_cup_t();
        StatusType res = obj->update_player_stats(1, 1, -1, 1);
        REQUIRE(res == StatusType::INVALID_INPUT);
        delete obj;
    }

    SECTION("\nupdate player stats with negative cards\n");
    {
        world_cup_t *obj = new world_cup_t();
        StatusType res = obj->update_player_stats(1, 1, 1, -1);
        REQUIRE(res == StatusType::INVALID_INPUT);
        delete obj;
    }

    SECTION("\nupdate player stats of a player that doesn't exist\n");
    {
        world_cup_t *obj = new world_cup_t();
        StatusType res = obj->update_player_stats(1, 1, 1, 1);
        REQUIRE(res == StatusType::FAILURE);
        delete obj;
    }

    SECTION("\nupdate player stats of a player that was removed\n");
    {
        world_cup_t *obj = new world_cup_t();
        StatusType res = obj->add_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, 3, 3, 3, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_player(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->update_player_stats(1, 1, 1, 1);
        REQUIRE(res == StatusType::FAILURE);
        delete obj;
    }

    SECTION("\nupdate player stats of a player that was removed and added again\n");
    {
        world_cup_t *obj = new world_cup_t();
        StatusType res = obj->add_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, 3, 3, 3, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->remove_player(1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, 3, 3, 3, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->update_player_stats(1, 1, 1, 1);
        REQUIRE(res == StatusType::SUCCESS);
        delete obj;
    }

    SECTION("\nupdate twice player stats\n");
    {
        world_cup_t *obj = new world_cup_t();
        StatusType res = obj->add_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1, 1, 3, 3, 3, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->update_player_stats(1, 1, 1, 1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->update_player_stats(1, 1, 1, 1);
        REQUIRE(res == StatusType::SUCCESS);
        delete obj;
    }

    SECTION("\nadd teams and players - segel test\n");
    {
        world_cup_t *obj = new world_cup_t();
        StatusType res = obj->add_team(1, 10000);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2, 20000);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(3, 30000);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(4, 40000);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1003, 1, 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1004, 1, 10, 0, 2, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1005, 1, 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1006, 1, 10, 4, 3, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1007, 1, 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1008, 1, 10, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1009, 1, 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1010, 1, 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1011, 1, 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1012, 1, 10, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(2001, 2, 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2002, 2, 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2003, 2, 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2004, 2, 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2005, 2, 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2006, 2, 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2007, 2, 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2008, 2, 20, 6, 4, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2009, 2, 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2010, 2, 0, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(2011, 2, 20, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(3001, 3, 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3002, 3, 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3003, 3, 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3004, 3, 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3005, 3, 30, 2, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3006, 3, 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3007, 3, 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3008, 3, 30, 0, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3009, 3, 30, 0, 2, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3010, 3, 30, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(3011, 3, 30, 0, 0, true);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->add_player(4001, 4, 2, 1, 2, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(4002, 4, 2, 2, 2, false);
        REQUIRE(res == StatusType::SUCCESS);

        res = obj->update_player_stats(3001, 2, 1, 1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->update_player_stats(3011, 2, 1, 1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->update_player_stats(1001, 2, 1, 1);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->update_player_stats(4001, 2, 1, 1);
        REQUIRE(res == StatusType::SUCCESS);
        // play_match 2 3 play_match 1 2 get_num_played_games 2003 get_team_points 3 unite_teams 1 2 2 get_top_scorer - 1 get_all_players_count 3 get_all_players 4 get_closest_player 2008 2 knockout_winner 0 3 remove_team 2 remove_player 3008

        delete obj;
    }
}
