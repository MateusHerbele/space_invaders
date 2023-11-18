#ifndef __GAME__
#define __GAME__

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
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
#include "menu.h"


void update_bullets(player *player, enemy** enemies, int n_enemies);
void update_player_position(player *player, ALLEGRO_BITMAP* sprite_sheet);
void explosion_animation(int x, int y, ALLEGRO_BITMAP* sprite_sheet); 
void player_score(int* score, int enemy_type);
void check_collision(player *player, enemy **enemies, int n_enemies, obstacle** obstacles, int n_obstacles, ALLEGRO_BITMAP* sprite_sheet);
int two_points_distance(int x1, int x2, int y1, int y2);
void update_enemies_shots(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int player_x, int player_y, int round);
void draw_player_bullets(player* player, ALLEGRO_BITMAP* sprite_sheet);
void draw_enemies_bullets(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet);
void game_event(short unsigned* running, int round, player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles, hud* hud, ALLEGRO_FONT* font, ALLEGRO_BITMAP* sprite_sheet, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_DISPLAY* disp, ALLEGRO_EVENT event);
void generating_game(int game_event, int round, player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles, hud* hud, ALLEGRO_FONT* font, ALLEGRO_BITMAP* sprite_sheet, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_DISPLAY* disp, ALLEGRO_EVENT event);



#define X_SCREEN 640																																														//Definição do tamanho da tela em pixels no eixo x
#define Y_SCREEN 800		

#endif // __GAME__