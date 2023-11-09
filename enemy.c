#include <stdlib.h>
#include <stdio.h>

#include "enemy.h"
#include "Bullet.h"

shot_sentinel* create_shotlist(void){
	shot_sentinel *list = (shot_sentinel*) malloc (sizeof(shot_sentinel));
	if (list == NULL) return NULL;
	list->first = NULL;
	list->last  = NULL;
	return list;
}

shot* remove_shot(shot* current, shot* previous, shot_sentinel *list){
//IMPLEMENTAR!
//	Remove os tiros da lista
	if(current == list->first){ // se o tiro for o primeiro da lista
		list->first = current->next;
		free(current);
		return list->first;
	}
	if(current == list->last){ // se o tiro for o ultimo da lista
		list->last = previous;
		previous->next = NULL;
		free(current);
		return NULL;
	} // se não, remove o tiro do meio da lista
	previous->next = current->next;
	free(current);
	return previous->next;
}

void clean_shots(shot_sentinel *list){
	shot *p = (shot*) list->first;
	shot *q = NULL;
	while (p != NULL)
		p = remove_shot(p, q, list);
}

void update_shots(space *board, shot_sentinel *list){
//IMPLEMENTAR!
//Os tiros presentes no tabuleiro devem ser atualizados
//  Se o tiro acertar um alvo, ou sair do tabuleiro, ele deve ser removido da lista
//  Caso contrário, ele deve "andar" uma casa (sqm) à frente
shot *current = list->first;
while(current != NULL){
	if(current->position_y == board->max_y){ // se o tiro estiver na ultima linha
			current = remove_shot(current, list->first, list);// remove o tiro
	}else{
			current->position_y++; // se não, anda uma casa para frente
			current = current->next;
		} 
	}
}

shot* straight_shoot(space *board, shot_sentinel *list, enemy *shooter){
//IMPLEMENTAR!
//Adiciona um novo tiro à lista. Neste momento, todos os tiros se movem apenas para frente
	shot *newShot = malloc(sizeof(shot));
	if(!newShot){
		fprintf(stderr, "Falha ao alocar memória para um novo tiro.\n");
		exit(1);
	}
	if(!list->first){ // se a lista estiver vazia
		newShot->position_x = shooter->position_x;
		newShot->position_y = shooter->position_y;
		list->first = newShot;
		list->last = newShot;
		newShot->next = NULL;
		return newShot;
	}
	newShot->position_x = shooter->position_x;
	newShot->position_y = shooter->position_y;
	list->last->next = newShot;
	list->last = newShot;
	newShot->next = NULL;
	return newShot;
}

int add_enemy(space *board, int position_y, int position_x){
//IMPLEMENTAR!
//Adiciona um inimigo no tabuleiro. Essa tarefa inclui a alocação do mesmo
	if(board->max_x < position_x || board->max_y < position_y || position_x < 0 || position_y < 0){
		fprintf(stderr, "Posição inválida dentro do espaço.\n");
		return 1;
	} // verifica se a posição é válida
	enemy *newEnemy = malloc(sizeof(enemy));
	if(newEnemy == NULL){
		fprintf(stderr, "Falha ao alocar memória para um novo inimigo.\n");
		exit(1);
	}
	newEnemy->position_x = position_x;
	newEnemy->position_y = position_y;
	board->map[position_y][position_x].type = ENEMY;
	board->map[position_y][position_x].entity = newEnemy;
	return 0;
}

int remove_enemy(space *board, int position_y, int position_x){
//IMPLEMENTAR!
//Remove um inimigo do tabuleiro. Essa tarefa inclui a desalocação do mesmo
	if(board->max_x < position_x || board->max_y < position_y || position_x < 0 || position_y < 0){
		fprintf(stderr, "Posição inválida dentro do espaço.\n");
		return 1;
	} // verifica se a posição é válida
	free(board->map[position_y][position_x].entity);
	board->map[position_y][position_x].entity = NULL;
	return 0;
}
