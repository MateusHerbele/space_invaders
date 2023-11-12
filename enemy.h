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

typedef struct frames{
	int x1;
	int y1;
	int x2;
	int y2
} frames;

typedef struct enemy{
	int alive;
	int position_x;
	int position_y;
	frames sprite;
	int direction;
	int type;
	pistol* gun;
} enemy;

enemy* create_enemy(int x, int y, int enemy_type);
enemy** create_enemies(int n_enemies, int columns, int lines);
// void generate_enemies(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int max_x);
void update_enemies_position(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int max_x);
void remove_enemy(enemy** enemies, int n_enemies, enemy* enemy);

#endif
