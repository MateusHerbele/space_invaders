#include "hud.h"

// Cria a hud
hud* create_hud(){
    hud* new_hud = (hud*) malloc(sizeof(hud)); // Aloca memória para a hud
    if(new_hud == NULL){
        fprintf(stderr, "ERRO: não foi possível alocar memória para o hud!\n");
        exit(1);
    }
    // Inicializa os atributos da hud
    new_hud->position_x_lives = 0;
    new_hud->position_y_lives = 0;
    new_hud->position_x_score = 0;
    new_hud->position_y_score = 0;
    new_hud->width_lives = 0;
    new_hud->height_lives = 0;
    return new_hud;
}
// Gera a hud na tela
void generate_hud(hud* hud, player* player, unsigned short round, ALLEGRO_BITMAP* sprite_sheet, ALLEGRO_FONT* font){
   // Corações cheios
   for(int i = 0; i < player->lives; i++) // Desenha os corações cheios
        al_draw_scaled_bitmap(sprite_sheet, 80, 48, 16, 16, hud->position_x_lives + (40*i), hud->position_y_lives, 16 * 2, 16 * 2, 0);
   // Corações vazios
   for(int j = player->lives; j <  player->max_lives; j++) // Desenha os corações vazios
        al_draw_scaled_bitmap(sprite_sheet, 96, 48, 16, 16, hud->position_x_lives + (40*j), hud->position_y_lives, 16 * 2, 16 * 2, 0);

    // Desenha o score
    al_draw_text(font, al_map_rgb(255, 255, 255), 640 - 100, 0, 0, "SCORE:");	
    al_draw_textf(font, al_map_rgb(255, 255, 255), 640 - 100, 20, 0, "%d", player->score);
    // Desenha o round que o player está
    al_draw_text(font, al_map_rgb(255, 255, 255), 640/2 - 100, 0, 0, "ROUND:");
    al_draw_textf(font, al_map_rgb(255, 255, 255), 640/2 - 100, 20, 0, "%u", round/8 + 1);

    // Desenha a linha de divisão da hud
    al_draw_line(0, 70, 640, 70, al_map_rgb(255, 255, 255), 5);
}