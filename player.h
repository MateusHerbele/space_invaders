#ifndef PLAYER_H
#define PLAYER_H

#include "Pistol.h"
#include "Joystick.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#define STEPS 5

typedef struct player{
	int position_x;
	int position_y;
    short unsigned lives;
    short unsigned max_lives;
    int sprite_x;
    int sprite_y;
    int score; 
    joystick* control;
    pistol* gun;
} player;

// Cria o player e seus componentes
player* create_player(int x, int y);
// Move o player

void player_move(player *player, short trajectory, unsigned short max_x);
// Atira

void player_shot(player *player);
// Libera o espaço alocado para o player e seus componentes

void free_player(player* player);
// Atualiza a pontuação do player

void player_score(int* score, int enemy_type);
// Atualiza a posição do player e desenha ele na tela

void update_player_position(player *player, ALLEGRO_BITMAP* sprite_sheet);
// Desenha os tiros do player

void draw_player_bullets(player* player, ALLEGRO_BITMAP* sprite_sheet); 

#endif // PLAYER_H