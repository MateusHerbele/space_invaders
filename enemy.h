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

typedef struct frames{
	int x1;
	int y1;
	int x2;
	int y2;
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
void move_extra_enemy(enemy* extra_enemy, ALLEGRO_BITMAP* sprite_sheet);
void update_enemies_position(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int max_x, unsigned short round);
void enemy_shot(enemy *enemy);
int enemies_alive(enemy** enemies, int n_enemies);
void free_enemies(enemy** enemies, int n_enemies);
int enemy_has_shot_column(int column, enemy** enemies, int n_entities);
int enemy_in_front_of_enemy(enemy* current_enemy, enemy** enemies, int n_enemies);
int bottommost_enemy(enemy** enemies, int n_enemies);
void extra_enemy_event(enemy** enemies, int n_enemies);
#endif
