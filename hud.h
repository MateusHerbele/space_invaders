#ifndef __HUD__
#define __HUD__

#include <stdio.h>

#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_image.h>


typedef struct {
    unsigned short position_x_lifes;
    unsigned short position_y_lifes;
    unsigned short position_x_score;
    unsigned short position_y_score;
    unsigned short width_lifes;
    unsigned short height_lifes;
} hud;

hud* create_hud();
void generate_hud(hud* hud, unsigned short player_lifes, ALLEGRO_BITMAP* sprite_sheet);


#endif // __HUD__