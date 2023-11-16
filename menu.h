#ifndef __MENU__
#define __MENU__

typedef struct{
    int x, y; // Posição da posição
    int width, height; // Largura e altura do menu
    char texto[100]; // Texto do botão
    void (*action)(); // Função a ser executada ao clicar no botão
} menu_option;

void start_game_action(unsigned short* program_event);
void instructions_action();
void quit_game_action();

#endif // __MENU__