#ifndef PLAYER_H
#define PLAYER_H

#include "guns.h"

typedef struct player{
	int position_x;
	int position_y;
    short unsigned int lifes;
    unsigned int score;
    // unsignet int power_ups;
    // unsigned int power_ups_time;
    gun* player_gun;
} player;

player* create_player(int x, int y, int lifes, int score, gun* player_gun);

#endif // PLAYER_H