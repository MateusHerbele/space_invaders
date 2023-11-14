#include "hud.h"


hud* create_hud(){
    hud* new_hud = (hud*) malloc(sizeof(hud));
    if(new_hud == NULL){
        fprintf(stderr, "ERRO: não foi possível alocar memória para o hud!\n");
        exit(1);
    }
    new_hud->position_x_lifes = 0;
    new_hud->position_y_lifes = 0;
    new_hud->position_x_score = 0;
    new_hud->position_y_score = 0;
    new_hud->width_lifes = 0;
    new_hud->height_lifes = 0;
    return new_hud;
}

void generate_hud(hud* hud, unsigned short player_lifes, ALLEGRO_BITMAP* sprite_sheet){
   unsigned short lost_lifes = 3 - player_lifes;
   int i = 0;
   
   for(i = 0; i < player_lifes; i++){
        al_draw_scaled_bitmap(sprite_sheet, 80, 48, 16, 16, hud->position_x_lifes + (40*i), hud->position_y_lifes, 16 * 2, 16 * 2, 0);
        printf("que haja coracoes\n");    
   }
   printf("lost lifes: %d\n", lost_lifes);
   for(int j = 1; j < lost_lifes; j++)
        al_draw_scaled_bitmap(sprite_sheet, 96, 48, 16, 16, hud->position_x_lifes + (40*(i+j)), hud->position_y_lifes, 16 * 2, 16 * 2, 0);
}