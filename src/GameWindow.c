/**
 * @file GameWindow.c
 * @author @EddiePricefield & @rogerioccastro
 * @brief GameWindow implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdlib.h>
#include <stdbool.h>

#include "GameWindow.h"
#include "GameWorld.h"
#include "ResourceManager.h"
#include "raylib/raylib.h"

/**
 * @brief Creates a dinamically allocated GameWindow struct instance.
 */
GameWindow *createGameWindow( 
        int width, 
        int height, 
        const char *title, 
        int targetFPS,
        bool antialiasing, 
        bool resizable, 
        bool fullScreen,
        bool undecorated, 
        bool alwaysOnTop, 
        bool invisibleBackground, 
        bool alwaysRun, 
        bool loadResources, 
        bool initAudio ) {

    GameWindow *gameWindow = (GameWindow*) malloc( sizeof( GameWindow ) );

    gameWindow->width = width;
    gameWindow->height = height;
    gameWindow->title = title;
    gameWindow->targetFPS = targetFPS;
    gameWindow->antialiasing = antialiasing;
    gameWindow->resizable = resizable;
    gameWindow->fullScreen = fullScreen;
    gameWindow->undecorated = undecorated;
    gameWindow->alwaysOnTop = alwaysOnTop;
    gameWindow->invisibleBackground = invisibleBackground;
    gameWindow->alwaysRun = alwaysRun;
    gameWindow->loadResources = loadResources;
    gameWindow->initAudio = initAudio;
    gameWindow->gw = NULL;
    gameWindow->initialized = false;

    return gameWindow;

}

/**
 * @brief Initializes the Window, starts the game loop and, when it
 * finishes, the window will be finished and destroyed too.
 */
void initGameWindow( GameWindow *gameWindow ) {

    if ( !gameWindow->initialized ) {

        gameWindow->initialized = true;

        if ( gameWindow->antialiasing ) {
            SetConfigFlags( FLAG_MSAA_4X_HINT );
        }

        if ( gameWindow->resizable ) {
            SetConfigFlags( FLAG_WINDOW_RESIZABLE );
        }

        if ( gameWindow->fullScreen ) {
            SetConfigFlags( FLAG_FULLSCREEN_MODE );
        }

        if ( gameWindow->undecorated ) {
            SetConfigFlags( FLAG_WINDOW_UNDECORATED );
        }

        if ( gameWindow->alwaysOnTop ) {
            SetConfigFlags( FLAG_WINDOW_TOPMOST );
        }

        if ( gameWindow->invisibleBackground ) {
            SetConfigFlags( FLAG_WINDOW_TRANSPARENT );
        }

        if ( gameWindow->alwaysRun ) {
            SetConfigFlags( FLAG_WINDOW_ALWAYS_RUN );
        }

        InitWindow( gameWindow->width, gameWindow->height, gameWindow->title );

        if ( gameWindow->initAudio ) {
            InitAudioDevice();
        }

        SetTargetFPS( gameWindow->targetFPS );    

        if ( gameWindow->loadResources ) {
            loadResourcesResourceManager();
        }

        gameWindow->gw = createGameWorld();

        // game loop
        while ( !WindowShouldClose() ) {

            // O delta time é limitado a 1/30s para evitar que frames muito
            // longos (ex.: lentidão na inicialização) causem deslocamentos
            // grandes demais, fazendo personagens atravessarem obstáculos
            // (tunneling).
            float delta = GetFrameTime();
            if ( delta > 1.0f / 30.0f ) {
                delta = 1.0f / 30.0f;
            }

            updateGameWorld( gameWindow->gw, delta );
            drawGameWorld( gameWindow->gw );

            
        }

        if ( gameWindow->loadResources ) {
            unloadResourcesResourceManager();
        }

        bool initAudio = gameWindow->initAudio;

        destroyGameWindow( gameWindow );

        if ( initAudio ) {
            CloseAudioDevice();
        }

        CloseWindow();

    }

}

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWindow( GameWindow *gameWindow ) {
    if ( gameWindow != NULL ) {
        destroyGameWorld( gameWindow->gw );
        free( gameWindow );
    }
}