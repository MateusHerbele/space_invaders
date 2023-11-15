#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																											//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>	
#include <allegro5/allegro_image.h>
#include <allegro5/timer.h>				
// #include <allegro5/allegro_ttf.h>																																							//Biblioteca de primitivas do Allegro

#include "enemy.h"
#include "player.h"
#include "Joystick.h"
#include "Pistol.h"
#include "Bullet.h"
#include "obstacle.h"
#include "hud.h"

#define X_SCREEN 640																																														//Definição do tamanho da tela em pixels no eixo x
#define Y_SCREEN 800		

void update_bullets(player *player, enemy** enemies, int n_enemies){																																										//Implementação da função que atualiza o posicionamento de projéteis conforme o movimento dos mesmos (!)
	bullet *previous = NULL;																																												//Variável auxiliar para salvar a posição imediatamente anterior na fila (!)
	for (bullet *index_player = player->gun->shots; index_player != NULL;){																																				//Para cada projétil presente na lista de projéteis disparados (!)
		index_player->y -= BULLET_MOVE;																																											//Atualiza a posição do projétil (!)
		if ((index_player->y < 0) || (index_player->y > Y_SCREEN)){																																						//Verifica se o projétil saiu das bordas da janela (!)
			if (previous){																																													//Verifica se não é o primeiro elemento da lista de projéteis (!)
				previous->next = index_player->next;																																								//Se não for, salva o próximo projétil (!)
				bullet_destroy(index_player);																																										//Chama o destrutor para o projétil atual (!)
				index_player = (bullet*) previous->next;																																							//Atualiza para o próximo projétil (!)
			}
			else {																																															//Se for o primeiro projétil da lista (!)
				player->gun->shots = (bullet*) index_player->next;																																					//Atualiza o projétil no início da lista (!)
				bullet_destroy(index_player);																																										//Chama o destrutor para o projétil atual (!)
				index_player = player->gun->shots;																																									//Atualiza para o próximo projétil (!)
			}
		}
		else{																																																//Se não saiu da tela (!)
			previous = index_player;																																												//Atualiza o projétil anterior (para a próxima iteração) (!)
			index_player = (bullet*) index_player->next;																																									//Atualiza para o próximo projétil (!)
		}
	}

	previous = NULL;
	for(int i = 0; i < n_enemies; i++){
		for(bullet* index_enemies = enemies[i]->gun->shots; index_enemies != NULL;){
			index_enemies->y += BULLET_MOVE;
			if((index_enemies->y < 0) || (index_enemies->y > Y_SCREEN)){
				if(previous){
					previous->next = index_enemies->next;
					// bullet_destroy(index_enemies);
					index_enemies = (bullet*) previous->next;
				}else{
					enemies[i]->gun->shots = (bullet*) index_enemies->next;
					// bullet_destroy(index_enemies);
					index_enemies = enemies[i]->gun->shots;
				}
			}
			else{
				previous = index_enemies;
				index_enemies = (bullet*) index_enemies->next;
			}
		}
	}
}
// update position
void update_position(player *player, enemy** enemies, int n_enemies){																																					//Função de atualização das posições dos quadrados conforme os comandos do controle
	if (player->control->left){																																											//Se o botão de movimentação para esquerda do controle do primeiro jogador está ativado...
		player_move(player, -1, X_SCREEN);		
		if(player->sprite_x < 48) player->sprite_x += 16;																																		//Move o quadrado do primeiro jogador para a esquerda
	}
	else 
	if (player->control->right){																																											//Se o botão de movimentação para direita do controle do primeir ojogador está ativado...
		player_move(player, 1, X_SCREEN);	
		if(player->sprite_x < 48) player->sprite_x += 16;																																		//Move o quadrado do primeiro jogador para a esquerda
	
	}
	else
		player->sprite_x = 0;
	
	if(!has_shot_column(player->gun->shots, player->position_x) && player->control->fire){
		if(!player->gun->timer){
			player_shot(player);
			player->gun->timer = PISTOL_COOLDOWN;
		}	
	}
	if (player->control->fire){																																											//Verifica se o primeiro jogador está atirando (!)
		if (!player->gun->timer){																																											//Verifica se a arma do primeiro jogador não está em cooldown (!)																																										//Se não estiver, faz um disparo (!)
			player->gun->timer = PISTOL_COOLDOWN;																																							//Inicia o cooldown da arma (!)
		} 
	}
	update_bullets(player, enemies, n_enemies);			
																																									//Atualiza os disparos do primeiro jogador (!)
}

void explosion_animation(int x, int y, ALLEGRO_BITMAP* sprite_sheet){
	int sprite_width = 16;
	int sprite_height = 16;
	int sprite_x = 32;
	int sprite_y = 64;
	
	for(int i = 0; i < 11; i++){
		al_draw_scaled_bitmap(sprite_sheet, sprite_x, sprite_y, sprite_width, sprite_height, x , y , sprite_width * 3, sprite_height * 3, 0);		
		sprite_x += 16;
		if(i == 4){
			sprite_x = 0;
			sprite_y += 16;
		}
	}
}

// void clean_shots(pistol* gun){
//     bullet* index = gun->shots;
//     bullet* next = NULL;

//     while (index != NULL){
//         if (index->y < 0){
//             next = (bullet*)index->next;
//             free(index);
//             index = next;
//         } else {
//             index = (bullet*)index->next;
//         }
//     }

//     //gun->shots = NULL;  // Atualize o ponteiro de tiros após a remoção
// }


// void check_collision(player *player, enemy **enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet) {
//     // player bullets
//     if (player->gun->shots == NULL) return;

//     bullet *current_bullet = player->gun->shots;
//     bullet *previous_bullet = NULL;

//     while (current_bullet != NULL) {
//         int hit = 0;

//         for (int i = 0; i < n_enemies; i++) {
//             if (enemies[i]->alive &&
//                 current_bullet->x >= enemies[i]->position_x - 16 &&
//                 current_bullet->x <= enemies[i]->position_x + 16 &&
//                 current_bullet->y >= enemies[i]->position_y - 16 &&
//                 current_bullet->y <= enemies[i]->position_y + 16) {
//                 enemies[i]->alive = 0;
//                 explosion_animation(enemies[i]->position_x, enemies[i]->position_y, sprite_sheet);
//                 hit = 1;
//                 break;
//             }
//         }

//         if (hit) {
//             bullet *temp = current_bullet;
//             current_bullet = current_bullet->next;

//             if (previous_bullet) {
//                 previous_bullet->next = current_bullet;
//             } else {
//                 player->gun->shots = current_bullet;
//             }

//             bullet_destroy(temp);
//         } else {
//             previous_bullet = current_bullet;
//             current_bullet = current_bullet->next;
//         }
//     }
// }

void player_score(int* score, int enemy_type){
	int extra_multiplyer = 1;
	switch(enemy_type){
		case 0:
			*score += 10;
		break;
		case 1:
			*score += 20;
		break;
		case 2:
			*score += 40;
		break;
		case 3:
			*score += 100;
			if(extra_multiplyer < 3)
				extra_multiplyer += 1;
		break;
	}
}

void check_collision(player *player, enemy **enemies, int n_enemies, obstacle** obstacles, int n_obstacles, ALLEGRO_BITMAP* sprite_sheet){
	// player bullets
	//if(player->gun->shots == NULL) return;
	for(bullet* index = player->gun->shots; index != NULL; index = (bullet*) index->next){
		for(int i = 0; i < n_enemies; i++){
			if(enemies[i]->alive){
				if(index->x >= enemies[i]->position_x - 16 && index->x <= enemies[i]->position_x + 16){
					if(index->y >= enemies[i]->position_y - 16 && index->y <= enemies[i]->position_y + 16){
						enemies[i]->alive = 0;
						explosion_animation(enemies[i]->position_x - 10, enemies[i]->position_y - 10, sprite_sheet);
						index->y = -1;
						// bullet_destroy(index);
						player_score(&player->score, enemies[i]->type);
						printf("score: %d\n", player->score);
						break;
					}
				}
			}
		}
		// obstacles
		for(int i = 0; i < n_obstacles; i++){
			if(index->x >= obstacles[i]->position_x && index->x <= obstacles[i]->position_x + 64){
				if(index->y >= obstacles[i]->position_y && index->y <= obstacles[i]->position_y + 16){
					if(obstacles[i]->lives > 0){
						printf("passei por aqui\n");
						obstacles[i]->lives--;
						// bullet_destroy(index);
						index->y = -1;
						break;
					}
				}
			}
		}
	}
	// enemies bullets
	for(int i = 0; i < n_enemies; i++){
		if(enemies[i]->gun->shots == NULL) continue;
			for(bullet* index = enemies[i]->gun->shots; index != NULL; index = (bullet*) index->next){
			if(index->x >= player->position_x - 16 && index->x <= player->position_x + 16){
				if(index->y >= player->position_y - 16 && index->y <= player->position_y + 16){
					printf("entrei aqui2\n");
					explosion_animation(player->position_x - 20, player->position_y - 20, sprite_sheet);
					player->lives--;
					index->x = -1;
					// bullet_destroy(index);
					break;
				}
			} // obstacles
			for(int i = 0; i < n_obstacles; i++){
				if(index->x >= obstacles[i]->position_x && index->x <= obstacles[i]->position_x + 64){
					if(index->y >= obstacles[i]->position_y && index->y <= obstacles[i]->position_y + 16){
						if(obstacles[i]->lives > 0){
							printf("entrei aqui\n");
							switch(enemies[i]->type){
								case 0: 
									obstacles[i]->lives--;
								break;
								case 1:
									obstacles[i]->lives -= 2;
								break;
								case 2:
									obstacles[i]->lives -= 3;
								break;
							}
							index->x = -1;
							// bullet_destroy(index);
							break;
						}
					}
				}
			}	
		}
	}
	// colision between enemy shots && player shots
	for(int i = 0; i < n_enemies; i++){
		if(enemies[i]->gun->shots == NULL) continue;
			for(bullet* enemy_index = enemies[i]->gun->shots; enemy_index != NULL; enemy_index = (bullet*) enemy_index->next){
				for(bullet* player_index = player->gun->shots; player_index != NULL; player_index = (bullet*) player_index->next){
					if(enemy_index->x >= player_index->x - 16 && enemy_index->x <= player_index->x + 16){
						if(enemy_index->y >= player_index->y - 16 && enemy_index->y <= player_index->y + 16){
							explosion_animation(enemy_index->x - 16, enemy_index->y - 10, sprite_sheet);
							enemy_index->x = -1;
							player_index->y = -1;
							break;
						}
					}
				}
			}
	}

	
}
int two_points_distance(int x1, int x2, int y1, int y2){
	int x_difference = 0;
	int y_difference = 0;
	if(x1 > x2)
		x_difference = x1 - x2;
	else
		x_difference = x2 - x1;

	if(y1 > y2)
		y_difference = y1 - y2;
	else
		y_difference = y2 - y1;

	return sqrt((x_difference * x_difference) + (y_difference * y_difference));
}

void update_enemies_shots(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int player_x, int player_y){
	// preciso analisar os inimigos mais próximos do player
	// e fazer com que eles atirem
	static int shot_delay_0 = 0;
	static int shot_delay_1 = 0;
	static int shot_delay_2 = 0;
	int actual_distance = 0;
	int closest_distance_0 = 1000; // valor arbitrário para iniciar a comparação
	int closest_distance_1 = 1000; // valor arbitrário para iniciar a comparação
	int closest_distance_2 = 1000; // valor arbitrário para iniciar a comparação
	enemy* closest_enemy_0 = enemies[0];
	enemy* closest_enemy_1 = enemies[0];
	enemy* closest_enemy_2 = enemies[0];

	for(int i = 0; i < n_enemies; i++){
		if(enemies[i]->alive){
			switch(enemies[i]->type){
				case 0:
					actual_distance = two_points_distance(enemies[i]->position_x, player_x, enemies[i]->position_y, player_y);
					if(actual_distance < closest_distance_0){
						closest_distance_0 = actual_distance;
						closest_enemy_0 = enemies[i];
					}
				break;
				case 1:
					actual_distance = two_points_distance(enemies[i]->position_x, player_x, enemies[i]->position_y, player_y);
					if(actual_distance < closest_distance_1){
						closest_distance_1 = actual_distance;
						closest_enemy_1 = enemies[i];
					}
				break;
				case 2:
					actual_distance = two_points_distance(enemies[i]->position_x, player_x, enemies[i]->position_y, player_y);
					if(actual_distance < closest_distance_2){
						closest_distance_2 = actual_distance;
						closest_enemy_2 = enemies[i];
					}
				break;
			}
		}
	}
	if(!shot_delay_0){
		enemy_shot(closest_enemy_0);
		shot_delay_0 = ENEMY_SHOT_COOLDOWN + 20;
	}else shot_delay_0--;
	if(!shot_delay_1){
		enemy_shot(closest_enemy_1);
		shot_delay_1 = ENEMY_SHOT_COOLDOWN + 10;
	}else shot_delay_1--;
	if(!shot_delay_2){
		enemy_shot(closest_enemy_2);
		shot_delay_2 = ENEMY_SHOT_COOLDOWN;
	}else shot_delay_2--;
	

}


int main(int argc, char** argv){
	int n_enemies = 66;
	int n_obstacles = 4;
	player* player = create_player(X_SCREEN/2, Y_SCREEN - 16);
	enemy** enemies =  create_enemies(n_enemies, 6, 11);
	obstacle** obstacles = create_obstacles(n_obstacles, X_SCREEN, Y_SCREEN);
	hud* hud = create_hud();
	// Funcoes allegro
	al_init();																																																//Faz a preparação de requisitos da biblioteca Allegro
	al_init_primitives_addon();																																												//Faz a inicialização dos addons das imagens básicas
	
	al_install_keyboard();																																													//Habilita a entrada via teclado (eventos de teclado), no programa

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);																																						//Cria o relógio do jogo; isso indica quantas atualizações serão realizadas por segundo (30, neste caso)
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();																																					//Cria a fila de eventos; todos os eventos (programação orientada a eventos) 
	ALLEGRO_FONT* font = al_create_builtin_font();																																							//Carrega uma fonte padrão para escrever na tela (é bitmap, mas também suporta adicionar fontes ttf)
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);			

	al_register_event_source(queue, al_get_keyboard_event_source());																																		//Indica que eventos de teclado serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_display_event_source(disp));																																		//Indica que eventos de tela serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_timer_event_source(timer));			
	
	// Obtenha as dimensões da tela
    int screen_width = al_get_display_width(al_get_current_display());
    int screen_height = al_get_display_height(al_get_current_display());

    // Calcule a posição inicial para centralizar a janela
    int window_x = (screen_width)/2 + X_SCREEN/4;
    int window_y = (screen_height)/2 - Y_SCREEN;	

	al_set_window_position(disp, window_x, window_y);																											//Indica que eventos de relógio serão inseridos na nossa fila de eventos

	ALLEGRO_EVENT event;																																												//Cria uma variável para armazenar o evento atual
	al_start_timer(timer);																																//Cria uma janela para o programa, define a largura (x) e a altura (y) da tela em píxeis (320x320, neste caso)

	// Inicia imagens
	al_init_image_addon();

	// Inicia Primtivos
	al_init_primitives_addon();

	// Sprites do jogo
	
	ALLEGRO_BITMAP* sprite_sheet = al_load_bitmap("sprites/spritesheet.png");
	if(!sprite_sheet){
		fprintf(stderr, "Falha ao carregar spritesheet!\n");
		exit(1);
	}
	ALLEGRO_BITMAP* icon = al_load_bitmap("sprites/icon.png");
	if(!icon){
		fprintf(stderr, "Falha ao carregar o ícone do jogo!\n");
		exit(1);
	}
	al_set_display_icon(disp, icon);
	
	// if (!al_init_ttf_addon()) {
    // fprintf(stderr, "Erro ao inicializar o addon TrueType do Allegro.\n");
    // return -1;
	// }

	font = al_load_font("fonts/PressStart2P.ttf", 16, 0);
	int sprite_width = 16;
	int sprite_height = 16;
	


	while(1){
		// Laço principal do jogo
		al_wait_for_event(queue, &event);
		
		// Verifica se a tecla ESC está pressionada
		if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE || event.keyboard.keycode == ALLEGRO_KEY_Q){
			break;
    	}
		// Verifica se o botão de fechar foi pressionado
		if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			break;
		}
		if(player->lives == 0){
			// mudar pra tela de restart
			generate_hud(hud, player, sprite_sheet, font);
			explosion_animation(player->position_x - 16, player->position_y -16, sprite_sheet);
			al_flip_display();
			break;
		}else{
			if(event.type == ALLEGRO_EVENT_TIMER){
				update_position(player, enemies, n_enemies);
				al_clear_to_color(al_map_rgb(0, 0, 0));		
				generate_hud(hud, player, sprite_sheet, font);
    			// textprintf_ex(disp, font, 16, 16, makecol(0,0,255), -1, "SCORE:");
				al_draw_scaled_bitmap(sprite_sheet, player->sprite_x, player->sprite_y, sprite_width, sprite_height, player->position_x - 16, player->position_y - 16, sprite_width * 2, sprite_height * 2, 0);		
				generate_obstacles(obstacles, 4, sprite_sheet);
				update_enemies_position(enemies, n_enemies, sprite_sheet, X_SCREEN);
				update_enemies_shots(enemies, n_enemies, sprite_sheet, player->position_x, player->position_y);
				check_collision(player, enemies, n_enemies, obstacles, n_obstacles, sprite_sheet); // checa se houve colisões
				for (bullet *index = player->gun->shots; index != NULL; index = (bullet*) index->next) {
					al_draw_scaled_bitmap(sprite_sheet, 64, 0, sprite_width, sprite_height, index->x - 14 , index->y - 16, sprite_width * 2, sprite_height * 2, 0);							
				}
				for(int i = 0; i < n_enemies; i++){
					if(enemies[i]->gun->timer) enemies[i]->gun->timer--;
					for(bullet* index = enemies[i]->gun->shots; index != NULL; index = (bullet*) index->next)
						switch(enemies[i]->type){
							case 0:
								al_draw_scaled_bitmap(sprite_sheet, 0, 32, sprite_width, sprite_height, index->x - 14, index->y - 16, sprite_width * 2, sprite_height * 2, 0);
							break;
							case 1:
								al_draw_scaled_bitmap(sprite_sheet, 16, 48, sprite_width, sprite_height, index->x - 14, index->y - 16, sprite_width * 2, sprite_height * 2, 0);
							break;
							case 2:
								al_draw_scaled_bitmap(sprite_sheet, 64, 32, sprite_width, sprite_height, index->x - 14, index->y - 16, sprite_width * 2, sprite_height * 2, 0);
							break;
						}
				}
				if(player->gun->timer) player->gun->timer--;

				al_flip_display();																																		
			}else{
				if ((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_KEY_UP)){																																				//Verifica se o evento é de botão do teclado abaixado ou levantado
				if (event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT){
					joystick_left(player->control);
					// player->sprite_x = 16;
				} 																															//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à esquerda)
				else if (event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT){
					joystick_right(player->control);
					// player->sprite_x = 16;
				} 																													//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à direita)
				else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE || event.keyboard.keycode == ALLEGRO_KEY_Z) joystick_fire(player->control);																														//Indica o evento correspondente no controle do primeiro joagdor (botão de disparo - c) (!)					
				}
			}
		}
	}

	// clean_board(board);
	// destroy_board(board);
	// clean_shots(shot_list);
	// free(shot_list);
	// free(o);
	// fclose(report);

	// Allegro
	al_destroy_font(font);																																													//Destrutor da fonte padrão
	al_destroy_display(disp);																																												//Destrutor da tela
	al_destroy_timer(timer);																																												//Destrutor do relógio
	al_destroy_event_queue(queue);	

	return 0;
}
