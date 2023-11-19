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
			new_enemy->alive = 0;
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
    for(int i = 0; i < n_enemies - 1; i++){
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
	enemies[n_enemies - 1] = create_enemy(640, 80, 3);
    return enemies;
}

void move_extra_enemy(enemy* extra_enemy, ALLEGRO_BITMAP* sprite_sheet){
	static int animation_delay = 20;
	static int animation_counter = 0;
	if(extra_enemy->alive){
		if(extra_enemy->position_x < -32){
			extra_enemy->position_x = 640;
			// extra_enemy->alive = 0;
		}
		extra_enemy->position_x += 2 * extra_enemy->direction;
		if(animation_counter % (2 * animation_delay) < animation_delay)
			// al_draw_tinted_scaled_bitmap(sprite_sheet, al_map_rgba_f(255, 0, 0, 255), extra_enemy->sprite.x2, extra_enemy->sprite.y2, 16, 16, extra_enemy->position_x, extra_enemy->position_y, 16 * 3, 16 * 3, 0);
			al_draw_scaled_bitmap(sprite_sheet, extra_enemy->sprite.x2, extra_enemy->sprite.y2, 16, 16, extra_enemy->position_x, extra_enemy->position_y, 16 * 3, 16 * 3, 0);
		else
			// al_draw_tinted_scaled_bitmap(sprite_sheet, al_map_rgba_f(255, 0, 0, 255), extra_enemy->sprite.x1, extra_enemy->sprite.y1, 16, 16, extra_enemy->position_x, extra_enemy->position_y, 16 * 3, 16 * 3, 0);
			al_draw_scaled_bitmap(sprite_sheet, extra_enemy->sprite.x1, extra_enemy->sprite.y1, 16, 16, extra_enemy->position_x, extra_enemy->position_y, 16 * 3, 16 * 3, 0);

		animation_counter = (animation_counter + 1) % (2 * animation_delay);
	}
}

void update_enemies_position(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int max_x, unsigned short round){
    int speed = 16 + (round/4); // define a velocidade do movimento
	static int animation_delay = 40;
	static int animation_counter = 0;

	// Descobre o que está mais a esquerda e o que está mais a direita
	enemy* leftmost_enemy = enemies[0];
	enemy* rightmost_enemy = enemies[0];
	if(animation_counter % animation_delay == 0){
	for(int i = 1; i < n_enemies - 1; i++){
    if(enemies[i]->position_x < leftmost_enemy->position_x && enemies[i]->alive){
        leftmost_enemy = enemies[i];
    }
    if(enemies[i]->position_x > rightmost_enemy->position_x && enemies[i]->alive){
        rightmost_enemy = enemies[i];
    }
	}
	if(leftmost_enemy->position_x <= 0 || rightmost_enemy->position_x >= max_x - 32){
		for(int i = 0; i < n_enemies - 1; i++){
			if(enemies[i]->alive){
			enemies[i]->direction *= -1;
			enemies[i]->position_x += speed * enemies[i]->direction;
			enemies[i]->position_y += 4;
			}
		}

	}else{
		for(int i = 0; i < n_enemies - 1; i++){
			if(enemies[i]->alive)
				enemies[i]->position_x += speed * enemies[i]->direction;
		}
	}
	
	for (int i = 0; i < n_enemies - 1; i++) {
		if(!enemies[i]->alive) continue;
		if(animation_counter % (2 * animation_delay) < animation_delay ){
			al_draw_scaled_bitmap(sprite_sheet, enemies[i]->sprite.x1, enemies[i]->sprite.y1, 16, 16, enemies[i]->position_x, enemies[i]->position_y, 16 * 2, 16 * 2, 0);
		}else{
			al_draw_scaled_bitmap(sprite_sheet, enemies[i]->sprite.x2, enemies[i]->sprite.y2, 16, 16, enemies[i]->position_x, enemies[i]->position_y, 16 * 2, 16 * 2, 0);
		}
	}
	}else{
		for (int i = 0; i < n_enemies - 1; i++) {		
			if(!enemies[i]->alive) continue;
			if(animation_counter % (2 * animation_delay) < animation_delay){
				al_draw_scaled_bitmap(sprite_sheet, enemies[i]->sprite.x1, enemies[i]->sprite.y1, 16, 16, enemies[i]->position_x, enemies[i]->position_y, 16 * 2, 16 * 2, 0);				
			}else{
				al_draw_scaled_bitmap(sprite_sheet, enemies[i]->sprite.x2, enemies[i]->sprite.y2, 16, 16, enemies[i]->position_x, enemies[i]->position_y, 16 * 2, 16 * 2, 0);
			}
	}
	}
	animation_counter = (animation_counter + 1) % (2 * animation_delay);
	move_extra_enemy(enemies[n_enemies - 1], sprite_sheet);
}

void enemy_shot(enemy *enemy){
	if(!enemy->alive) return;
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
		if(enemies[i]->gun->shots != NULL)
			destroy_bullet_list(enemies[i]->gun->shots);
		free(enemies[i]->gun);
		free(enemies[i]);
	}
	free(enemies);
}

int enemy_has_shot_column(int column, enemy** enemies, int n_enemies){	
	for(int i = 0; i < n_enemies; i++)
		for(bullet* index = enemies[i]->gun->shots; index != NULL; index = (bullet*) index->next)
			if(index->x + 32 > column && index->x - 32 < column) return 1;
		
	return 0;	
}

int enemy_in_front_of_enemy(enemy* current_enemy, enemy** enemies, int n_enemies){
	for(int i = 0; i < n_enemies; i++){
		if(enemies[i]->position_x == current_enemy->position_x && enemies[i]->position_y > current_enemy->position_y)
			return 1;
	}
	return 0;
}

void extra_enemy_event(enemy** enemies, int n_enemies){
	if(enemies[n_enemies - 1]->alive) return;
	srand(time(NULL));
	int random = rand() % 100;
	if(random == 0){
		enemies[n_enemies - 1]->position_x = 640;
		enemies[n_enemies - 1]->position_y = 65;
		enemies[n_enemies - 1]->alive = 1;
	}
}