#include <stdlib.h>
#include "Bullet.h"
#include <stdio.h> // remover dps, só pra debug

bullet* bullet_create(int x, int y, char trajectory, bullet *next){			//Implementação da função "bullet_create"
	if ((trajectory < -1) || (trajectory > 1)) return NULL;
	bullet *new_bullet = (bullet*) malloc(sizeof(bullet));													//Aloca memória na heap para uma instância de projétil
	if (!new_bullet) return NULL;																			//Verifica o sucesso da alocação de memória; retorna NULL em caso de falha
	new_bullet->x = x;																						//Armazena a posição X informada
	new_bullet->y = y;																						//Armazena a posição Y informada
	new_bullet->trajectory = trajectory;																	//Armazena a trajetória informada
	new_bullet->next = (struct bullet*) next;																//Armazena o próximo projétil informado
	return new_bullet;																						//Retorna a instância criada de projétil
}

void bullet_move(bullet *elements){																			//Implementação da função "bullet_move"
	for (bullet *index = elements; index != NULL; index = (bullet*) index->next){							//Para cada projétil presente na lista de projéteis informada
		if (!index->trajectory) index->x = index->x - BULLET_MOVE;											//Se a trajetória for para a esquerda, movimenta um passo à esquerda
		else index->x = index->x + BULLET_MOVE;																//Se a trajetória for para a direita, movimenta um passo à direita
	}
}

int has_shot_column(bullet* elements, int column){															//Implementação da função "has_shot_column"
	for (bullet *index = elements; index != NULL; index = (bullet*) index->next){							//Para cada projétil presente na lista de projéteis informada
		if (index->x + 32 > column && index->x - 32 < column) return 1;										//Se a trajetória for para a esquerda, movimenta um passo à esquerda														//Se a trajetória for para a direita, movimenta um passo à direita
	}
	return 0;
}

void bullet_destroy(bullet *element){
		printf("bullet_destroy: x: %d, y: %d, endereco: %p.\n", element->x, element->y, element);
		printf("bullet_destroy: next: %p.\n", element->next);
        bullet *next = (bullet *)element->next;
        free(element);
        element = next;
}
