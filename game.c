#include "game.h"

void update_bullets(player *player, enemy** enemies, int n_enemies){	// game.c																																									//Implementação da função que atualiza o posicionamento de projéteis conforme o movimento dos mesmos (!)
	bullet *previous = NULL;																																												//Variável auxiliar para salvar a posição imediatamente anterior na fila (!)
	for (bullet *index_player = player->gun->shots; index_player != NULL;){																																				//Para cada projétil presente na lista de projéteis disparados (!)
		index_player->y -= BULLET_MOVE;																																											//Atualiza a posição do projétil (!)
		if ((index_player->y < 80) || (index_player->y > Y_SCREEN)){																																						//Verifica se o projétil saiu das bordas da janela (!)
			if (previous){																																													//Verifica se não é o primeiro elemento da lista de projéteis (!)
				previous->next = index_player->next;																																								//Se não for, salva o próximo projétil (!)
				// bullet_destroy(index_player);																																										//Chama o destrutor para o projétil atual (!)
				index_player = (bullet*) previous->next;																																							//Atualiza para o próximo projétil (!)
			}
			else {																																															//Se for o primeiro projétil da lista (!)
				player->gun->shots = (bullet*) index_player->next;																																					//Atualiza o projétil no início da lista (!)
				// bulmakelet_destroy(index_player);																																										//Chama o destrutor para o projétil atual (!)
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
		printf("teste lopp entrei \n");
		if(enemies[i]->gun->shots == NULL) continue;
		for(bullet* index_enemies = enemies[i]->gun->shots; index_enemies != NULL;){
			index_enemies->y += BULLET_MOVE;
			if(((index_enemies->y < 0) || (index_enemies->y > Y_SCREEN)) || (index_enemies->x < 0)){
				if(previous){
					printf("teste1\n");
					previous->next = index_enemies->next;
					printf("teste2\n");
					// bullet_destroy(index_enemies);
					printf("teste3\n");
					index_enemies = (bullet*) previous->next;
					printf("teste4\n");
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
void update_player_position(player *player, ALLEGRO_BITMAP* sprite_sheet){	 // game.c																																				//Função de atualização das posições dos quadrados conforme os comandos do controle
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
	al_draw_scaled_bitmap(sprite_sheet, player->sprite_x, player->sprite_y, 16, 16, player->position_x - 16, player->position_y - 16, 16 * 2, 16 * 2, 0);		
}

void explosion_animation(int x, int y, ALLEGRO_BITMAP* sprite_sheet){ // game.c
	int sprite_x = 32;
	int sprite_y = 64;
	
	for(int i = 0; i < 11; i++){
		al_draw_scaled_bitmap(sprite_sheet, sprite_x, sprite_y, 16, 16, x , y , 16 * 3, 16 * 3, 0);		
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

void player_score(int* score, int enemy_type){ // game.c
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

int two_points_distance(int x1, int x2, int y1, int y2){ // game.c
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

void update_enemies_shots(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int player_x, int player_y, int round){ // game.c
	static int shot_delay_0 = 200;
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
		shot_delay_0 = ENEMY_SHOT_COOLDOWN + 20 - (round/2);
	}else shot_delay_0--;
	if(!shot_delay_1){
		enemy_shot(closest_enemy_1);
		shot_delay_1 = ENEMY_SHOT_COOLDOWN + 10 - (round/2);
	}else shot_delay_1--;
	if(!shot_delay_2){
		enemy_shot(closest_enemy_2);
		shot_delay_2 = ENEMY_SHOT_COOLDOWN -  (round/2);
	}else shot_delay_2--;
}

void draw_player_bullets(player* player, ALLEGRO_BITMAP* sprite_sheet){ // game.c
	for(bullet* index = player->gun->shots; index != NULL; index = (bullet*) index->next)
		al_draw_scaled_bitmap(sprite_sheet, 64, 0, 16, 16, index->x - 14, index->y - 16, 16 * 2, 16 * 2, 0);
}

void draw_enemies_bullets(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet){
	for(int i = 0; i < n_enemies; i++){
	if(enemies[i]->gun->timer) enemies[i]->gun->timer--;
	for(bullet* index = enemies[i]->gun->shots; index != NULL; index = (bullet*) index->next)
		switch(enemies[i]->type){
			case 0:
				al_draw_scaled_bitmap(sprite_sheet, 0, 32, 16, 16, index->x - 14, index->y - 16, 16 * 2, 16 * 2, 0);
			break;
			case 1:
				al_draw_scaled_bitmap(sprite_sheet, 16, 48, 16, 16, index->x - 14, index->y - 16, 16 * 2, 16 * 2, 0);
			break;
			case 2:
				al_draw_scaled_bitmap(sprite_sheet, 64, 32, 16, 16, index->x - 14, index->y - 16, 16 * 2, 16 * 2, 0);
			break;
		}
	}
}
void menu_event(){
	static menu_option startButton = {100, 200, 200, 50, "Começar Jogo", start_game_action};
	static menu_option instructionsButton = {100, 300, 200, 50, "Instruções", instructions_action};
	static menu_option quitButton = {100, 400, 200, 50, "Sair do Jogo", quit_game_action};

}

void game_event(short unsigned* running, int round, player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles, hud* hud, ALLEGRO_FONT* font, ALLEGRO_BITMAP* sprite_sheet, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_DISPLAY* disp, ALLEGRO_EVENT event){
	// Laço principal do jogo
	al_wait_for_event(queue, &event);
	// Verifica se a tecla ESC está pressionada ou o botão de fechar a janela foi clicado
	if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE || event.keyboard.keycode == ALLEGRO_KEY_Q || event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		*running = 0;
	if(player->lives == 0){
		generate_hud(hud, player, round, sprite_sheet, font);
		explosion_animation(player->position_x - 16, player->position_y -16, sprite_sheet);
		al_flip_display();
		*running = 0;
	}else{
		if(event.type == ALLEGRO_EVENT_TIMER){
			al_clear_to_color(al_map_rgb(0, 0, 0));		
			update_player_position(player, sprite_sheet);
			update_bullets(player, enemies, n_enemies);
			generate_hud(hud, player, round, sprite_sheet, font);
			generate_obstacles(obstacles, 4, sprite_sheet);
			update_enemies_position(enemies, n_enemies, sprite_sheet, X_SCREEN, round);
			update_enemies_shots(enemies, n_enemies, sprite_sheet, player->position_x, player->position_y, round);
			check_collision(player, enemies, n_enemies, obstacles, n_obstacles, sprite_sheet); // checa se houve colisões
			draw_player_bullets(player, sprite_sheet);
			draw_enemies_bullets(enemies, n_enemies, sprite_sheet);
			if(player->gun->timer) player->gun->timer--;
			if(!enemies_alive(enemies, n_enemies)){
				// resetar o round e aumentar a velocidade dos inimigos e da frequência de tiro deles
				free_enemies(enemies, n_enemies);
				restore_obstacles(obstacles, n_obstacles);
				round += 8;
				enemies = create_enemies(n_enemies, 6, 11);
				if(player->lives == player->max_lives){
					if(player->max_lives < 6){
					player->max_lives++;
					player->lives++;
					}
				}else{
					player->lives++;
				}
			}
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

void generating_game(int program_event, int round, player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles, hud* hud, ALLEGRO_FONT* font, ALLEGRO_BITMAP* sprite_sheet, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_DISPLAY* disp, ALLEGRO_EVENT event){
		unsigned short running = 1;
		while(running){
			switch(program_event){
				case 0: // menu event
				break;
				case 1: // game event
				game_event(&running, round, player, enemies, n_enemies, obstacles, n_obstacles, hud, font, sprite_sheet, timer, queue, disp, event);
				break;		
				case 2: // game over event
				break;
			}	
	}

}