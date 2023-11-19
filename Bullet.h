#ifndef __BULLET__ 																																//Guardas de inclusão (!)
#define __BULLET__																																//Guardas de inclusão (!)

#define BULLET_MOVE 5
#define BULLET_COOLDOWN 0

typedef struct {																																//Definição da estrutura de um pŕojétil (!)
	int x;																															//Local, no eixo x, onde se localiza a bala (!)
	int y;																															//Local, no eixo y, onde se localiza a bala (!)
	unsigned char trajectory;				
	struct bullet *next; 																														//Próxima bala, se houver, formando uma lista encadeada (!)
} bullet;																																		//Definição do nome da estrutura (!)

bullet* bullet_create(int x, int y, char trajectory, bullet *next);												//Protótipo da função de criação de uma bala (!)
void bullet_move(bullet *elements);			
int has_shot_column(bullet* elements, int column);																									//Protótipo da função de movimentação de uma bala (!)	
void bullet_destroy(bullet* element);																											//Protótipo da função de destruição de uma bala (!)
void enemy_bullet_destroy(bullet **element_ptr);
void destroy_bullet_list(bullet *elements);

#endif																																			//Guardas de inclusão (!)