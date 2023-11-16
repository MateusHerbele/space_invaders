#include <stdlib.h>
#include <stdio.h>

#include "enemy.h"
#include "Bullet.h"

enemy* create_enemy(int x, int y, int enemy_type){
	enemy* new_enemy = (enemy*) malloc(sizeof(enemy));
    if(new_enemy == 0){
        fprintf(stderr, "ERRO: não foi possível alocar memória para o jogador!\n");
        exit(1);
    }
	new_enemy->alive = 1;
    new_enemy->position_x = x;
    new_enemy->position_y = y;   
	new_enemy->direction = -1; // começam indo pra esquerda
	new_enemy->type = enemy_type;
	switch(enemy_type){
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
			new_enemy->sprite.x1 = 48;
			new_enemy->sprite.y1 = 48;
			new_enemy->sprite.x2 = 64;
			new_enemy->sprite.y2 = 48;
		break;
	}
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
    int y = 100;
    int index = 0;
    for(int i = 0; i < n_enemies; i++){
		if(y < 48 * 2 + 100)
        	enemies[index] = create_enemy(x, y, 2);
		else if(y < 48 * 4 + 100)
			enemies[index] = create_enemy(x, y, 1);
		else
			enemies[index] = create_enemy(x, y, 0);
        index++;
        y += 48;
        if(index % columns == 0){
            x += 48;
            y = 100;
        }
    }
    return enemies;
}

void update_enemies_position(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int max_x, int round){
    int speed = 16 + round; // define a velocidade do movimento
	static int animation_delay = 40;
	static int animation_counter = 0;

	// preciso descobrir oq está mais a esquerda e o que está mais a direita
	enemy* leftmost_enemy = enemies[0];
	enemy* rightmost_enemy = enemies[0];
	if(animation_counter % animation_delay == 0){
	for(int i = 1; i < n_enemies; i++){
    if(enemies[i]->position_x < leftmost_enemy->position_x && enemies[i]->alive){
        leftmost_enemy = enemies[i];
    }
    if(enemies[i]->position_x > rightmost_enemy->position_x && enemies[i]->alive){
        rightmost_enemy = enemies[i];
    }
	}
	if(leftmost_enemy->position_x <= 0 || rightmost_enemy->position_x >= max_x - 32){
		for(int i = 0; i < n_enemies; i++){
			// printf("tao na borda AAAAAAAAAA\n.");
			if(enemies[i]->alive){
			enemies[i]->direction *= -1;
			enemies[i]->position_x += speed * enemies[i]->direction;
			enemies[i]->position_y += 4;
			}
		}

	}else{
		for(int i = 0; i < n_enemies; i++){
			// printf("tao no meio\n.");
			if(enemies[i]->alive)
				enemies[i]->position_x += speed * enemies[i]->direction;
		}
	}
	
	for (int i = 0; i < n_enemies; i++) {
		if(!enemies[i]->alive) continue;
		if(animation_counter % (2 * animation_delay) < animation_delay ){
			al_draw_scaled_bitmap(sprite_sheet, enemies[i]->sprite.x1, enemies[i]->sprite.y1, 16, 16, enemies[i]->position_x, enemies[i]->position_y, 16 * 2, 16 * 2, 0);
		}else{
			al_draw_scaled_bitmap(sprite_sheet, enemies[i]->sprite.x2, enemies[i]->sprite.y2, 16, 16, enemies[i]->position_x, enemies[i]->position_y, 16 * 2, 16 * 2, 0);
		}
	}
	}else{
		for (int i = 0; i < n_enemies; i++) {		
			if(!enemies[i]->alive) continue;
			if(animation_counter % (2 * animation_delay) < animation_delay){
				al_draw_scaled_bitmap(sprite_sheet, enemies[i]->sprite.x1, enemies[i]->sprite.y1, 16, 16, enemies[i]->position_x, enemies[i]->position_y, 16 * 2, 16 * 2, 0);				
			}else{
				al_draw_scaled_bitmap(sprite_sheet, enemies[i]->sprite.x2, enemies[i]->sprite.y2, 16, 16, enemies[i]->position_x, enemies[i]->position_y, 16 * 2, 16 * 2, 0);
			}
	}
	}
	animation_counter = (animation_counter + 1) % (2 * animation_delay);
}

void enemy_shot(enemy *enemy){
    bullet* shot;
    shot = pistol_shot(enemy->position_x, enemy->position_y + 16, 1, enemy->gun);										//Quadrado atira para a esquerda (!)
	enemy->gun->shots = shot;
}

int enemies_alive(enemy** enemies, int n_enemies){
	int alive = 0;
	for(int i = 0; i < n_enemies; i++){
		if(enemies[i]->alive)
			alive++;
	}
	return alive;
}

void free_enemies(enemy** enemies, int n_enemies){
	for(int i = 0; i < n_enemies; i++){
		free(enemies[i]->gun);
		free(enemies[i]);
	}
	free(enemies);
}

void remove_enemy(enemy** enemies, int n_enemies, enemy* enemy){
// 	enemy* aux = NULL;
// 	for(int i = 0; i < n_enemies; i++){
// 		if(enemies[i] == enemy){
// 			free(enemies[i]);
// 			enemies[i] = NULL;
// 			break;
// 		}
// 		aux = enemies[i];
// 	}
// }
}

