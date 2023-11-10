#include <stdlib.h>
#include <stdio.h>

#include "player.h"

player* create_player(int x, int y){
    player* new_player = (player*) malloc(sizeof(player));
    if(new_player == 0){
        fprintf(stderr, "ERRO: não foi possível alocar memória para o jogador!\n");
        exit(1);
    }
    new_player->position_x = x;
    new_player->position_y = y;
    new_player->lifes = 3;
    new_player->sprite_x = 0;
    new_player->sprite_y = 0;    
    new_player->gun = pistol_create();
    new_player->control = joystick_create();
    return new_player;
}
void player_move(player *player, short trajectory, unsigned short max_x){									//Implementação da função "player_move"
    if ((player->position_x + (STEPS* trajectory) -16 > 0) && (player->position_x + (STEPS * trajectory) + 16  < max_x)) 
        player->position_x = player->position_x + (STEPS * trajectory);
} 

void player_shot(player *player){
    bullet* shot;
    shot = pistol_shot(player->position_x, player->position_y - 16, -1, player->gun);										//Quadrado atira para a esquerda (!)
	player->gun->shots = shot;
}