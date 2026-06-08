/**
 * @file Jogador.h
 * @author @EddiePricefield & @rogerioccastro
 * @brief Declarações das funções do Jogador.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria uma instância alocada dinamicamente da struct Jogador.
 */
Jogador *criarJogador( float x, float y, float w, float h );

/**
 * @brief Destrói um objeto Jogador e libera seus recursos.
 */
void destruirJogador( Jogador *j );

/**
 * @brief Lê a entrada do usuário e atualiza as velocidades do jogador.
 */
void entradaJogador( Jogador *j, float delta );

/**
 * @brief Aplica física e resolve colisões do jogador com o mundo.
 */
void atualizarJogador( Jogador *j, GameWorld *gw, float delta );

/**
 * @brief Desenha o jogador.
 */
void desenharJogador( Jogador *j );