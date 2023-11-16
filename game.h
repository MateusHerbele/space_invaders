#ifndef __GAME__
#define __GAME__

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																											//Biblioteca de fontes do Allegro
#include <allegro5/allegro_ttf.h>																																							//Biblioteca de primitivas do Allegro
#include <allegro5/allegro_primitives.h>	
#include <allegro5/allegro_image.h>
#include <allegro5/timer.h>				

#include "enemy.h"
#include "player.h"
#include "Joystick.h"
#include "Pistol.h"
#include "Bullet.h"
#include "obstacle.h"
#include "hud.h"


void update_bullets(player *player, enemy** enemies, int n_enemies);
void update_position(player *player, enemy** enemies, int n_enemies);
void explosion_animation(int x, int y, ALLEGRO_BITMAP* sprite_sheet); 
void player_score(int* score, int enemy_type);
void check_collision(player *player, enemy **enemies, int n_enemies, obstacle** obstacles, int n_obstacles, ALLEGRO_BITMAP* sprite_sheet);
int two_points_distance(int x1, int x2, int y1, int y2);
void update_enemies_shots(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int player_x, int player_y, int round);



#define X_SCREEN 640																																														//Definição do tamanho da tela em pixels no eixo x
#define Y_SCREEN 800		

#endif // __GAME__