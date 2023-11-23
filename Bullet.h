#ifndef __BULLET__ 																																//Guardas de inclusão (!)
#define __BULLET__																																//Guardas de inclusão (!)

#define BULLET_MOVE 5
#define BULLET_COOLDOWN 0

typedef struct {																																//Definição da estrutura de um pŕojétil (!)
	int x;																																		//Local, no eixo x, onde se localiza a bala (!)
	int y;																																		//Local, no eixo y, onde se localiza a bala (!)
	unsigned char trajectory;				
	struct bullet *next; 																														//Próxima bala, se houver, formando uma lista encadeada (!)
} bullet;																																		//Definição do nome da estrutura (!)

bullet* bullet_create(int x, int y, char trajectory, bullet *next);																				//Protótipo da função de criação de uma bala (!)
void bullet_move(bullet *elements);																												//Protótipo da função de movimentação de uma bala (!)				
int has_shot_column(bullet* elements, int column); // Verifica se existe uma bala na coluna especificada
void bullet_destroy(bullet* element);																											//Protótipo da função de destruição de uma bala (!)
void destroy_bullet_list(bullet *elements); // Destroi a lista de balas																									

#endif																																			//Guardas de inclusão (!)