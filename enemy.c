#include <stdlib.h>
#include <stdio.h>

#include "enemy.h"
#include "Bullet.h"

// Cria um inimigo
enemy* create_enemy(int x, int y, int enemy_type){
	enemy* new_enemy = (enemy*) malloc(sizeof(enemy)); // Aloca memória para o inimigo
    if(new_enemy == 0){
        fprintf(stderr, "ERRO: não foi possível alocar memória para o jogador!\n");
        exit(1);
    }
	new_enemy->alive = 1; // Começa vivo
    new_enemy->position_x = x; // Define a posição inicial
    new_enemy->position_y = y; // Define a posição inicial
	new_enemy->direction = -1; // Começam indo pra esquerda
	new_enemy->type = enemy_type; // Define o tipo de inimigo
	switch(enemy_type){ // Define o sprite do inimigo
		case 0:
			new_enemy->sprite.x1 = 80;
			new_enemy->sprite.y1 = 16;
			new_enemy->sprite.x2 = 96;
			new_enemy->sprite.y2 = 16;
		break;
		case 1:
			new_enemy->sprite.x1 = 96;
			new_enemy->sprite.y1 = 32;
			new_enemy->sprite.x2 = 0;
			new_enemy->sprite.y2 = 48;
		break;
		case 2:
			new_enemy->sprite.x1 = 32;
			new_enemy->sprite.y1 = 32;
			new_enemy->sprite.x2 = 48;
			new_enemy->sprite.y2 = 32;
		break;
		case 3:
			new_enemy->alive = 0; // Começa morto se for o inimigo especial
			new_enemy->sprite.x1 = 48;
			new_enemy->sprite.y1 = 48;
			new_enemy->sprite.x2 = 64;
			new_enemy->sprite.y2 = 48;
		break;
	}
    new_enemy->gun = pistol_create(); // Cria a arma do inimigo
    return new_enemy;
}
// Cria um array de inimigos
enemy** create_enemies(int n_enemies, int columns, int lines){
    enemy** enemies = (enemy**) malloc(sizeof(enemy*) * n_enemies); // Aloca memória para o array de inimigos
    if(enemies == NULL){
        fprintf(stderr, "ERRO: não foi possível alocar memória para o array de inimigos!\n");
        exit(1);
    }
    int x = 0; // Posição inicial do primeiro inimigo
    int y = 100; // Posição inicial do primeiro inimigo
    for(int i = 0; i < n_enemies - 1;){ //  Itera sobre o array de inimigos
		if(y < 48 * 2 + 100) // Define o tipo de inimigo de acordo com a linha
        	enemies[i] = create_enemy(x, y, 2);
		else if(y < 48 * 4 + 100) // Define o tipo de inimigo de acordo com a linha
			enemies[i] = create_enemy(x, y, 1);
		else // Define o tipo de inimigo de acordo com a linha
			enemies[i] = create_enemy(x, y, 0);
        i++; // Incrementa o índice
        y += 48; // Incrementa a posição no eixo y
        if(i % columns == 0){ // Se o índice for múltiplo do número de colunas
            x += 48; // Incrementa a posição no eixo x
            y = 100; // Reseta a posição no eixo y
        }
    }
	enemies[n_enemies - 1] = create_enemy(640, 80, 3); // Cria o inimigo especial
    return enemies;
}
// Move o inimigo especial
void move_extra_enemy(enemy* extra_enemy, ALLEGRO_BITMAP* sprite_sheet){
	static int animation_delay = 20; // Define o delay da animação
	static int animation_counter = 0; // Contador da animação
	if(extra_enemy->alive){ // Se o inimigo estiver vivo
		if(extra_enemy->position_x < -32) // Se o inimigo passar da tela
			extra_enemy->alive = 0; // O inimigo morre
		
		extra_enemy->position_x += 2 * extra_enemy->direction; // Move o inimigo
		if(animation_counter % (2 * animation_delay) < animation_delay) // Se o resto da divisão do contador pelo delay * 2 for menor que o delay
			// al_draw_tinted_scaled_bitmap(sprite_sheet, al_map_rgba_f(255, 0, 0, 255), extra_enemy->sprite.x2, extra_enemy->sprite.y2, 16, 16, extra_enemy->position_x, extra_enemy->position_y, 16 * 3, 16 * 3, 0);
			al_draw_scaled_bitmap(sprite_sheet, extra_enemy->sprite.x2, extra_enemy->sprite.y2, 16, 16, extra_enemy->position_x, extra_enemy->position_y, 16 * 3, 16 * 3, 0); // Desenha o inimigo
		else // Se não
			// al_draw_tinted_scaled_bitmap(sprite_sheet, al_map_rgba_f(255, 0, 0, 255), extra_enemy->sprite.x1, extra_enemy->sprite.y1, 16, 16, extra_enemy->position_x, extra_enemy->position_y, 16 * 3, 16 * 3, 0);
			al_draw_scaled_bitmap(sprite_sheet, extra_enemy->sprite.x1, extra_enemy->sprite.y1, 16, 16, extra_enemy->position_x, extra_enemy->position_y, 16 * 3, 16 * 3, 0); // Desenha o inimigo

		animation_counter = (animation_counter + 1) % (2 * animation_delay); // Incrementa o contador
	}
}
// Move os inimigos
void update_enemies_position(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int max_x, unsigned short round){
    int speed = 16 + (round/4); // Define a velocidade do movimento
	static int animation_delay = 40; // Define o delay da animação
	static int animation_counter = 0; // Contador da animação

	// Descobre o que está mais a esquerda e o que está mais a direita
	enemy* leftmost_enemy = enemies[0]; // Define o inimigo mais a esquerda
	enemy* rightmost_enemy = enemies[0]; // Define o inimigo mais a direita
	if(animation_counter % animation_delay == 0){ // Se o resto da divisão do contador pelo delay for 0
		for(int i = 1; i < n_enemies - 1; i++){ // Itera sobre o array de inimigos
		if(enemies[i]->position_x < leftmost_enemy->position_x && enemies[i]->alive)
			leftmost_enemy = enemies[i]; // Define o inimigo mais a esquerda
		
		if(enemies[i]->position_x > rightmost_enemy->position_x && enemies[i]->alive)
			rightmost_enemy = enemies[i]; // Define o inimigo mais a direita
		
		}
		if(leftmost_enemy->position_x <= 0 || rightmost_enemy->position_x >= max_x - 32){ // Se o inimigo mais a esquerda chegarem nos limites estabelecidos
			for(int i = 0; i < n_enemies - 1; i++){ // Itera sobre o array de inimigos
				if(enemies[i]->alive){ // Se o inimigo estiver vivo
				enemies[i]->direction *= -1; // Inverte a direção do inimigo
				enemies[i]->position_x += speed * enemies[i]->direction; // Move o inimigo
				enemies[i]->position_y += 4; // Move o inimigo
				}
			}
		}else{ // Se não, só continua movendo eles na direção que já estão
			for(int i = 0; i < n_enemies - 1; i++){
				if(enemies[i]->alive)
					enemies[i]->position_x += speed * enemies[i]->direction;
			}
		}
		// Desenha os inimigos
		for(int i = 0; i < n_enemies - 1; i++){
			if(!enemies[i]->alive) continue; // Se o inimigo estiver morto, pula para o próximo
			if(animation_counter % (2 * animation_delay) < animation_delay){ // Verifica qual sprite desenhar
				al_draw_scaled_bitmap(sprite_sheet, enemies[i]->sprite.x1, enemies[i]->sprite.y1, 16, 16, enemies[i]->position_x, enemies[i]->position_y, 16 * 2, 16 * 2, 0);
			}else{
				al_draw_scaled_bitmap(sprite_sheet, enemies[i]->sprite.x2, enemies[i]->sprite.y2, 16, 16, enemies[i]->position_x, enemies[i]->position_y, 16 * 2, 16 * 2, 0);
			}
		}
	}else{ // Se não, só desenha os inimigos na posição que estão
		for(int i = 0; i < n_enemies - 1; i++){		
			if(!enemies[i]->alive) continue; // Se o inimigo estiver morto, pula para o próximo
			if(animation_counter % (2 * animation_delay) < animation_delay){ // Verifica qual sprite desenhar
				al_draw_scaled_bitmap(sprite_sheet, enemies[i]->sprite.x1, enemies[i]->sprite.y1, 16, 16, enemies[i]->position_x, enemies[i]->position_y, 16 * 2, 16 * 2, 0);				
			}else{
				al_draw_scaled_bitmap(sprite_sheet, enemies[i]->sprite.x2, enemies[i]->sprite.y2, 16, 16, enemies[i]->position_x, enemies[i]->position_y, 16 * 2, 16 * 2, 0);
			}
	}
	}
	animation_counter = (animation_counter + 1) % (2 * animation_delay); // Incrementa o contador
	move_extra_enemy(enemies[n_enemies - 1], sprite_sheet); // Move o inimigo especial
}
// Faz o inimigo atirar
void enemy_shot(enemy *enemy){
	if(enemy == NULL) return; // Se o inimigo for nulo, não atira
	if(!enemy->alive) return; // Se o inimigo estiver morto, não atira
    bullet* shot; // Cria uma bala
    shot = pistol_shot(enemy->position_x, enemy->position_y + 16, 1, enemy->gun); // Cria uma bala
	enemy->gun->shots = shot; // Adiciona a bala na lista de balas
}
// Retorna quantos inimigos estão vivos
int enemies_alive(enemy** enemies, int n_enemies){
	int alive = 0;
	for(int i = 0; i < n_enemies; i++){
		if(enemies[i]->alive)
			alive++;
	}
	return alive;
}
// Libera a memória alocada para os inimigos e suas balas e armas
void free_enemies(enemy** enemies, int n_enemies){
	for(int i = 0; i < n_enemies; i++){
		if(enemies[i]->gun->shots != NULL)
			destroy_bullet_list(enemies[i]->gun->shots);
		free(enemies[i]->gun);
		free(enemies[i]);
	}
	free(enemies);
}
// Verifica se algum inimigo atirou na coluna
int enemy_has_shot_column(int column, enemy** enemies, int n_enemies){	
	for(int i = 0; i < n_enemies; i++)
		for(bullet* index = enemies[i]->gun->shots; index != NULL; index = (bullet*) index->next)
			if(index->x + 32 > column && index->x - 32 < column) return 1;
		
	return 0;	
}
// Verifica se algum inimigo está na frente de outro inimigo
int enemy_in_front_of_enemy(enemy* current_enemy, enemy** enemies, int n_enemies){
	for(int i = 0; i < n_enemies; i++){
		if(enemies[i]->position_x == current_enemy->position_x && enemies[i]->position_y > current_enemy->position_y)
			return 1;
	}
	return 0;
}
// Retorna a posição y do inimigo mais abaixo
int bottommost_enemy(enemy** enemies, int n_enemies){
	int bottommost = 0;
	for(int i = 0; i < n_enemies; i++){
		if(enemies[i]->position_y > bottommost)
			bottommost = enemies[i]->position_y;
	}
	return bottommost;
}
// Evento do inimigo especial
void extra_enemy_event(enemy** enemies, int n_enemies){
	if(enemies[n_enemies - 1]->alive) return; // Se o inimigo especial estiver vivo, não faz nada
	srand(time(NULL)); // Seta a seed do rand
	int random = rand() % 100; // Gera um número aleatório
	if(random == 0){ // Se o número aleatório for 0, aciona o evento
		enemies[n_enemies - 1]->position_x = 640;
		enemies[n_enemies - 1]->position_y = 65;
		enemies[n_enemies - 1]->alive = 1;
	}
}
// Retorna a distância entre dois pontos
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
// Atualiza os tiros dos inimigos
void update_enemies_shots(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int player_x, int player_y, unsigned short round){
	static int shot_delay_0 = 170; // Valor pra não começar o round com tiros
	static int shot_delay_1 = 200; // Valor pra não começar o round com tiros	
	int actual_distance = 0; // Distância atual
	int closest_distance_0 = 1000; // valor arbitrário para iniciar a comparação
	int closest_distance_1 = 1000; // valor arbitrário para iniciar a comparação
	enemy* closest_enemy_0 = NULL; // Inimigo mais próximo
	enemy* closest_enemy_1 = NULL; // Segundo inimigo mais próximo

	for(int i = 0; i < n_enemies - 1; i++){ // Itera sobre o array de inimigos
		if(enemies[i]->alive) // Se o inimigo estiver vivo
			switch(enemies[i]->type){ // Verifica o tipo de inimigo e realiza a ação correspondente, e salva os inimigos mais próximos
				case 0:
					if(enemy_has_shot_column(enemies[i]->position_x, enemies, n_enemies) ||  enemy_in_front_of_enemy(enemies[i], enemies, n_enemies))
						continue;
					else{
						actual_distance = two_points_distance(enemies[i]->position_x, player_x, enemies[i]->position_y, player_y);
						if(actual_distance < closest_distance_1){
							if(actual_distance < closest_distance_0){
								closest_distance_1 = closest_distance_0;
								closest_enemy_1 = closest_enemy_0;
								closest_distance_0 = actual_distance;
								closest_enemy_0 = enemies[i];
							}else{
								closest_distance_1 = actual_distance;
								closest_enemy_1 = enemies[i];
							}
						}
					}
				break;
				case 1:
					if(enemy_has_shot_column(enemies[i]->position_x, enemies, n_enemies))
						continue;
					else{
						actual_distance = two_points_distance(enemies[i]->position_x, player_x, enemies[i]->position_y, player_y);
						if(actual_distance < closest_distance_1){
							if(actual_distance < closest_distance_0){
								closest_distance_1 = closest_distance_0;
								closest_enemy_1 = closest_enemy_0;
								closest_distance_0 = actual_distance;
								closest_enemy_0 = enemies[i];
							}else{
								closest_distance_1 = actual_distance;
								closest_enemy_1 = enemies[i];
							}
						}
					}
				break;
				case 2:
					actual_distance = two_points_distance(enemies[i]->position_x, player_x, enemies[i]->position_y, player_y);
					if(actual_distance < closest_distance_1){
						if(actual_distance < closest_distance_0){
							closest_distance_1 = closest_distance_0;
							closest_enemy_1 = closest_enemy_0;
							closest_distance_0 = actual_distance;
							closest_enemy_0 = enemies[i];
						}else{
							closest_distance_1 = actual_distance;
							closest_enemy_1 = enemies[i];
						}
					}
				break;
			}
	}
	if(shot_delay_0 <= 0){ // Se o delay do tiro for 0, atira
		enemy_shot(closest_enemy_0); // Atira
		shot_delay_0 = ENEMY_SHOT_COOLDOWN + 20 - (round/2); // Reseta o delay
	}else shot_delay_0--; // Se não, decrementa o delay
	
	if(shot_delay_1 <= 0 && closest_enemy_1){ // Se o delay do tiro for 0 e closest_enemy_1 for diferente de nulo, atira
		enemy_shot(closest_enemy_1);// Atira
		shot_delay_1 = ENEMY_SHOT_COOLDOWN + 10 - (round/2); // Reseta o delay
	}else shot_delay_1--; // Se não, decrementa o delay
}
// Desenha os tiros dos inimigos
void draw_enemies_bullets(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet){
	for(int i = 0; i < n_enemies; i++){ // Itera sobre o array de inimigos
	if(enemies[i]->gun->timer) enemies[i]->gun->timer--; // Se o timer da arma for maior que 0, decrementa
	for(bullet* index = enemies[i]->gun->shots; index != NULL; index = (bullet*) index->next) // Itera sobre a lista de balas
		switch(enemies[i]->type){ // Verifica o tipo de inimigo e desenha a bala correspondente
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
