#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																											//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>	
#include <allegro5/allegro_image.h>
#include <allegro5/timer.h>																																											//Biblioteca de primitivas do Allegro

#include "enemy.h"
#include "player.h"
#include "Joystick.h"

#define X_SCREEN 640																																														//Definição do tamanho da tela em pixels no eixo x
#define Y_SCREEN 640		

char is_nat(char *string){
	for (int i = 0; string[i]; i++) 
		if (!isdigit(string[i])) 
			return 0;
	return 1; 
}

void update_report(FILE *report, space *board, shot_sentinel *list, int r){
	fprintf(report, "============= ROUND %d =============\n=> MAP:\n", r);
	for (int i = 1; i <= board->max_y; i++){
		for (int j = 1; j <= board->max_x; j++){
			if (board->map[i][j].entity)
				fprintf(report, "| E ");
			else
				fprintf(report, "| 0 ");
		}
		fprintf(report, "|\n");
	}
	fprintf(report, "\n=> SHOTS:\n");
	for (shot *i = (shot*) list->first; i; i = (shot*) i->next) 
		fprintf(report, " - x: %u, y: %u\n", i->position_x, i->position_y);
	fprintf(report, "\n====================================\n\n");
}

void execute_event(space *board, shot_sentinel *list){
//IMPLEMENTAR!
//A cada evento:

//  Os tiros que não acertaram o alvo, ou não sairam do tabuleiro devem ser atualizados (movidos para frente no tabuleiro)
//  Os inimigos que não tem outros inimigos em sua frente devem atirar
	shot *current = NULL;
	unsigned int hasShot = 0; // variavel para verificar se já tem um tiro na coluna
	for (int i = 1; i <= board->max_y; i++){ 
		/*
		começa em 1, tanto i quanto j, pois os mapas são gerados seguindo essa mesma inicialização 
		no loop, assim, aqui faço o mesmo
		*/
		hasShot = 0; // reinicia o hasShot
		for (int j = 1; j <= board->max_x; j++){
			if (!board->map[i][j].entity) continue; // se não tiver uma entidade, continua
			if(i+1 <= board->max_y && board->map[i+1][j].entity) continue; // se tiver uma entidade na frente, continua
			if(board->map[i][j].type == SHIP || board->map[i][j].type == OBSTACLE) continue; // se não for um inimigo, continua
			current = list->first; // volta o current para o inicio da lista
			while(current != NULL){ // percorre a lista de tiros
				if((current->position_x == j)){ // se já tiver um tiro na mesma coluna
					hasShot = 1;
					break;
				} // se já tem um tiro na coluna sai do loop
				current = current->next;
			}
			if(!hasShot){ // caso não tenha adiciona na lista
				straight_shoot(board, list, board->map[i][j].entity);
			}
		}
	}
	update_shots(board, list);
}


// update position
void update_position(player *player){																																					//Função de atualização das posições dos quadrados conforme os comandos do controle
	if (player->control->left){																																											//Se o botão de movimentação para esquerda do controle do primeiro jogador está ativado...
		player_move(player, -1, X_SCREEN);		
		if(player->sprite_x < 32) player->sprite_x += 16;																																		//Move o quadrado do primeiro jogador para a esquerda
	}
	else 
	if (player->control->right){																																											//Se o botão de movimentação para direita do controle do primeir ojogador está ativado...
		player_move(player, 1, X_SCREEN);	
		if(player->sprite_x < 32) player->sprite_x += 16;																																		//Move o quadrado do primeiro jogador para a esquerda
	}else
		player->sprite_x = 0;
	// if (player->control->fire){																																											//Verifica se o primeiro jogador está atirando (!)
	// 	if (!player->gun->timer){																																											//Verifica se a arma do primeiro jogador não está em cooldown (!)
	// 		square_shot(player); 																																											//Se não estiver, faz um disparo (!)
	// 		player->gun->timer = PISTOL_COOLDOWN;																																							//Inicia o cooldown da arma (!)
	// 	} 
	// }
	// update_bullets(player);																																												//Atualiza os disparos do primeiro jogador (!)
}
int main(int argc, char** argv){
	player* player = create_player(X_SCREEN/2, Y_SCREEN/2, 3, 0, 0);
	// Funcoes allegro
	al_init();																																																//Faz a preparação de requisitos da biblioteca Allegro
	al_init_primitives_addon();																																												//Faz a inicialização dos addons das imagens básicas
	
	al_install_keyboard();																																													//Habilita a entrada via teclado (eventos de teclado), no programa

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);																																						//Cria o relógio do jogo; isso indica quantas atualizações serão realizadas por segundo (30, neste caso)
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();																																					//Cria a fila de eventos; todos os eventos (programação orientada a eventos) 
	ALLEGRO_FONT* font = al_create_builtin_font();																																							//Carrega uma fonte padrão para escrever na tela (é bitmap, mas também suporta adicionar fontes ttf)
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);			

	al_register_event_source(queue, al_get_keyboard_event_source());																																		//Indica que eventos de teclado serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_display_event_source(disp));																																		//Indica que eventos de tela serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_timer_event_source(timer));																																		//Indica que eventos de relógio serão inseridos na nossa fila de eventos

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
	int sprite_width = 16;
	int sprite_height = 16;


	while(1){
		// Laço principal do jogo
		al_wait_for_event(queue, &event);
		
		// Verifica se a tecla ESC está pressionada
		if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
			break;
    	}
		// Verifica se o botão de fechar foi pressionado
		if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			break;
		}
		if(player->lifes < 0){

		}else{
			if(event.type == ALLEGRO_EVENT_TIMER){
				update_position(player);
				al_clear_to_color(al_map_rgb(0, 0, 0));		
				al_draw_scaled_bitmap(sprite_sheet, player->sprite_x, player->sprite_y, sprite_width, sprite_height, player->position_x, player->position_y, sprite_width * 2, sprite_height * 2, 0);				
				al_flip_display();																																		
			}else{
				if ((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_KEY_UP)){																																				//Verifica se o evento é de botão do teclado abaixado ou levantado
				if (event.keyboard.keycode == ALLEGRO_KEY_A){
					joystick_left(player->control);
					// player->sprite_x = 16;
				} 																															//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à esquerda)
				else if (event.keyboard.keycode == ALLEGRO_KEY_D){
					joystick_right(player->control);
					// player->sprite_x = 16;
				} 																													//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à direita)
				else if (event.keyboard.keycode == 3) joystick_fire(player->control);																														//Indica o evento correspondente no controle do primeiro joagdor (botão de disparo - c) (!)					
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
