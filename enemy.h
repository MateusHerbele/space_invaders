#ifndef __ENEMY__
#define __ENEMY__

#include "space.h"
#include "Pistol.h"
#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																											//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>	
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_color.h>
#include <allegro5/timer.h>				

typedef struct shot shot;
struct shot{
	int position_x;
	int position_y;
	int trajectory;
	shot *next;
} ;

typedef struct shot_sentinel{
	shot *first;
	shot *last;
} shot_sentinel;

typedef struct enemy{
	int position_x;
	int position_y;
	int sprite_x;
    int sprite_y;
	int direction;
	pistol* gun;
} enemy;

enemy* create_enemy(int x, int y);
enemy** create_enemies(int n_enemies, int columns, int lines);
void generate_enemies(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int max_x);
void update_enemies_position(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int max_x);
int remove_enemy(space *board, int position_y, int position_x);

#endif
