#include "game.h"

int main(int argc, char** argv){
	int n_enemies = 66;
	int n_obstacles = 4;
	int round = 0;
	player* player = create_player(X_SCREEN/2, Y_SCREEN - 16);
	enemy** enemies =  create_enemies(n_enemies, 6, 11);
	obstacle** obstacles = create_obstacles(n_obstacles, X_SCREEN, Y_SCREEN);
	hud* hud = create_hud();
	// Funcoes allegro
	al_init();																																																//Faz a preparação de requisitos da biblioteca Allegro
	al_init_primitives_addon();																																												//Faz a inicialização dos addons das imagens básicas
	
	al_install_keyboard();																																													//Habilita a entrada via teclado (eventos de teclado), no programa
	al_init_font_addon();
	al_init_ttf_addon();


	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);																																						//Cria o relógio do jogo; isso indica quantas atualizações serão realizadas por segundo (30, neste caso)
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();																																					//Cria a fila de eventos; todos os eventos (programação orientada a eventos) 
	// ALLEGRO_FONT* font = al_create_builtin_font();		
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
	ALLEGRO_FONT* font = al_load_font("fonts/Arcade.ttf", 32, 0);
	if(!font){
		fprintf(stderr, "Falha ao carregar fonte!\n");
		exit(1);
	}

	

	// font = al_load_font("fonts/PressStart2P.ttf", 16, 0);
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
			generate_hud(hud, player, round, sprite_sheet, font);
			explosion_animation(player->position_x - 16, player->position_y -16, sprite_sheet);
			al_flip_display();
			break;
		}else{
			if(event.type == ALLEGRO_EVENT_TIMER){
				update_position(player, enemies, n_enemies);
				al_clear_to_color(al_map_rgb(0, 0, 0));		
				generate_hud(hud, player, round, sprite_sheet, font);
    			// textprintf_ex(disp, font, 16, 16, makecol(0,0,255), -1, "SCORE:");
				al_draw_scaled_bitmap(sprite_sheet, player->sprite_x, player->sprite_y, sprite_width, sprite_height, player->position_x - 16, player->position_y - 16, sprite_width * 2, sprite_height * 2, 0);		
				generate_obstacles(obstacles, 4, sprite_sheet);
				update_enemies_position(enemies, n_enemies, sprite_sheet, X_SCREEN, round);
				update_enemies_shots(enemies, n_enemies, sprite_sheet, player->position_x, player->position_y, round);
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
