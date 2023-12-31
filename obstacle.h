#ifndef __OBSTACLE__
#define __OBSTACLE__

#include <stdio.h>
#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																											//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>	
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_color.h>
#include <allegro5/timer.h>		

typedef struct {
    unsigned short position_x;
    unsigned short position_y;
    short lives;
} obstacle;

// Cria um obstáculo
obstacle* obstacle_create(int x, int y);
// Cria um array de obstáculos

obstacle** create_obstacles(int n_obstacles, int max_x, int max_y);
// Gera os obstáculos na tela

void generate_obstacles(obstacle** array, int n_obstacles, ALLEGRO_BITMAP* sprite_sheet);
// Restaura a vida dos obstáculos

void restore_obstacles(obstacle** obstacle, int n_obstacles);
// Libera o espaço alocado para o obstáculo

void free_obstacles(obstacle** obstacles, int n_obstacles);

#endif // __OBSTACLE__