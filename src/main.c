/**
 * @file main.c
 * @author @EddiePricefield & @rogerioccastro
 * @brief Função principal e lógica do jogo. Template base para desenvolvimento
 * de jogos em C usando Raylib (https://www.raylib.com/).
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "GameWindow.h"

int main( void ) {

    GameWindow *gameWindow = createGameWindow(
        800,             // width
        450,             // height
        "WindowSurfer", // title
        60,              // target FPS
        false,           // antialiasing
        false,           // resizable
        false,           // full screen
        false,           // undecorated
        false,           // always on top
        false,           // invisible background
        false,           // always run
        true,            // load resources
        true             // init audio
    );

    initGameWindow( gameWindow );

    return 0;

}