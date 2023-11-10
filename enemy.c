#include <stdlib.h>
#include <stdio.h>

#include "enemy.h"
#include "Bullet.h"

// shot_sentinel* create_shotlist(void){
// 	shot_sentinel *list = (shot_sentinel*) malloc (sizeof(shot_sentinel));
// 	if (list == NULL) return NULL;
// 	list->first = NULL;
// 	list->last  = NULL;
// 	return list;
// }

// shot* remove_shot(shot* current, shot* previous, shot_sentinel *list){
// //IMPLEMENTAR!
// //	Remove os tiros da lista
// 	if(current == list->first){ // se o tiro for o primeiro da lista
// 		list->first = current->next;
// 		free(current);
// 		return list->first;
// 	}
// 	if(current == list->last){ // se o tiro for o ultimo da lista
// 		list->last = previous;
// 		previous->next = NULL;
// 		free(current);
// 		return NULL;
// 	} // se não, remove o tiro do meio da lista
// 	previous->next = current->next;
// 	free(current);
// 	return previous->next;
// }

// void clean_shots(shot_sentinel *list){
// 	shot *p = (shot*) list->first;
// 	shot *q = NULL;
// 	while (p != NULL)
// 		p = remove_shot(p, q, list);
// }

// void update_shots(space *board, shot_sentinel *list){
// //IMPLEMENTAR!
// //Os tiros presentes no tabuleiro devem ser atualizados
// //  Se o tiro acertar um alvo, ou sair do tabuleiro, ele deve ser removido da lista
// //  Caso contrário, ele deve "andar" uma casa (sqm) à frente
// shot *current = list->first;
// while(current != NULL){
// 	if(current->position_y == board->max_y){ // se o tiro estiver na ultima linha
// 			current = remove_shot(current, list->first, list);// remove o tiro
// 	}else{
// 			current->position_y++; // se não, anda uma casa para frente
// 			current = current->next;
// 		} 
// 	}
// }

// shot* straight_shoot(space *board, shot_sentinel *list, enemy *shooter){
// //IMPLEMENTAR!
// //Adiciona um novo tiro à lista. Neste momento, todos os tiros se movem apenas para frente
// 	shot *newShot = malloc(sizeof(shot));
// 	if(!newShot){
// 		fprintf(stderr, "Falha ao alocar memória para um novo tiro.\n");
// 		exit(1);
// 	}
// 	if(!list->first){ // se a lista estiver vazia
// 		newShot->position_x = shooter->position_x;
// 		newShot->position_y = shooter->position_y;
// 		list->first = newShot;
// 		list->last = newShot;
// 		newShot->next = NULL;
// 		return newShot;
// 	}
// 	newShot->position_x = shooter->position_x;
// 	newShot->position_y = shooter->position_y;
// 	list->last->next = newShot;
// 	list->last = newShot;
// 	newShot->next = NULL;
// 	return newShot;
// }

enemy* create_enemy(int x, int y){
//IMPLEMENTAR!
//Adiciona um inimigo no tabuleiro. Essa tarefa inclui a alocação do mesmo
	// if(board->max_x < position_x || board->max_y < position_y || position_x < 0 || position_y < 0){
	// 	fprintf(stderr, "Posição inválida dentro do espaço.\n");
	// 	return 1;
	// } // verifica se a posição é válida
	// enemy *newEnemy = malloc(sizeof(enemy));
	// if(newEnemy == NULL){
	// 	fprintf(stderr, "Falha ao alocar memória para um novo inimigo.\n");
	// 	exit(1);
	// }
	// newEnemy->position_x = position_x;
	// newEnemy->position_y = position_y;
	// board->map[position_y][position_x].type = ENEMY;
	// board->map[position_y][position_x].entity = newEnemy;
	// return 0;
	enemy* new_enemy = (enemy*) malloc(sizeof(enemy));
    if(new_enemy == 0){
        fprintf(stderr, "ERRO: não foi possível alocar memória para o jogador!\n");
        exit(1);
    }
    new_enemy->position_x = x;
    new_enemy->position_y = y;
    new_enemy->sprite_x = 80;
    new_enemy->sprite_y = 16;    
	new_enemy->direction = 1;
    new_enemy->gun = pistol_create();
    return new_enemy;
}

enemy** create_enemies(int n_enemies, int columns, int lines){
    enemy** enemies = (enemy**) malloc(sizeof(enemy*) * n_enemies);
    if(enemies == NULL){
        fprintf(stderr, "ERRO: não foi possível alocar memória para o array de inimigos!\n");
        exit(1);
    }
    int x = 0;
    int y = 0;
    int index = 0;
    for(int i = 0; i < n_enemies; i++){
        enemies[index] = create_enemy(x, y);
        index++;
        y += 32;
        if(index % columns == 0){
            x += 32;
            y = 0;
        }
    }
    return enemies;
}

void generate_enemies(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int max_x){
	for(int i = 0; i < n_enemies; i++){
		al_draw_scaled_bitmap(sprite_sheet, enemies[i]->sprite_x, enemies[i]->sprite_y, 16, 16, enemies[i]->position_x + max_x/4, enemies[i]->position_y, 16 * 2, 16 * 2, 0);		
	}
}

void update_enemies_position(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int max_x){
    int speed = 2; // define a velocidade do movimento
	
	if(enemies[0]->position_x <= 32 || enemies[n_enemies-1]->position_x >= max_x - 32){
		for(int i = 0; i < n_enemies; i++){
			enemies[i]->direction *= -1;
			enemies[i]->position_x += speed * enemies[i]->direction;
			al_draw_scaled_bitmap(sprite_sheet, enemies[i]->sprite_x, enemies[i]->sprite_y, 16, 16, enemies[i]->position_x + max_x/4, enemies[i]->position_y, 16 * 2, 16 * 2, 0);	
		}
	}else{
		for(int i = 0; i < n_enemies; i++){
			enemies[i]->position_x += speed * enemies[i]->direction;
			al_draw_scaled_bitmap(sprite_sheet, enemies[i]->sprite_x, enemies[i]->sprite_y, 16, 16, enemies[i]->position_x + max_x/4, enemies[i]->position_y, 16 * 2, 16 * 2, 0);		
		}
	}
	//generate_enemies(enemies, n_enemies, sprite_sheet, max_x);
    // for(int i = 0; i < n_enemies; i++){
    //     if(enemies[i]->position_x <= 0 || enemies[i]->position_x >= max_x - 32){
    //         enemies[i]->direction *= -1; // inverte a direção do movimento
    //     }
    //     enemies[i]->position_x += speed * enemies[i]->direction; // atualiza a posição x
    //     al_draw_scaled_bitmap(sprite_sheet, enemies[i]->sprite_x, enemies[i]->sprite_y, 16, 16, enemies[i]->position_x, enemies[i]->position_y, 16 * 2, 16 * 2, 0); // desenha o inimigo na tela
//    }
}
int remove_enemy(space *board, int position_y, int position_x){
//IMPLEMENTAR!
//Remove um inimigo do tabuleiro. Essa tarefa inclui a desalocação do mesmo
	// if(board->max_x < position_x || board->max_y < position_y || position_x < 0 || position_y < 0){
	// 	fprintf(stderr, "Posição inválida dentro do espaço.\n");
	// 	return 1;
	// } // verifica se a posição é válida
	// free(board->map[position_y][position_x].entity);
	// board->map[position_y][position_x].entity = NULL;
	return 0;
}
