#include <stdlib.h>
#include <stdio.h>

#include "player.h"

// Cria o player e seus componentes
player* create_player(int x, int y){
    player* new_player = (player*) malloc(sizeof(player)); // Aloca memória para o player
    if(new_player == 0){
        fprintf(stderr, "ERRO: não foi possível alocar memória para o jogador!\n");
        exit(1);
    }
	// Inicializa os componentes do player e seus atributos
    new_player->position_x = x;
    new_player->position_y = y;
    new_player->lives = 3;
    new_player->max_lives = 3;
    new_player->sprite_x = 0;
    new_player->sprite_y = 0;    
    new_player->score = 0;
    new_player->gun = pistol_create();
    new_player->control = joystick_create();
    return new_player;
}
// Move o player
void player_move(player *player, short trajectory, unsigned short max_x){
    if((player->position_x + (STEPS* trajectory) -16 > 0) && (player->position_x + (STEPS * trajectory) + 16  < max_x)) // Verifica se o player não vai sair da tela
        player->position_x = player->position_x + (STEPS * trajectory); // Move o player
} 
// Atira
void player_shot(player *player){
    bullet* shot;
    shot = pistol_shot(player->position_x, player->position_y - 16, -1, player->gun); // Cria o tiro
	player->gun->shots = shot; // Adiciona o tiro na lista de tiros do player
}
// Libera o espaço alocado para o player e seus componentes
void free_player(player* player){
    destroy_bullet_list(player->gun->shots);
    free(player->gun);
    free(player->control);
    free(player);
}
// Atualiza a pontuação do player
void player_score(int* score, int enemy_type){
	static int extra_multiplyer = 1; // Multiplicador de pontos para o inimigo especial
	switch(enemy_type){ // Aumenta a pontuação do player de acordo com o tipo de inimigo
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
// Atualiza a posição do player e desenha ele na tela
void update_player_position(player *player, ALLEGRO_BITMAP* sprite_sheet){
	if(player->control->left){	 // Verifica se o player está se movendo para a esquerda
		player_move(player, -1, 640);		
		if(player->sprite_x < 48) player->sprite_x += 16; // Muda o sprite do player
	}
	else 
	if(player->control->right){	// Verifica se o player está se movendo para a direita
		player_move(player, 1, 640);	
		if(player->sprite_x < 48) player->sprite_x += 16; // Muda o sprite do player
	
	}
	else
		player->sprite_x = 0; // Muda o sprite do player para o sprite parado
	
	if(!has_shot_column(player->gun->shots, player->position_x) && player->control->fire){ // Verifica se o player pode atirar e se ele quer atirar
		if(!player->gun->timer){ // Verifica se ele já pode atirar
			player_shot(player); // Atira
			player->gun->timer = PISTOL_COOLDOWN; // Reseta o tempo de recarga do tiro do player
		}	
	}
	al_draw_scaled_bitmap(sprite_sheet, player->sprite_x, player->sprite_y, 16, 16, player->position_x - 16, player->position_y - 16, 16 * 2, 16 * 2, 0); // Desenha o player
}
// Desenha os tiros do player
void draw_player_bullets(player* player, ALLEGRO_BITMAP* sprite_sheet){
	if(player->gun->timer) player->gun->timer--; // Diminui o tempo de recarga do tiro do jogador
	for(bullet* index = player->gun->shots; index != NULL; index = (bullet*) index->next) // Percorre a lista de tiros do player
		al_draw_scaled_bitmap(sprite_sheet, 64, 0, 16, 16, index->x - 14, index->y - 16, 16 * 2, 16 * 2, 0); // Desenha o tiro do player
}