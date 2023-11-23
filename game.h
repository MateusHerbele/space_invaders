#ifndef __GAME__
#define __GAME__

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

#define X_SCREEN 640																																														//Definição do tamanho da tela em pixels no eixo x
#define Y_SCREEN 800	

// Atualiza a posição dos tiros do jogador e dos inimigos
void update_bullets(player *player, enemy** enemies, int n_enemies);
// Animação de explosão

void explosion_animation(int x, int y, ALLEGRO_BITMAP* sprite_sheet); 
// Checa se há colisão entre os tiros do jogador com inimigos ou obstáculos

void check_collision_player_shots(player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles, ALLEGRO_BITMAP* sprite_sheet);
// Checa se há colisão entre os tiros dos inimigos com o jogador ou obstáculos

void check_collision_enemy_shots(player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles, ALLEGRO_BITMAP* sprite_sheet);
// Checa se há colisão entre os tiros dos inimigos com os tiros do jogador

void check_collision_enemy_player(player* player, enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet);
// Rediciona para as funções de colisão

void check_collision(player *player, enemy **enemies, int n_enemies, obstacle** obstacles, int n_obstacles, ALLEGRO_BITMAP* sprite_sheet);
// Reseta as condições do jogo

void restart_conditions(unsigned short* round, player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles);
// Renderiza a tela de game over

void game_over_event(unsigned short* running, unsigned short* program_event, ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, unsigned short round, int score);
// Prepara os itens que são usados pra renderizar o menu e chama a função que renderiza ele

void menu_event(unsigned short* running, unsigned short* program_event, ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_BITMAP* sprite_sheet, ALLEGRO_FONT* font);
// Passa para o próximo round, aumentando o valor do round, a vida do jogador e reinciando os inimigos e obstáculos

void next_round(unsigned short* round, player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles);
// Realiza os evntos do jogo e renderiza seus elementos na tela

void game_event(short unsigned* running, unsigned short* program_event, unsigned short* round, player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles, hud* hud, ALLEGRO_FONT* font, ALLEGRO_BITMAP* sprite_sheet, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_DISPLAY* disp, ALLEGRO_EVENT event);
// Libera a memória alocada para os elementos do jogo

void free_all(player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles, hud* hud);
// Gera o jogo

void generating_game(unsigned short game_event, unsigned short round, player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles, hud* hud, ALLEGRO_FONT* font, ALLEGRO_BITMAP* sprite_sheet, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_DISPLAY* disp, ALLEGRO_EVENT event);

#endif // __GAME__