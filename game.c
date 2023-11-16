#include "game.h"

void update_bullets(player *player, enemy** enemies, int n_enemies){	// game.c																																									//Implementação da função que atualiza o posicionamento de projéteis conforme o movimento dos mesmos (!)
	bullet *previous = NULL;																																												//Variável auxiliar para salvar a posição imediatamente anterior na fila (!)
	for (bullet *index_player = player->gun->shots; index_player != NULL;){																																				//Para cada projétil presente na lista de projéteis disparados (!)
		index_player->y -= BULLET_MOVE;																																											//Atualiza a posição do projétil (!)
		if ((index_player->y < 0) || (index_player->y > Y_SCREEN)){																																						//Verifica se o projétil saiu das bordas da janela (!)
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
void update_position(player *player, enemy** enemies, int n_enemies){	 // game.c																																				//Função de atualização das posições dos quadrados conforme os comandos do controle
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
	update_bullets(player, enemies, n_enemies);			// mudar pra essa chamada ser fora dessa função, ser em sequência
																																									//Atualiza os disparos do primeiro jogador (!)
}

void explosion_animation(int x, int y, ALLEGRO_BITMAP* sprite_sheet){ // game.c
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
	// preciso analisar os inimigos mais próximos do player
	// e fazer com que eles atirem
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