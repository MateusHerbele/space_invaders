#include "game.h"

int main(int argc, char** argv){
	int n_enemies = 67; // 22 Fracos + 22 Médios + 22 Fortes + 1 Extra
	int n_obstacles = 4; // 4 obstáculos
	unsigned short program_event = 0; // Começa em 0 para ir para o menu
	unsigned short round = 0; // Contador de rounds

	player* player = create_player(X_SCREEN/2, Y_SCREEN - 64); // Cria o jogador
	enemy** enemies =  create_enemies(n_enemies, 6, 11); // Cria os inimigos
	obstacle** obstacles = create_obstacles(n_obstacles, X_SCREEN, Y_SCREEN); // Cria os obstáculos
	hud* hud = create_hud(); // Cria a hud

	// Allegro
	al_init(); // Inicia a preparação para os recursos da Allegro
	al_init_primitives_addon();	 // Faz a inicialização dos addons das imagens básicas
	al_init_image_addon(); // Inicia imagens
	al_init_primitives_addon(); // Inicia Primtivos
	
	al_install_keyboard();	// Habilita a entrada via teclado (eventos de teclado), no programa
	al_init_font_addon(); // Inicia a inicialização dos addons das fontes
	al_init_ttf_addon(); // Inicia a inicialização dos addons das fontes, nesse caso para mudar ela


	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);  // Cria o relógio do jogo; isso indica quantas atualizações serão realizadas por segundo (60, neste caso)
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue(); // Cria a fila de eventos; todos os eventos (programação orientada a eventos) 
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN); // Cria o display do programa, com as dimensões definidas		

	al_register_event_source(queue, al_get_keyboard_event_source()); // Indica que eventos de teclado serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_display_event_source(disp));	// Indica que eventos de tela serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_timer_event_source(timer));	// Indica que eventos de serão inseridos na nossa fila de eventos	
	
	// Obtenha as dimensões da tela
    int screen_width = al_get_display_width(al_get_current_display()); 
    int screen_height = al_get_display_height(al_get_current_display());

    // Calcule a posição inicial para centralizar a janela
    int window_x = (screen_width)/2 + X_SCREEN/4;
    int window_y = (screen_height)/2 - Y_SCREEN;	

	al_set_window_position(disp, window_x, window_y); // Define a posição da janela do programa na tela

	ALLEGRO_EVENT event; // Cria uma variável para armazenar o evento atual																																			
	al_start_timer(timer); // Inicia o relógio do jogo

	// Sprites do jogo	
	ALLEGRO_BITMAP* sprite_sheet = al_load_bitmap("sprites/spritesheet.png"); // Carrega a imagem do spritesheet
	if(!sprite_sheet){
		fprintf(stderr, "Falha ao carregar spritesheet!\n");
		exit(1);
	}
	ALLEGRO_BITMAP* icon = al_load_bitmap("sprites/icon.png"); // Carrega a imagem do ícone
	if(!icon){
		fprintf(stderr, "Falha ao carregar o ícone do jogo!\n");
		exit(1);
	}
	al_set_display_icon(disp, icon);
	ALLEGRO_FONT* font = al_load_font("fonts/Arcade.ttf", 32, 0); // Carrega a fonte utilizada no jogo
	if(!font){
		fprintf(stderr, "Falha ao carregar fonte!\n");
		exit(1);
	}
	// Pincipal função, controla os eventos do programa e as entidades do jogo, fora a geração de imagens no display
	generating_game(program_event, round, player, enemies, n_enemies, obstacles, n_obstacles, hud, font, sprite_sheet, timer, queue, disp, event);

	// Allegro limpeza
	al_destroy_bitmap(sprite_sheet); // Libera o espaço de memória alocado para o spritesheet
	al_destroy_bitmap(icon); // Libera o espaço de memória alocado para o ícone
	al_destroy_font(font);	// Libera o espaço de memória alocado para a fonte
	al_destroy_display(disp); // Libera o espaço de memória alocado para o display	
	al_destroy_timer(timer); // Libera o espaço de memória alocado para o timer
	al_destroy_event_queue(queue); // Libera o espaço de memória alocado para a fila de eventos

	return 0;
}
