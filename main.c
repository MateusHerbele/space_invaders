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
#include "Pistol.h"
#include "Bullet.h"

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

void update_bullets(player *player){																																										//Implementação da função que atualiza o posicionamento de projéteis conforme o movimento dos mesmos (!)
	bullet *previous = NULL;																																												//Variável auxiliar para salvar a posição imediatamente anterior na fila (!)
	for (bullet *index = player->gun->shots; index != NULL;){																																				//Para cada projétil presente na lista de projéteis disparados (!)
		index->y -= BULLET_MOVE;																																											//Atualiza a posição do projétil (!)
		// if (!index->trajectory) index->x -= BULLET_MOVE;																																					//Se a trajetória for para a esquerda, atualiza a posição para a esquerda (!)
		// else if (index->trajectory == 1) index->x += BULLET_MOVE;																																			//Se a trajetória for para a direita, atualiza a posição para a esquerda (!)
		
		if ((index->y < 0) || (index->y > Y_SCREEN)){																																						//Verifica se o projétil saiu das bordas da janela (!)
			if (previous){																																													//Verifica se não é o primeiro elemento da lista de projéteis (!)
				previous->next = index->next;																																								//Se não for, salva o próximo projétil (!)
				bullet_destroy(index);																																										//Chama o destrutor para o projétil atual (!)
				index = (bullet*) previous->next;																																							//Atualiza para o próximo projétil (!)
			}
			else {																																															//Se for o primeiro projétil da lista (!)
				player->gun->shots = (bullet*) index->next;																																					//Atualiza o projétil no início da lista (!)
				bullet_destroy(index);																																										//Chama o destrutor para o projétil atual (!)
				index = player->gun->shots;																																									//Atualiza para o próximo projétil (!)
			}
		}
		else{																																																//Se não saiu da tela (!)
			previous = index;																																												//Atualiza o projétil anterior (para a próxima iteração) (!)
			index = (bullet*) index->next;																																									//Atualiza para o próximo projétil (!)
		}
	}
}
// update position
void update_position(player *player){																																					//Função de atualização das posições dos quadrados conforme os comandos do controle
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
	// if (player->control->fire){																																											//Verifica se o primeiro jogador está atirando (!)
	// 	if (!player->gun->timer){																																											//Verifica se a arma do primeiro jogador não está em cooldown (!)
	// 		square_shot(player); 																																											//Se não estiver, faz um disparo (!)
	// 		player->gun->timer = PISTOL_COOLDOWN;																																							//Inicia o cooldown da arma (!)
	// 	} 
	// }
	update_bullets(player);																																												//Atualiza os disparos do primeiro jogador (!)
}
int main(int argc, char** argv){
	player* player = create_player(X_SCREEN/2, Y_SCREEN - 16, 3, 0);
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
	ALLEGRO_BITMAP* icon = al_load_bitmap("sprites/icon.png");
	if(!icon){
		fprintf(stderr, "Falha ao carregar o ícone do jogo!\n");
		exit(1);
	}
	al_set_display_icon(disp, icon);
	
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
		if(player->lifes < 0){

		}else{
			if(event.type == ALLEGRO_EVENT_TIMER){
				update_position(player);
				al_clear_to_color(al_map_rgb(0, 0, 0));		
				al_draw_scaled_bitmap(sprite_sheet, player->sprite_x, player->sprite_y, sprite_width, sprite_height, player->position_x - 16, player->position_y - 16, sprite_width * 2, sprite_height * 2, 0);				
				for (bullet *index = player->gun->shots; index != NULL; index = (bullet*) index->next) {
					// printf("entrei aqui\n");
					// index->y -= BULLET_MOVE;
					al_draw_scaled_bitmap(sprite_sheet, 64, 0, sprite_width, sprite_height, index->x - 14 , index->y - 16, sprite_width * 2, sprite_height * 2, 0);							
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
