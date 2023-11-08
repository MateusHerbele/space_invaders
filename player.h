#ifndef PLAYER_H
#define PLAYER_H

#include "Pistol.h"
#include "Joystick.h"

#define STEPS 10

typedef struct player{
	int position_x;
	int position_y;
    int velocity_x; // juntar as duas
    int velocity_y;
    short unsigned int lifes;
    int sprite_x;
    int sprite_y; 
    // unsignet int power_ups;
    // unsigned int power_ups_time;
    joystick* control;
    pistol* gun;
} player;

player* create_player(int x, int y, int lifes, int score);
void player_move(player *player, short trajectory, unsigned short max_x);							//Implementação da função "player_move"
void player_shot(player *player);

#endif // PLAYER_H