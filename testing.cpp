//
// Created by ohadr on 11/28/2022.
//
#include "Player.h"
#include <iostream>

int main(){

    Player kobe = Player(24, 8, 81, 5, 5, false);
    Player Mj = Player(23, 45, 63, 6, 5, false);
    std::cout << (kobe > Mj);

    return 0;
}
