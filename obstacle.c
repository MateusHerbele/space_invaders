#include "obstacle.h"

obstacle* obstacle_create(int x, int y){
    obstacle* new_obstacle = (obstacle*) malloc(sizeof(obstacle));
    if(new_obstacle == 0){
        fprintf(stderr, "ERRO: não foi possível alocar memória para o obstáculo!\n");
        exit(1);
    }
    new_obstacle->lifes = 10;
    new_obstacle->position_x = x;
    new_obstacle->position_y = y;
    return new_obstacle;
}

obstacle** create_obstacles(int n_obstacles, int max_x, int max_y){
    obstacle** obstacles = (obstacle**) malloc(sizeof(obstacle*) * n_obstacles);
    if(obstacles == NULL){
        fprintf(stderr, "ERRO: não foi possível alocar memória para o array de obstáculos!\n");
        exit(1);
    }
    int x = 40;
    int y = 3*(max_y/n_obstacles);
    for(int i = 0; i < n_obstacles; i++){
        // if(i == n_obstacles -1){
        //     x = max_x - 64;
        //     obstacles[i] = obstacle_create(x, y);
        //     break;
        // }
        obstacles[i] = obstacle_create(x, y);
        x += (max_x/n_obstacles);
    }
    return obstacles;
}

void generate_obstacles(obstacle** array, int n_obstacles, ALLEGRO_BITMAP* sprite_sheet){
    for(int i = 0; i < n_obstacles; i++){
        // al_draw_bitmap_region(sprite_sheet, 0, 0, 16, 16, array[i]->position_x, array[i]->position_y, 0);
        switch(array[i]->lifes){
            case 10:
                al_draw_scaled_bitmap(sprite_sheet, 0, 96, 16, 16, array[i]->position_x, array[i]->position_y, 16 * 4, 16 * 2, 0);
            break;
            case 9:
                al_draw_scaled_bitmap(sprite_sheet, 16, 96, 16, 16, array[i]->position_x, array[i]->position_y, 16 * 4, 16 * 2, 0);
            break;
            case 8:
                al_draw_scaled_bitmap(sprite_sheet, 32, 96, 16, 16, array[i]->position_x, array[i]->position_y, 16 * 4, 16 * 2, 0);
            break;
            case 7:
                al_draw_scaled_bitmap(sprite_sheet, 48, 96, 16, 16, array[i]->position_x, array[i]->position_y, 16 * 4, 16 * 2, 0);
            break;
            case 6:
                al_draw_scaled_bitmap(sprite_sheet, 64, 96, 16, 16, array[i]->position_x, array[i]->position_y, 16 * 4, 16 * 2, 0);
            break;
            case 5:
                al_draw_scaled_bitmap(sprite_sheet, 80, 96, 16, 16, array[i]->position_x, array[i]->position_y, 16 * 4, 16 * 2, 0);
            break;
            case 4:
                al_draw_scaled_bitmap(sprite_sheet, 96, 96, 16, 16, array[i]->position_x, array[i]->position_y, 16 * 4, 16 * 2, 0);
            break;
            case 3:
                al_draw_scaled_bitmap(sprite_sheet, 0, 112, 16, 16, array[i]->position_x, array[i]->position_y, 16 * 4, 16 * 2, 0);
            break;
            case 2:
                al_draw_scaled_bitmap(sprite_sheet, 16, 112, 16, 16, array[i]->position_x, array[i]->position_y, 16 * 4, 16 * 2, 0);
            break;
            case 1:
                al_draw_scaled_bitmap(sprite_sheet, 32, 112, 16, 16, array[i]->position_x, array[i]->position_y, 16 * 4, 16 * 2, 0);
            break;
        }
    }
}