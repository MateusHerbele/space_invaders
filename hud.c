#include "hud.h"


hud* create_hud(){
    hud* new_hud = (hud*) malloc(sizeof(hud));
    if(new_hud == NULL){
        fprintf(stderr, "ERRO: não foi possível alocar memória para o hud!\n");
        exit(1);
    }
    new_hud->position_x_lives = 0;
    new_hud->position_y_lives = 0;
    new_hud->position_x_score = 0;
    new_hud->position_y_score = 0;
    new_hud->width_lives = 0;
    new_hud->height_lives = 0;
    return new_hud;
}

void generate_hud(hud* hud, player* player, unsigned short round, ALLEGRO_BITMAP* sprite_sheet, ALLEGRO_FONT* font){
   unsigned short lost_lives = player->max_lives - player->lives;
   int i = 0;
   // full hearts
   for(i = 0; i < player->lives; i++)
        al_draw_scaled_bitmap(sprite_sheet, 80, 48, 16, 16, hud->position_x_lives + (40*i), hud->position_y_lives, 16 * 2, 16 * 2, 0);
   // empty hearts
   for(int j = 1; j <= lost_lives; j++)
        switch(lost_lives){
            case 1:
                al_draw_scaled_bitmap(sprite_sheet, 96, 48, 16, 16, hud->position_x_lives + (40*2), hud->position_y_lives, 16 * 2, 16 * 2, 0);
            break;
            case 2:
                al_draw_scaled_bitmap(sprite_sheet, 96, 48, 16, 16, hud->position_x_lives + (40*2), hud->position_y_lives, 16 * 2, 16 * 2, 0);
                al_draw_scaled_bitmap(sprite_sheet, 96, 48, 16, 16, hud->position_x_lives + (40*1), hud->position_y_lives, 16 * 2, 16 * 2, 0);
            break;
            case 3:
                al_draw_scaled_bitmap(sprite_sheet, 96, 48, 16, 16, hud->position_x_lives + (40*2), hud->position_y_lives, 16 * 2, 16 * 2, 0);
                al_draw_scaled_bitmap(sprite_sheet, 96, 48, 16, 16, hud->position_x_lives + (40*1), hud->position_y_lives, 16 * 2, 16 * 2, 0);
                al_draw_scaled_bitmap(sprite_sheet, 96, 48, 16, 16, hud->position_x_lives + (40*0), hud->position_y_lives, 16 * 2, 16 * 2, 0);
            break;
        }
    // score
    al_draw_text(font, al_map_rgb(255, 255, 255), 640 - 100, 0, 0, "SCORE:");	
    al_draw_textf(font, al_map_rgb(255, 255, 255), 640 - 100, 20, 0, "%d", player->score);
    // round
    al_draw_text(font, al_map_rgb(255, 255, 255), 640/2 - 100, 0, 0, "ROUND:");
    al_draw_textf(font, al_map_rgb(255, 255, 255), 640/2 - 100, 20, 0, "%u", round/8);

    // linha de divisão
    al_draw_line(0, 70, 640, 70, al_map_rgb(255, 255, 255), 5);
}