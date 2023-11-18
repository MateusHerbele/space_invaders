#include "game.h"

int main(int argc, char** argv){
	int n_enemies = 66;
	int n_obstacles = 4;
	int game_event = 0; // eh pra começar em 0, ta em 1 pra testes
	int round = 0;

	player* player = create_player(X_SCREEN/2, Y_SCREEN - 16);
	enemy** enemies =  create_enemies(n_enemies, 6, 11);
	obstacle** obstacles = create_obstacles(n_obstacles, X_SCREEN, Y_SCREEN);
	hud* hud = create_hud();
	// Funcoes allegro
	al_init();																																																//Faz a preparação de requisitos da biblioteca Allegro
	al_init_primitives_addon();																																												//Faz a inicialização dos addons das imagens básicas
	
	al_install_keyboard();																																													//Habilita a entrada via teclado (eventos de teclado), no programa
	al_init_font_addon();
	al_init_ttf_addon();


	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);																																						//Cria o relógio do jogo; isso indica quantas atualizações serão realizadas por segundo (30, neste caso)
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();																																					//Cria a fila de eventos; todos os eventos (programação orientada a eventos) 
	// ALLEGRO_FONT* font = al_create_builtin_font();		
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);			

	al_register_event_source(queue, al_get_keyboard_event_source());																																		//Indica que eventos de teclado serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_display_event_source(disp));																																		//Indica que eventos de tela serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_timer_event_source(timer));			
	
	// Obtenha as dimensões da tela
    int screen_width = al_get_display_width(al_get_current_display());
    int screen_height = al_get_display_height(al_get_current_display());

    // Calcule a posição inicial para centralizar a janela
    int window_x = (screen_width)/2 + X_SCREEN/4;
    int window_y = (screen_height)/2 - Y_SCREEN;	

	al_set_window_position(disp, window_x, window_y);																											//Indica que eventos de relógio serão inseridos na nossa fila de eventos

	ALLEGRO_EVENT event;																																												//Cria uma variável para armazenar o evento atual
	al_start_timer(timer);																																//Cria uma janela para o programa, define a largura (x) e a altura (y) da tela em píxeis (320x320, neste caso)

	// Inicia imagens
	al_init_image_addon();

	// Inicia Primtivos
	al_init_primitives_addon();

	// Sprites do jogo
	
	ALLEGRO_BITMAP* sprite_sheet = al_load_bitmap("sprites/spritesheet.png");
	if(!sprite_sheet){
		fprintf(stderr, "Falha ao carregar spritesheet!\n");
		exit(1);
	}
	ALLEGRO_BITMAP* icon = al_load_bitmap("sprites/icon.png");
	if(!icon){
		fprintf(stderr, "Falha ao carregar o ícone do jogo!\n");
		exit(1);
	}
	al_set_display_icon(disp, icon);
	ALLEGRO_FONT* font = al_load_font("fonts/Arcade.ttf", 32, 0);
	if(!font){
		fprintf(stderr, "Falha ao carregar fonte!\n");
		exit(1);
	}

	generating_game(game_event, round, player, enemies, n_enemies, obstacles, n_obstacles, hud, font, sprite_sheet, timer, queue, disp, event);
	// Allegro
	al_destroy_font(font);																																													//Destrutor da fonte padrão
	al_destroy_display(disp);																																												//Destrutor da tela
	al_destroy_timer(timer);																																												//Destrutor do relógio
	al_destroy_event_queue(queue);	

	return 0;
}
