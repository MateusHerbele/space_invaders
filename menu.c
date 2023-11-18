#include  "menu.h"


void start_game_action(unsigned short* program_event){
    *program_event = 1;
}

void instructions_action(){
    
}

void quit_game_action(unsigned short* running){
    *running = 0;
}

void render_menu_option(menu_option* option, ALLEGRO_FONT* font) {
    // Desenha o fundo do option do menu
    if (option->selected) {
        al_draw_filled_rectangle(option->x, option->y, option->x + option->width, option->y + option->height, al_map_rgb(255, 0, 0)); // Cor vermelha para o option selecionado
    } else {
        al_draw_filled_rectangle(option->x, option->y, option->x + option->width, option->y + option->height, al_map_rgb(0, 0, 0)); // Cor de fundo padrão
    }

    // Desenha o texto no centro do option do menu
    int text_width = al_get_text_width(font, option->text);
    int text_height = al_get_font_line_height(font);
    al_draw_text(font, al_map_rgb(255, 255, 255), option->x + (option->width - text_width) / 2, option->y + (option->height - text_height) / 2, 0, option->text);
}

void render_menu(ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_BITMAP* sprite_sheet, menu_option* options[], ALLEGRO_FONT* font, unsigned short* running) {
    static int selected_menu_option = 0; // Índice do item selecionado
    al_wait_for_event(queue, &event); // Aguarda um evento
    // Desenha o fundo do menu
    al_draw_filled_rectangle(0, 0, 640, 800, al_map_rgb(0, 0, 0));
    // Verifica se a tecla para cima foi pressionada
    if (event.keyboard.keycode == ALLEGRO_KEY_UP || event.keyboard.keycode == ALLEGRO_KEY_W) {
        options[selected_menu_option]->selected = false; // Desmarca o item atual
        switch(selected_menu_option){
            case 0:
                selected_menu_option = 1;
            break;
            case 1:
                selected_menu_option = 2;
            break;
            case 2:
                selected_menu_option = 0;
            break;
        }
        options[selected_menu_option]->selected = true; // Marca o novo item
    }

    // Verifica se a tecla para baixo foi pressionada
    if (event.keyboard.keycode == ALLEGRO_KEY_DOWN || event.keyboard.keycode == ALLEGRO_KEY_S) {
        options[selected_menu_option]->selected = false; // Desmarca o item atual
        switch(selected_menu_option){
            case 0:
                selected_menu_option = 2;
            break;
            case 1:
                selected_menu_option = 0;
            break;
            case 2:
                selected_menu_option = 1;
            break;
        }
        options[selected_menu_option]->selected = true; // Marca o novo item
    }

    // Verifica se a tecla Enter foi pressionada
    if (event.keyboard.keycode == ALLEGRO_KEY_ENTER || event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
        options[selected_menu_option]->action(); // Executa a ação associada ao item selecionado
    }

    // Fechar o jogo com ESC, Q e ao clicar no X
    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE || event.keyboard.keycode == ALLEGRO_KEY_Q || event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        *running = 0;
    }

    while(!al_is_event_queue_empty(queue)) {
        al_get_next_event(queue, &event);
        for (int i = 0; i < 3; i++) 
        render_menu_option(options[i], font);
    
    // Desenha o logo do menu
        al_draw_scaled_bitmap(sprite_sheet, 48, 0, 16, 16, 640/2 - 135, 10, 16 * 16, 16 * 16, 0);
        // Desenha o texto de orientação do menu
        int text_width = al_get_text_width(font, "PRESS 'ENTER' TO SELECT");
        int text_height = al_get_font_line_height(font);
        al_draw_text(font, al_map_rgb(255, 255, 255), 220 + (200 - text_width) / 2, 700 + (50 - text_height) / 2, 0, "PRESS 'ENTER' TO SELECT");

        al_flip_display();
    }
    // Desenha as opções do menu
    
}