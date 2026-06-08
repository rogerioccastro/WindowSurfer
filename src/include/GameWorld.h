/**
 * @file GameWorld.h
 * @author @EddiePricefield & @rogerioccastro
 * @brief Declarações de funções do GameWorld.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria uma instância alocada dinamicamente da struct GameWorld.
 */
GameWorld *createGameWorld( void );

/**
 * @brief Destrói um objeto GameWorld e suas dependências.
 */
void destroyGameWorld( GameWorld *gw );

/**
 * @brief Lê a entrada do usuário e atualiza o estado do jogo.
 */
void updateGameWorld( GameWorld *gw, float delta );

/**
 * @brief Desenha o estado do jogo.
 */
void drawGameWorld( GameWorld *gw );

/**
 * @brief Alterar o estado do jogo.
 */
void alterarEstadoJogo( EstadoJogo novoEstado );
