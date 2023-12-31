#ifndef __HUD__
#define __HUD__

#include <stdio.h>

#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>		
#include <allegro5/allegro_primitives.h>																																										//Biblioteca de fontes do Allegro

#include "player.h"

typedef struct {
    unsigned short position_x_lives;
    unsigned short position_y_lives;
    unsigned short position_x_score;
    unsigned short position_y_score;
    unsigned short width_lives;
    unsigned short height_lives;
} hud;

// Cria a hud
hud* create_hud();
// Gera a hud na tela
void generate_hud(hud* hud, player* player, unsigned short round, ALLEGRO_BITMAP* sprite_sheet, ALLEGRO_FONT* font);

#endif // __HUD__