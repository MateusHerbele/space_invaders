#include "game.h"

void update_bullets(player *player, enemy** enemies, int n_enemies){
	bullet *previous = NULL;
	for (bullet *index_player = player->gun->shots; index_player != NULL;){ // Itera sobre todos os tiros do jogador
		index_player->y -= BULLET_MOVE + 4; // Atualiza a posição do tiro
		if ((index_player->y < 80) || (index_player->y > Y_SCREEN)){ // Se o tiro sair da tela, ele é destruído	
			if (previous){ // Se o tiro anterior existir, ele aponta para o próximo tiro
				// printf("entrei aqui if\n");	
				previous->next = index_player->next; // O tiro anterior aponta para o próximo tiro
				bullet_destroy(index_player); // O tiro atual é destruído
				index_player = (bullet*) previous->next; // O tiro atual aponta para o próximo tiro
			}
			else { // Se o tiro anterior não existir, o tiro atual é o primeiro da lista
				// printf("entrei aqui else\n");			
				player->gun->shots = (bullet*) index_player->next; // O primeiro tiro da lista aponta para o próximo tiro
				bullet_destroy(index_player); // O tiro atual é destruído
				index_player = player->gun->shots; // O tiro atual aponta para o próximo tiro		
			}
		}
		else{ // Se o tiro não sair da tela, o tiro anterior aponta para o tiro atual e o tiro atual aponta para o próximo tiro
			previous = index_player; // O tiro anterior aponta para o tiro atual					
			index_player = (bullet*) index_player->next; // O tiro atual aponta para o próximo tiro
		}
	}

	for(int i = 0; i < n_enemies; i++){ // Itera sobre todos os inimigos
		previous = NULL;
		if(enemies[i]->gun->shots == NULL) continue; // Se o inimigo não tiver disparado, pula para o próximo
		// inimigo da vez
		for(bullet* index_enemies = enemies[i]->gun->shots; index_enemies != NULL;){ // Itera sobre todos os tiros do inimigo da vez
			index_enemies->y += BULLET_MOVE; // Atualiza a posição do tiro
			if(((index_enemies->y < 0) || (index_enemies->y > Y_SCREEN)) || (index_enemies->x < 0)){ // Se o tiro sair da tela, ele é destruído
				if(previous){ // Se o tiro anterior existir, ele aponta para o próximo tiro
					previous->next = index_enemies->next; // O tiro anterior aponta para o próximo tiro
					bullet_destroy(index_enemies); // O tiro atual é destruído
					index_enemies = (bullet*) previous->next; // O tiro atual aponta para o próximo tiro
				}else{ // Se o tiro anterior não existir, o tiro atual é o primeiro da lista
					enemies[i]->gun->shots = (bullet*) index_enemies->next; // O primeiro tiro da lista aponta para o próximo tiro
					bullet_destroy(index_enemies); // O tiro atual é destruído
					index_enemies = enemies[i]->gun->shots; // O tiro atual aponta para o próximo tiro
				}
				break;
			}
			else{ // Se o tiro não sair da tela, o tiro anterior aponta para o tiro atual e o tiro atual aponta para o próximo tiro
				previous = index_enemies; // O tiro anterior aponta para o tiro atual
				index_enemies = (bullet*) index_enemies->next; // O tiro atual aponta para o próximo tiro
			}
		}
	}
}
// update position
void update_player_position(player *player, ALLEGRO_BITMAP* sprite_sheet){																																					//Função de atualização das posições dos quadrados conforme os comandos do controle
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

void explosion_animation(int x, int y, ALLEGRO_BITMAP* sprite_sheet){
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

void player_score(int* score, int enemy_type){
	static int extra_multiplyer = 1;
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
			*score += 100 * extra_multiplyer;
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
				if(enemies[i]->type == 3){ // Caso seja o inimigo especial aumenta a hitbox da colisão
					if(index->x >= enemies[i]->position_x - 10 && index->x <= enemies[i]->position_x + 32){
						if(index->y >= enemies[i]->position_y - 24 && index->y <= enemies[i]->position_y + 24){
							enemies[i]->alive = 0;
							explosion_animation(enemies[i]->position_x - 16, enemies[i]->position_y - 16, sprite_sheet);
							index->y = -32;
							player_score(&player->score, enemies[i]->type);
							break;
						}
					}
				}else
				if(index->x >= enemies[i]->position_x - 10 && index->x <= enemies[i]->position_x + 16){
					if(index->y >= enemies[i]->position_y - 16 && index->y <= enemies[i]->position_y + 16){
						enemies[i]->alive = 0;
						explosion_animation(enemies[i]->position_x - 10, enemies[i]->position_y - 10, sprite_sheet);
						index->y = -32;
						player_score(&player->score, enemies[i]->type);
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
						obstacles[i]->lives--;
						index->y = -32;
						break;
					}
				}
			}
		}
	}
	// enemies bullets
	for(int i = 0; i < n_enemies; i++){
		if(enemies[i]->gun->shots == NULL) continue;
			for(bullet* index_enemies = enemies[i]->gun->shots; index_enemies != NULL; index_enemies = (bullet*) index_enemies->next){
			if(index_enemies->x >= player->position_x - 16 && index_enemies->x <= player->position_x + 16){
				if(index_enemies->y >= player->position_y - 16 && index_enemies->y <= player->position_y + 16){
					explosion_animation(player->position_x - 20, player->position_y - 20, sprite_sheet);
					player->lives--;
					index_enemies->x = -32;
					break;
				}
			} // obstacles
			for(int i = 0; i < n_obstacles; i++){
				if(index_enemies->x >= obstacles[i]->position_x && index_enemies->x <= obstacles[i]->position_x + 64){
					if(index_enemies->y >= obstacles[i]->position_y && index_enemies->y <= obstacles[i]->position_y + 16){
						if(obstacles[i]->lives > 0){
							switch(enemies[i]->type){
								case 0: 
									obstacles[i]->lives--;
								break;
								case 1:
									obstacles[i]->lives -= 2;
								break;
								case 2:
									obstacles[i]->lives -= 2;
								break;
							}
							index_enemies->x = -32;
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
							enemy_index->x = -32;
							player_index->y = -32;
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

void update_enemies_shots(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int player_x, int player_y, unsigned short round){
	static int shot_delay_0 = 200; // Valor pra não começar o round com tiros
	static int shot_delay_1 = 200; // Valor pra não começar o round com tiros	
	int actual_distance = 0;
	int closest_distance_0 = 1000; // valor arbitrário para iniciar a comparação
	enemy* closest_enemy_0 = NULL;
	enemy* closest_enemy_1 = NULL;

	for(int i = 0; i < n_enemies; i++){
		if(enemies[i]->alive)
			switch(enemies[i]->type){
				case 0:
					if(enemy_has_shot_column(enemies[i]->position_x, enemies, n_enemies) ||  enemy_in_front_of_enemy(enemies[i], enemies, n_enemies))
						continue;
					else{
						actual_distance = two_points_distance(enemies[i]->position_x, player_x, enemies[i]->position_y, player_y);
						if(actual_distance < closest_distance_0){
							closest_enemy_1 = closest_enemy_0;
							closest_distance_0 = actual_distance;
							closest_enemy_0 = enemies[i];
						}
					}
				break;
				case 1:
					if(enemy_has_shot_column(enemies[i]->position_x, enemies, n_enemies))
						continue;
					else{
						actual_distance = two_points_distance(enemies[i]->position_x, player_x, enemies[i]->position_y, player_y);
						if(actual_distance < closest_distance_0){
							closest_enemy_1 = closest_enemy_0;
							closest_distance_0 = actual_distance;
							closest_enemy_0 = enemies[i];
						}
					}
				break;
				case 2:
					actual_distance = two_points_distance(enemies[i]->position_x, player_x, enemies[i]->position_y, player_y);
					if(actual_distance < closest_distance_0){
						closest_enemy_1 = closest_enemy_0;
						closest_distance_0 = actual_distance;
						closest_enemy_0 = enemies[i];
					}
				break;
			}
	}
	if(shot_delay_0 <= 0){
		enemy_shot(closest_enemy_0);
		shot_delay_0 = ENEMY_SHOT_COOLDOWN + 20 - (round/2);
	}else shot_delay_0--;

	if(shot_delay_1 <= 0){
		enemy_shot(closest_enemy_1);
		shot_delay_1 = ENEMY_SHOT_COOLDOWN + 10 - (round/2);
	}else
		shot_delay_1--;
}

void draw_player_bullets(player* player, ALLEGRO_BITMAP* sprite_sheet){
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

void restart_conditions(unsigned short* round, player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles){
	// player
	player->position_x = X_SCREEN/2;
	player->position_y = Y_SCREEN - 64;
	player->max_lives = 3;
	player->lives = 3;
	player->score = 0;
	destroy_bullet_list(player->gun->shots);
	// enemies
	free_enemies(enemies, n_enemies);
	enemies = create_enemies(n_enemies, 6, 11);
	// obstacles
	restore_obstacles(obstacles, n_obstacles);
	// round
	*round = 0;

}


void game_over_event(unsigned short* running, unsigned short* program_event, ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, int round, int score){ // &running, &program_event, event, queue, font, round, player->score
	int text_width = 0, text_height = 0;
	al_wait_for_event(queue, &event);
	if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE || event.keyboard.keycode == ALLEGRO_KEY_Q || event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		*running = 0;
	if(event.type == ALLEGRO_EVENT_TIMER){
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_rectangle(200, 200, 450, 300, al_map_rgb(255, 255, 255), 2);
		text_width = al_get_text_width(font, "GAME OVER");
		text_height = al_get_font_line_height(font);
		al_draw_text(font, al_map_rgb(255, 0, 0), 220 + (200 - text_width) / 2, 200 + (50 - text_height) / 2, 0, "GAME OVER");
		text_width = al_get_text_width(font, "ROUND: ");
		al_draw_textf(font, al_map_rgb(112, 221, 165), 200 + (200 - text_width) / 2, 225 + (50 - text_height) / 2, 0, "ROUND: %d", round);
		text_width = al_get_text_width(font, "SCORE: ");
		al_draw_textf(font, al_map_rgb(255, 255, 73), 200 + (200 - text_width) / 2, 250 + (50 - text_height) / 2, 0, "SCORE: %d", score);
		text_width = al_get_text_width(font, "PRESS 'ENTER' TO RESTART");
		al_draw_text(font, al_map_rgb(255, 255, 255), 220 + (200 - text_width) / 2, 700 + (50 - text_height) / 2, 0, "PRESS 'ENTER' TO RESTART");
		text_width = al_get_text_width(font, "PRESS 'ESC' TO QUIT");
		al_draw_text(font, al_map_rgb(255, 255, 255), 220 + (200 - text_width) / 2, 750 + (50 - text_height) / 2, 0, "PRESS 'ESC' TO QUIT");
		al_flip_display();
	}else{
		if(event.type == ALLEGRO_EVENT_KEY_DOWN){
			if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
				*program_event = 1;
			}
			if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				*running = 0;
		}
	}
}

void menu_event(unsigned short* running, unsigned short* program_event, ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_BITMAP* sprite_sheet, ALLEGRO_FONT* font){
	static menu_option start_button = {(X_SCREEN/2) - 100, 300, 200, 50, "START GAME", true, start_game_action};
	static menu_option instructions_button = {(X_SCREEN/2) - 100, 400, 200, 50, "INSTRUCTIONS", false, instructions_action};
	static menu_option quit_button = {(X_SCREEN/2) - 100, 500, 200, 50, "QUIT GAME", false, quit_game_action};

	menu_option* options[] = {&start_button, &instructions_button, &quit_button};
	render_menu(event, queue, sprite_sheet, options, font, running, program_event);

}
void next_round(unsigned short* round, player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles){
	// resetar o round e aumentar a velocidade dos inimigos e da frequência de tiro deles
	free_enemies(enemies, n_enemies);
	restore_obstacles(obstacles, n_obstacles);
	*round += 8;
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
void game_event(short unsigned* running, unsigned short* program_event, unsigned short* round, player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles, hud* hud, ALLEGRO_FONT* font, ALLEGRO_BITMAP* sprite_sheet, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_DISPLAY* disp, ALLEGRO_EVENT event){
	// Laço principal do jogo
	al_wait_for_event(queue, &event);
	// Verifica se a tecla ESC está pressionada ou o botão de fechar a janela foi clicado
	if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE || event.keyboard.keycode == ALLEGRO_KEY_Q || event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
		*running = 0;
		return;
	}
	if(player->lives == 0 || bottommost_enemy(enemies, n_enemies) >= player->position_y){
		generate_hud(hud, player, *round, sprite_sheet, font);
		explosion_animation(player->position_x - 16, player->position_y -16, sprite_sheet);
		al_flip_display();
		*program_event = 2;
		return;
	}else{
		if(event.type == ALLEGRO_EVENT_TIMER){
			al_clear_to_color(al_map_rgb(0, 0, 30));		
			update_player_position(player, sprite_sheet);
			update_bullets(player, enemies, n_enemies);
			generate_hud(hud, player, *round, sprite_sheet, font);
			generate_obstacles(obstacles, 4, sprite_sheet);
			update_enemies_position(enemies, n_enemies, sprite_sheet, X_SCREEN, *round);
			update_enemies_shots(enemies, n_enemies, sprite_sheet, player->position_x, player->position_y, *round);
			check_collision(player, enemies, n_enemies, obstacles, n_obstacles, sprite_sheet);
			draw_player_bullets(player, sprite_sheet);
			draw_enemies_bullets(enemies, n_enemies, sprite_sheet);
			extra_enemy_event(enemies, n_enemies);
			if(player->gun->timer) player->gun->timer--;
			if(!enemies_alive(enemies, n_enemies))
				next_round(round, player, enemies, n_enemies, obstacles, n_obstacles);
			al_flip_display();																																		
		}else{
			if ((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_KEY_UP)){																																				//Verifica se o evento é de botão do teclado abaixado ou levantado
			if (event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT)
				joystick_left(player->control);
			else if (event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
				joystick_right(player->control);
			else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE || event.keyboard.keycode == ALLEGRO_KEY_Z) joystick_fire(player->control);																														//Indica o evento correspondente no controle do primeiro joagdor (botão de disparo - c) (!)					
			}
		}
	}
}

void free_all(player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles, hud* hud){
	// Player
	free_player(player);
	// Enemies
	free_enemies(enemies, n_enemies);
	// Obstacles
	free_obstacles(obstacles, n_obstacles);
	// HUD
	free(hud);
}

void generating_game(unsigned short program_event, unsigned short round, player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles, hud* hud, ALLEGRO_FONT* font, ALLEGRO_BITMAP* sprite_sheet, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_DISPLAY* disp, ALLEGRO_EVENT event){
	unsigned short running = 1;
		while(running){
			switch(program_event){
				case 0: // menu event
				menu_event(&running, &program_event, event, queue, sprite_sheet, font);
				break;
				case 1: // game event
				game_event(&running, &program_event, &round, player, enemies, n_enemies, obstacles, n_obstacles, hud, font, sprite_sheet, timer, queue, disp, event);
				break;		
				case 2: // game over event
				restart_conditions(&round, player, enemies, n_enemies, obstacles, n_obstacles);
				game_over_event(&running, &program_event, event, queue, font, round, player->score);
				break;
			}	
		}
	free_all(player, enemies, n_enemies, obstacles, n_obstacles, hud);
}