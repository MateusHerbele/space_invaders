#include <stdlib.h>
#include <stdio.h>

#include "player.h"

player* create_player(int x, int y, int lifes, int score, gun* player_gun){
    player* new_player = (player*) malloc(sizeof(player));
    if(new_player == 0){
        fprintf(stderr, "ERRO: não foi possível alocar memória para o jogador!\n");
        exit(1);
    }
    new_player->position_x = x;
    new_player->position_y = y;
    new_player->velocity_x = 0;
    new_player->velocity_y = 0;
    new_player->lifes = lifes;
    new_player->sprite_x = 0;
    new_player->sprite_y = 0;    
    new_player->player_gun = player_gun;
    new_player->control = joystick_create();
    return new_player;
}
void player_move(player *player, short trajectory, unsigned short max_x){									//Implementação da função "player_move"
    if ((player->position_x + (STEPS* trajectory)) && (player->position_x + (STEPS * trajectory) < max_x)) 
        player->position_x = player->position_x + (STEPS * trajectory);
} 