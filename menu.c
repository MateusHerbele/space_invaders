#include  "menu.h"


void start_game_action(unsigned short* program_event){
    *program_event = 1;
}

void instructions_action(ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_BITMAP* sprite_sheet, unsigned short* running) {
    int text_width = 0, text_height = 0;

    al_clear_to_color(al_map_rgb(0, 0, 30));

    while (*running){
        al_wait_for_event(queue, &event); // Aguarda um evento
        // Fechar o jogo com ESC, Q e ao clicar no X
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE || event.keyboard.keycode == ALLEGRO_KEY_Q || event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                *running = 0;
                break;
            }
        if (event.type == ALLEGRO_EVENT_TIMER) {
            // Representa as instruções
            text_width = al_get_text_width(font, "-DEFEAT ALL ENEMIES");
            text_height = al_get_font_line_height(font);
            al_draw_text(font, al_map_rgb(255, 0, 0), 220 + (200 - text_width) / 2, 100 + (50 - text_height) / 2, 0, "-DEFEAT ALL ENEMIES");
            text_width = al_get_text_width(font, "TO MOVE TO THE NEXT ROUND-");
            al_draw_text(font, al_map_rgb(255, 0, 0), 220 + (200 - text_width) / 2, 120 + (50 - text_height) / 2, 0, "TO MOVE TO THE NEXT ROUND-");
            text_width = al_get_text_width(font, "-'A' and 'D' TO MOVE LEFT AND RIGHT-");
            al_draw_text(font, al_map_rgb(255, 189, 92), 220 + (200 - text_width) / 2, 140 + (50 - text_height) / 2, 0, "-'A' and 'D' TO MOVE LEFT AND RIGHT-");
            text_width = al_get_text_width(font, "-ARROWS CAN BE USED AS WELL-");
            al_draw_text(font, al_map_rgb(255, 189, 92), 220 + (200 - text_width) / 2, 160 + (50 - text_height) / 2, 0, "-ARROWS CAN BE USED AS WELL-");
            text_width = al_get_text_width(font, "-PRESS SPACE TO SHOT-");
            al_draw_text(font, al_map_rgb(255, 189, 92), 220 + (200 - text_width) / 2, 180 + (50 - text_height) / 2, 0, "-PRESS SPACE TO SHOT-");
            // Representa os inimigos e seus pontos
            text_width = al_get_text_width(font, " = 10 POINTS");
            // Inimigo fácil
            al_draw_text(font, al_map_rgb(230, 255, 255), 220 + (200 - text_width) / 2, 240 + (50 - text_height) / 2, 0, " = 10 POINTS");
            al_draw_scaled_bitmap(sprite_sheet, 80, 16, 16, 16, 185 + (200 - text_width) / 2, 240, 16 * 3, 16 * 3, 0);
            // Inimigo médio
            al_draw_text(font, al_map_rgb(0, 155, 111), 220 + (200 - text_width) / 2, 300 + (50 - text_height) / 2, 0, " = 20 POINTS");
            al_draw_scaled_bitmap(sprite_sheet, 96, 32, 16, 16, 185 + (200 - text_width) / 2, 300, 16 * 3, 16 * 3, 0);
            // Inimigo difícil
            al_draw_text(font, al_map_rgb(255, 0, 67), 220 + (200 - text_width) / 2, 360 + (50 - text_height) / 2, 0, " = 40 POINTS");
            al_draw_scaled_bitmap(sprite_sheet, 32, 32, 16, 16, 185 + (200 - text_width) / 2, 360, 16 * 3, 16 * 3, 0);
            // Inimigo especial
            al_draw_text(font, al_map_rgb(143, 0, 211), 220 + (200 - text_width) / 2, 420 + (50 - text_height) / 2, 0, " = ?? POINTS");
            al_draw_scaled_bitmap(sprite_sheet, 48, 48, 16, 16, 185 + (200 - text_width) / 2, 420, 16 * 3, 16 * 3, 0);
            // Representa o retorno pro menu
            text_width = al_get_text_width(font, "PRESS 'ENTER' TO RETURN TO MENU");
            al_draw_text(font, al_map_rgb(255, 255, 255), 220 + (200 - text_width) / 2, 700 + (50 - text_height) / 2, 0, "PRESS 'ENTER' TO RETURN TO MENU");
            al_flip_display();
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) 
            // Verifica se a tecla Enter foi pressionada
            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER){
                al_clear_to_color(al_map_rgb(0, 0, 0)); // Necessário para apagar o texto das instruções
                break;
            } 
    }
}


void quit_game_action(unsigned short* running){
    *running = 0;
}

void render_menu_option(menu_option* option, ALLEGRO_FONT* font) {
    // Desenha o fundo do option do menu
    if (option->selected) {
        al_draw_filled_rectangle(option->x - 2, option->y, option->x + option->width, option->y + option->height, al_map_rgb(255, 0, 0)); // Cor vermelha para o option selecionado
    } else {
        al_draw_filled_rectangle(option->x - 2, option->y, option->x + option->width, option->y + option->height, al_map_rgb(0, 0, 30)); // Cor de fundo padrão
    }

    // Desenha o texto no centro do option do menu
    int text_width = al_get_text_width(font, option->text);
    int text_height = al_get_font_line_height(font);
    al_draw_text(font, al_map_rgb(255, 255, 255), option->x + (option->width - text_width) / 2, option->y + (option->height - text_height) / 2, 0, option->text);
}

void render_menu(ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_BITMAP* sprite_sheet, menu_option* options[], ALLEGRO_FONT* font, unsigned short* running, unsigned short* program_event) {
    static int selected_menu_option = 0; // Índice do item selecionado
    int text_width = 0, text_height = 0;

    
    al_wait_for_event(queue, &event); // Aguarda um evento
    al_clear_to_color(al_map_rgb(0, 0, 30)); // Limpa a tela com a cor azul escuro
    // Fechar o jogo com ESC, Q e ao clicar no X
    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE || event.keyboard.keycode == ALLEGRO_KEY_Q || event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        *running = 0;
    
    if(event.type == ALLEGRO_EVENT_TIMER){
        // Desenha as opções do menu
        for (int i = 0; i < 3; i++) 
        render_menu_option(options[i], font);
    
        // Desenha o logo do menu
        al_draw_scaled_bitmap(sprite_sheet, 48, 0, 16, 16, 640/2 - 135, 10, 16 * 16, 16 * 16, 0);
        // Desenha o texto de orientação do menu
        text_width = al_get_text_width(font, "PRESS 'ENTER' TO SELECT");
        text_height = al_get_font_line_height(font);
        al_draw_text(font, al_map_rgb(255, 255, 255), 220 + (200 - text_width) / 2, 700 + (50 - text_height) / 2, 0, "PRESS 'ENTER' TO SELECT");

        al_flip_display();

    }else{
        // Verifica se a tecla para cima foi pressionada
        if (event.type == ALLEGRO_EVENT_KEY_DOWN){
            if (event.keyboard.keycode == ALLEGRO_KEY_UP || event.keyboard.keycode == ALLEGRO_KEY_W) {
                options[selected_menu_option]->selected = false; // Desmarca o item atual
                selected_menu_option = (selected_menu_option - 1 + 3) % 3;
                options[selected_menu_option]->selected = true; // Marca o novo item
            }

            // Verifica se a tecla para baixo foi pressionada
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN || event.keyboard.keycode == ALLEGRO_KEY_S) {
                options[selected_menu_option]->selected = false; // Desmarca o item atual
                selected_menu_option = (selected_menu_option + 1) % 3;
                options[selected_menu_option]->selected = true; // Marca o novo item
            }

            // Verifica se a tecla Enter foi pressionada
            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                switch(selected_menu_option){
                    case 0:
                        options[selected_menu_option]->action(program_event); // Executa a ação associada ao item selecionado
                    break;
                    case 1:
                        options[selected_menu_option]->action(event, queue, font, sprite_sheet, running); // Executa a ação associada ao item selecionado
                    break;
                    case 2:
                        options[selected_menu_option]->action(running); // Executa a ação associada ao item selecionado
                    break;
                }
            }
        }
    }
}