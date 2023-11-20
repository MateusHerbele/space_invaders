#ifndef __ENEMY__
#define __ENEMY__

#include <math.h>
#include "Pistol.h"

#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_image.h>

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

// Cria um inimigo
enemy* create_enemy(int x, int y, int enemy_type);
// Cria um array de inimigos

enemy** create_enemies(int n_enemies, int columns, int lines);
// Move o inimigo especial

void move_extra_enemy(enemy* extra_enemy, ALLEGRO_BITMAP* sprite_sheet);
// Move os inimigos

void update_enemies_position(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int max_x, unsigned short round);
// Faz o inimigo atirar

void enemy_shot(enemy *enemy);
// Retorna quantos inimigos estão vivos

int enemies_alive(enemy** enemies, int n_enemies);
// Libera a memória alocada para os inimigos e suas balas e armas

void free_enemies(enemy** enemies, int n_enemies);
// Verifica se algum inimigo atirou na coluna

int enemy_has_shot_column(int column, enemy** enemies, int n_entities);
// Verifica se algum inimigo está na frente de outro inimigo

int enemy_in_front_of_enemy(enemy* current_enemy, enemy** enemies, int n_enemies);
// Retorna a posição y do inimigo mais abaixo

int bottommost_enemy(enemy** enemies, int n_enemies);
// Evento do inimigo especial

void extra_enemy_event(enemy** enemies, int n_enemies);
// Retorna a distância entre dois pontos

int two_points_distance(int x1, int x2, int y1, int y2); 
// Atualiza os tiros dos inimigos

void update_enemies_shots(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int player_x, int player_y, unsigned short round);
// Desenha os tiros dos inimigos

void draw_enemies_bullets(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet);

#endif
