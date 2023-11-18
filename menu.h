#ifndef __MENU__
#define __MENU__

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

typedef struct{
    int x, y; // Posição da posição
    int width, height; // Largura e altura do menu
    char* text; // Texto do botão
    bool selected; // Indica se o botão está selecionado
    void (*action)(); // Função a ser executada ao clicar no botão
} menu_option;

void start_game_action(unsigned short* program_event);
void instructions_action();
void quit_game_action(unsigned short* running);
void render_menu_option(menu_option* option, ALLEGRO_FONT* font);
void render_menu(ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_BITMAP* sprite_sheet, menu_option** options, ALLEGRO_FONT* font, unsigned short* running);

#endif // __MENU__