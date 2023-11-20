#include "game.h"

// Atualiza a posição dos tiros do jogador e dos inimigos
void update_bullets(player *player, enemy** enemies, int n_enemies){
	bullet *previous = NULL;
	for(bullet *index_player = player->gun->shots; index_player != NULL;){ // Itera sobre todos os tiros do jogador
		index_player->y -= BULLET_MOVE + 4; // Atualiza a posição do tiro
		if((index_player->y < 80) || (index_player->y > Y_SCREEN)){ // Se o tiro sair da tela, ele é destruído	
			if(previous){ // Se o tiro anterior existir, ele aponta para o próximo tiro
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
// Animação de explosão
void explosion_animation(int x, int y, ALLEGRO_BITMAP* sprite_sheet){
	int sprite_x = 32;
	int sprite_y = 64;
	
	for(int i = 0; i < 11; i++){ // São 11 frames de animação
		al_draw_scaled_bitmap(sprite_sheet, sprite_x, sprite_y, 16, 16, x , y , 16 * 3, 16 * 3, 0);		
		sprite_x += 16;
		if(i == 4){ // Por conta da posição do sprite dentro do sprite sheet, é necessário pular uma linha
			sprite_x = 0;
			sprite_y += 16;
		}
	}
}
// Checa se há colisão entre os tiros do jogador com inimigos ou obstáculos
void check_collision_player_shots(player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles, ALLEGRO_BITMAP* sprite_sheet){
	for(bullet* index = player->gun->shots; index != NULL; index = (bullet*) index->next){ // Itera sobre todos os tiros do jogador
			for(int i = 0; i < n_enemies; i++){ // Itera sobre todos os inimigos
				if(enemies[i]->alive){ // Se o inimigo estiver vivo, checa a colisão
					if(enemies[i]->type == 3){ // Caso seja o inimigo especial aumenta a hitbox da colisão
						if(index->x >= enemies[i]->position_x - 10 && index->x <= enemies[i]->position_x + 32){ // Checa se o tiro está na mesma posição do inimigo
							if(index->y >= enemies[i]->position_y - 24 && index->y <= enemies[i]->position_y + 24){ // Checa se o tiro está na mesma posição do inimigo
								enemies[i]->alive = 0; // O inimigo morre
								explosion_animation(enemies[i]->position_x - 16, enemies[i]->position_y - 16, sprite_sheet); // Animação de explosão no inimigo
								index->y = -32; // O tiro é colocado em uma posição que será removido da tela e da lista de tiros
								player_score(&player->score, enemies[i]->type); // Aumenta a pontuação do jogador
								break; // Para a iteração do tiro atual com os inimigos
							}
						}
					}else // Se for os inimigos comuns:
					if(index->x >= enemies[i]->position_x - 10 && index->x <= enemies[i]->position_x + 16){ // Checa se o tiro está na mesma posição do inimigo
						if(index->y >= enemies[i]->position_y - 16 && index->y <= enemies[i]->position_y + 16){ // Checa se o tiro está na mesma posição do inimigo
							enemies[i]->alive = 0; // O inimigo morre
							explosion_animation(enemies[i]->position_x - 10, enemies[i]->position_y - 10, sprite_sheet); // Animação de explosão no inimigo
							index->y = -32; // O tiro é colocado em uma posição que será removido da tela e da lista de tiros
							player_score(&player->score, enemies[i]->type); // Aumenta a pontuação do jogador
							break; // Para a iteração do tiro atual com os inimigos
						}
					}
				}
			}
			// Obstáculos
			for(int i = 0; i < n_obstacles; i++){ // Itera sobre todos os obstáculos
				if(index->x >= obstacles[i]->position_x && index->x <= obstacles[i]->position_x + 64){ // Checa se o tiro está na mesma posição do obstáculo
					if(index->y >= obstacles[i]->position_y && index->y <= obstacles[i]->position_y + 16){ // Checa se o tiro está na mesma posição do obstáculo
						if(obstacles[i]->lives > 0){ // Se o obstáculo estiver vivo, ele perde uma vida
							obstacles[i]->lives--; // O obstáculo perde uma vida
							index->y = -32; // O tiro é colocado em uma posição que será removido da tela e da lista de tiros
							break; // Para a iteração do tiro atual com os obstáculos
						}
					}
				}
			}
	}
}
// Checa se há colisão entre os tiros dos inimigos com o jogador ou obstáculos
void check_collision_enemy_shots(player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles, ALLEGRO_BITMAP* sprite_sheet){
	for(int i = 0; i < n_enemies; i++){ // Itera sobre todos os inimigos
		if(enemies[i]->gun->shots == NULL) continue; // Se o inimigo não tiver disparado, pula para o próximo
			for(bullet* index_enemies = enemies[i]->gun->shots; index_enemies != NULL; index_enemies = (bullet*) index_enemies->next){ // Itera sobre todos os tiros do inimigo analisado
			if(index_enemies->x >= player->position_x - 16 && index_enemies->x <= player->position_x + 16){ // Checa se o tiro está na mesma posição do jogador
				if(index_enemies->y >= player->position_y - 16 && index_enemies->y <= player->position_y + 16){ // Checa se o tiro está na mesma posição do jogador
					explosion_animation(player->position_x - 20, player->position_y - 20, sprite_sheet); // Animação de explosão no jogador
					player->lives--; // O jogador perde uma vida
					index_enemies->x = -32; // O tiro é colocado em uma posição que será removido da tela e da lista de tiros
					break; // Para a iteração do tiro atual com a posição do jogador
				}
			} // Obstáculos
			for(int i = 0; i < n_obstacles; i++){ // Itera sobre todos os obstáculos
				if(index_enemies->x >= obstacles[i]->position_x && index_enemies->x <= obstacles[i]->position_x + 64){ // Checa se o tiro está na mesma posição do obstáculo
					if(index_enemies->y >= obstacles[i]->position_y && index_enemies->y <= obstacles[i]->position_y + 16){ // Checa se o tiro está na mesma posição do obstáculo
						if(obstacles[i]->lives > 0){ // Se o obstáculo estiver vivo, ele perde vida
							switch(enemies[i]->type){ // Perde vida com base no tipo de inimigo
								case 0: 
									obstacles[i]->lives--; // Tiro do inimigo fraco 1 de dano
								break;
								case 1:
								case 2:
									obstacles[i]->lives -= 2; // Tiro do inimigo médio e forte 2 de dano
								break;
							}
							index_enemies->x = -32; // O tiro é colocado em uma posição que será removido da tela e da lista de tiros
							break; // Para a iteração do tiro atual com os obstáculos
						}
					}
				}
			}	
		}
	}
}
// Checa se há colisão entre os tiros dos inimigos com os tiros do jogador
void check_collision_enemy_player(player* player, enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet){
	for(int i = 0; i < n_enemies; i++){ // Itera sobre todos os inimigos
		if(enemies[i]->gun->shots == NULL) continue; // Se o inimigo não tiver disparado, pula para o próximo
			for(bullet* enemy_index = enemies[i]->gun->shots; enemy_index != NULL; enemy_index = (bullet*) enemy_index->next){ // Itera sobre todos os tiros do inimigo analisado
				for(bullet* player_index = player->gun->shots; player_index != NULL; player_index = (bullet*) player_index->next){ // Itera sobre todos os tiros do jogador
					if(enemy_index->x >= player_index->x - 16 && enemy_index->x <= player_index->x + 16){ // Checa se o tiro do inimigo está na mesma posição do tiro do jogador
						if(enemy_index->y >= player_index->y - 16 && enemy_index->y <= player_index->y + 16){ // Checa se o tiro do inimigo está na mesma posição do tiro do jogador
							explosion_animation(enemy_index->x - 16, enemy_index->y - 10, sprite_sheet); // Animação de explosão com base em uma média em relação aos tiros
							enemy_index->x = -32; // O tiro do inimigo é colocado em uma posição que será removido da tela e da lista de tiros
							player_index->y = -32; // O tiro do jogador é colocado em uma posição que será removido da tela e da lista de tiros
							break; // Para a iteração do tiro do inimigo com o tiro do jogador
						}
					}
				}
			}
	}
}
// Rediciona para as funções de colisão
void check_collision(player *player, enemy **enemies, int n_enemies, obstacle** obstacles, int n_obstacles, ALLEGRO_BITMAP* sprite_sheet){
	// Player -> inimigos / obstáculos
	check_collision_player_shots(player, enemies, n_enemies, obstacles, n_obstacles, sprite_sheet);
	// Inimigos -> player / obstáculos
	check_collision_enemy_shots(player, enemies, n_enemies, obstacles, n_obstacles, sprite_sheet);
	// Tiros do jogador -> tiros dos inimigos
	check_collision_enemy_player(player, enemies, n_enemies, sprite_sheet);
}
// Reseta as condições do jogo
void restart_conditions(unsigned short* round, player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles){
	// Player
	player->position_x = X_SCREEN/2;
	player->position_y = Y_SCREEN - 64;
	player->max_lives = 3;
	player->lives = 3;
	player->score = 0;
	destroy_bullet_list(player->gun->shots);
	// Inimigos
	free_enemies(enemies, n_enemies);
	enemies = create_enemies(n_enemies, 6, 11);
	// Obstáculos
	restore_obstacles(obstacles, n_obstacles);
	// Round
	*round = 0;
}
// Renderiza a tela de game over
void game_over_event(unsigned short* running, unsigned short* program_event, ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, int round, int score){ 
	int text_width = 0, text_height = 0;
	al_wait_for_event(queue, &event); // Espera por um evento
	if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) // Verifica se o botão de fechar a janela foi clicado
		*running = 0; // Encerra o jogo no looping principal
	if(event.type == ALLEGRO_EVENT_TIMER){ // Se o evento dentro da fila for timer
		al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpa a tela com a cor preta
		al_draw_rectangle(200, 200, 450, 300, al_map_rgb(255, 255, 255), 2); // Desenha um retângulo branco
		text_width = al_get_text_width(font, "GAME OVER"); // Obtém a largura do texto
		text_height = al_get_font_line_height(font); // Obtém a altura do texto
		al_draw_text(font, al_map_rgb(255, 0, 0), 220 + (200 - text_width) / 2, 200 + (50 - text_height) / 2, 0, "GAME OVER"); // Desenha o texto
		text_width = al_get_text_width(font, "ROUND: ");
		al_draw_textf(font, al_map_rgb(112, 221, 165), 200 + (200 - text_width) / 2, 225 + (50 - text_height) / 2, 0, "ROUND: %d", round);
		text_width = al_get_text_width(font, "SCORE: ");
		al_draw_textf(font, al_map_rgb(255, 255, 73), 200 + (200 - text_width) / 2, 250 + (50 - text_height) / 2, 0, "SCORE: %d", score);
		text_width = al_get_text_width(font, "PRESS 'ENTER' TO RESTART");
		al_draw_text(font, al_map_rgb(255, 255, 255), 220 + (200 - text_width) / 2, 700 + (50 - text_height) / 2, 0, "PRESS 'ENTER' TO RESTART");
		text_width = al_get_text_width(font, "PRESS 'ESC' TO QUIT");
		al_draw_text(font, al_map_rgb(255, 255, 255), 220 + (200 - text_width) / 2, 750 + (50 - text_height) / 2, 0, "PRESS 'ESC' TO QUIT");
		al_flip_display(); // Atualiza a tela
	}else{ // Se o evento dentro da fila não for timer
		if(event.type == ALLEGRO_EVENT_KEY_DOWN){ // Verifica se teve alguma tecla pressionada
			if(event.keyboard.keycode == ALLEGRO_KEY_ENTER) // Verifica se a tecla ENTER foi pressionada
				*program_event = 1; // Redireciona para o evento do jogo, reiniando o jogo
			
			if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) // Verifica se a tecla ESC foi pressionada
				*running = 0; // Encerra o jogo no looping principal
		}
	}
}
// Prepara os itens que são usados pra renderizar o menu e chama a função que renderiza ele
void menu_event(unsigned short* running, unsigned short* program_event, ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_BITMAP* sprite_sheet, ALLEGRO_FONT* font){
	static menu_option start_button = {(X_SCREEN/2) - 100, 300, 200, 50, "START GAME", true, start_game_action};
	static menu_option instructions_button = {(X_SCREEN/2) - 100, 400, 200, 50, "INSTRUCTIONS", false, instructions_action};
	static menu_option quit_button = {(X_SCREEN/2) - 100, 500, 200, 50, "QUIT GAME", false, quit_game_action};

	menu_option* options[] = {&start_button, &instructions_button, &quit_button};
	// Renderiza o menu
	render_menu(event, queue, sprite_sheet, options, font, running, program_event);
}
// Passa para o próximo round, aumentando o valor do round, a vida do jogador e reinciando os inimigos e obstáculos
void next_round(unsigned short* round, player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles){
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
// Realiza os evntos do jogo e renderiza seus elementos na tela
void game_event(short unsigned* running, unsigned short* program_event, unsigned short* round, player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles, hud* hud, ALLEGRO_FONT* font, ALLEGRO_BITMAP* sprite_sheet, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_DISPLAY* disp, ALLEGRO_EVENT event){
	// Laço principal do jogo
	al_wait_for_event(queue, &event);
	// Verifica se a tecla ESC está pressionada ou o botão de fechar a janela foi clicado
	if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE || event.keyboard.keycode == ALLEGRO_KEY_Q || event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){ // Verifica se a tecla ESC ou Q foi pressionada e se o botão de fechar a janela foi clicado
		*running = 0; // Encerra o jogo no looping principal
		return;
	}
	if(player->lives == 0 || bottommost_enemy(enemies, n_enemies) >= player->position_y){ // Verifica se o jogador morreu ou se os inimigos chegaram até ele
		generate_hud(hud, player, *round, sprite_sheet, font); // Gera a hud só pra mostrar a vida do jogador
		explosion_animation(player->position_x - 16, player->position_y -16, sprite_sheet); // Animação de explosão no jogador
		al_flip_display(); // Atualiza a tela
		*program_event = 2; // Redireciona para o evento de game over
		return;
	}else{ // Se o jogador não morreu
		if(event.type == ALLEGRO_EVENT_TIMER){ // Verifica se o evento dentro da fila é timer
			al_clear_to_color(al_map_rgb(0, 0, 30)); // Limpa a tela com a cor azul escuro		
			update_player_position(player, sprite_sheet); // Atualiza a posição do jogador
			update_bullets(player, enemies, n_enemies); // Atualiza a posição dos tiros do jogador e dos inimigos
			generate_hud(hud, player, *round, sprite_sheet, font); // Gera a hud
			generate_obstacles(obstacles, 4, sprite_sheet); // Gera os obstáculos
			update_enemies_position(enemies, n_enemies, sprite_sheet, X_SCREEN, *round); // Atualiza a posição dos inimigos
			update_enemies_shots(enemies, n_enemies, sprite_sheet, player->position_x, player->position_y, *round); // Atualiza a posição dos tiros dos inimigos
			check_collision(player, enemies, n_enemies, obstacles, n_obstacles, sprite_sheet); // Checa se há colisão com os tiros das entidades
			draw_player_bullets(player, sprite_sheet); // Desenha os tiros do jogador
			draw_enemies_bullets(enemies, n_enemies, sprite_sheet); // Desenha os tiros dos inimigos
			extra_enemy_event(enemies, n_enemies); // Evento do inimigo especial
			if(!enemies_alive(enemies, n_enemies)) // Verifica se todos os inimigos morreram
				next_round(round, player, enemies, n_enemies, obstacles, n_obstacles); // Passa para o próximo round
			al_flip_display(); // Atualiza a tela
		}else{ // Se o evento dentro da fila não for timer
			if((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_KEY_UP)){ // Verifica se teve alguma tecla pressionada
			if(event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT) // Verifica se a tecla A ou seta para esquerda foi pressionada
				joystick_left(player->control); // Indica o evento correspondente no controle do primeiro joagdor (movimento para esquerda)
			else if(event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT) // Verifica se a tecla D ou seta para direita foi pressionada
				joystick_right(player->control); // Indica o evento correspondente no controle do primeiro joagdor (movimento para direita)
			else if(event.keyboard.keycode == ALLEGRO_KEY_SPACE) joystick_fire(player->control); // Verifica se a tecla ESPAÇO foi pressionada, se sim, indica o evento correrspondente (atirar)
			}
		}
	}
}
// Libera a memória alocada para os elementos do jogo
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
// Gera o jogo
void generating_game(unsigned short program_event, unsigned short round, player* player, enemy** enemies, int n_enemies, obstacle** obstacles, int n_obstacles, hud* hud, ALLEGRO_FONT* font, ALLEGRO_BITMAP* sprite_sheet, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_DISPLAY* disp, ALLEGRO_EVENT event){
	unsigned short running = 1; // Variável que indica se o programa está rodando
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