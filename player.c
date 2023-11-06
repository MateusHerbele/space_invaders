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
    new_player->lifes = lifes;
    new_player->score = score;
    new_player->player_gun = player_gun;
    return new_player;
}
