# space_invaders
Trabalho final da disciplina de Prog 2

Extras:
    Movimentação dos Inimigos
    Colunas Vazias
    Inimigo extra
    Colisão entre tiros

Notas:
    Lógica de funcionamentos do tiro dos inmigos
        Originalmente, o planejamento feito era para um inimigo de cada tipo deveria atirar a cada atraso de tmepo, nesta lógica foi aplicada a verificação para o tipo de inimigo fácil e médio se havia tiros inimigos na coluna em que iriam atirar, ainda sem a verificação se hava inimigo na frente ou não.
        A função em questão foi implementada assim:
```c
        void update_enemies_shots(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int player_x, int player_y, unsigned short round){
            static int shot_delay_0 = 200; // Valor pra não começar o round com tiros
            static int shot_delay_1 = 200; // Valor pra não começar o round com tiros	
            static int shot_delay_2 = 200; // Valor pra não começar o round com tiros
            int actual_distance = 0;
            int closest_distance_0 = 1000; // valor arbitrário para iniciar a comparação
            int closest_distance_1 = 1000; // valor arbitrário para iniciar a comparação
            int closest_distance_2 = 1000; // valor arbitrário para iniciar a comparação
            enemy* closest_enemy_0 = enemies[0];
            enemy* closest_enemy_1 = enemies[0];
            enemy* closest_enemy_2 = enemies[0];

            for(int i = 0; i < n_enemies; i++){
                if(enemies[i]->alive){
                    switch(enemies[i]->type){
                        case 0:
                            actual_distance = two_points_distance(enemies[i]->position_x, player_x, enemies[i]->position_y, player_y);
                            if(actual_distance < closest_distance_0){
                                closest_distance_0 = actual_distance;
                                closest_enemy_0 = enemies[i];
                            }
                        break;
                        case 1:
                            actual_distance = two_points_distance(enemies[i]->position_x, player_x, enemies[i]->position_y, player_y);
                            if(actual_distance < closest_distance_1){
                                closest_distance_1 = actual_distance;
                                closest_enemy_1 = enemies[i];
                            }
                        break;
                        case 2:
                            actual_distance = two_points_distance(enemies[i]->position_x, player_x, enemies[i]->position_y, player_y);
                            if(actual_distance < closest_distance_2){
                                closest_distance_2 = actual_distance;
                                closest_enemy_2 = enemies[i];
                            }
                        break;
                    }
                }
            }

            // Lógica de tiro Mateus Herbele
            if((shot_delay_0 <= 0)&& !enemy_has_shot_column(closest_enemy_0->gun->shots, closest_enemy_0->position_x, enemies, n_enemies)){
                enemy_shot(closest_enemy_0);
                shot_delay_0 = ENEMY_SHOT_COOLDOWN + 20 - (round/2);
            }else shot_delay_0--;

            if((shot_delay_1 <= 0) && !enemy_has_shot_column(closest_enemy_1->gun->shots, closest_enemy_1->position_x, enemies, n_enemies)){
                enemy_shot(closest_enemy_1);
                shot_delay_1 = ENEMY_SHOT_COOLDOWN + 10 - (round/2);
            }else
                shot_delay_1--;
            if(!shot_delay_2){
                enemy_shot(closest_enemy_2);
                shot_delay_2 = ENEMY_SHOT_COOLDOWN -  (round/2);
            }else shot_delay_2--;
        }
```
        Porém seguindo o que foi pedido no trabalho alterei para que a cada atraso de tempo dois dos inimigos mais próximos a nave atirassem:

```c
        void update_enemies_shots(enemy** enemies, int n_enemies, ALLEGRO_BITMAP* sprite_sheet, int player_x, int player_y, unsigned short round){ // game.c
            static int shot_delay_0 = 200; // Valor pra não começar o round com tiros
            static int shot_delay_1 = 200; // Valor pra não começar o round com tiros	
            int actual_distance = 0;
            int closest_distance_0 = 1000; // valor arbitrário para iniciar a comparação
            enemy* closest_enemy_0 = NULL;
            enemy* closest_enemy_1 = NULL;

            for(int i = 0; i < n_enemies; i++){
                if(enemies[i]->alive)
                    switch(enemies[i]->type){
                        case 0:
                            if(enemy_has_shot_column(enemies[i]->position_x, enemies, n_enemies) ||  enemy_in_front_of_enemy(enemies[i], enemies, n_enemies))
                                continue;
                            else{
                                actual_distance = two_points_distance(enemies[i]->position_x, player_x, enemies[i]->position_y, player_y);
                                if(actual_distance < closest_distance_0){
                                    closest_enemy_1 = closest_enemy_0;
                                    closest_distance_0 = actual_distance;
                                    closest_enemy_0 = enemies[i];
                                }
                            }
                        break;
                        case 1:
                            if(enemy_has_shot_column(enemies[i]->position_x, enemies, n_enemies))
                                continue;
                            else{
                                actual_distance = two_points_distance(enemies[i]->position_x, player_x, enemies[i]->position_y, player_y);
                                if(actual_distance < closest_distance_0){
                                    closest_enemy_1 = closest_enemy_0;
                                    closest_distance_0 = actual_distance;
                                    closest_enemy_0 = enemies[i];
                                }
                            }
                        break;
                        case 2:
                            actual_distance = two_points_distance(enemies[i]->position_x, player_x, enemies[i]->position_y, player_y);
                            if(actual_distance < closest_distance_0){
                                closest_enemy_1 = closest_enemy_0;
                                closest_distance_0 = actual_distance;
                                closest_enemy_0 = enemies[i];
                            }
                        break;
                    }
            }
            if(shot_delay_0 <= 0){
                enemy_shot(closest_enemy_0);
                shot_delay_0 = ENEMY_SHOT_COOLDOWN + 20 - (round/2);
            }else shot_delay_0--;

            if(shot_delay_1 <= 0){
                enemy_shot(closest_enemy_1);
                shot_delay_1 = ENEMY_SHOT_COOLDOWN + 10 - (round/2);
            }else
                shot_delay_1--;
        }
```
        Nessa versão, é verificado os inimigos que estão mais próximos da nave, que segundo as exigências de seus tipos, devem atirar ou não, optei por manter os atrasos a mais, pois eles causam uma baixa diferença em relação ao tempo que são atirados, porém é suficiente para que fique visualmente mais agradável e desafiador para o jogador.

    Inimigo extra:
        Eu optei por não deixar ele vermelho, pois esteticamente pra mim é mais agradável, porém na função que gera a imagem dele em jogo está comentado o código em que da pra alterar ele pra ficar avermelhado, é só inverter o que está comentado com o que está sendo executado.
    