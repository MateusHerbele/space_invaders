#include  "menu.h"


void start_game_action(unsigned short* program_event){
    program_event = 1;
}

void instructions_action(){
    
}

void quit_game_action(unsigned short* running){
    *running = 0;
}