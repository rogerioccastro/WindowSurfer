/**
 * @file Obstaculo.h
 * @author @EddiePricefield & @rogerioccastro
 * @brief Declarações das funções do Obstáculo.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "raylib/raylib.h"

#include "Tipos.h"

/**
 * @brief Cria um novo obstáculo.
 */
Obstaculo *criarObstaculo( Rectangle ret, Color cor, Rectangle fonte, Texture2D *textura );

/**
 * @brief Destroi um obstáculo.
 */
void destruirObstaculo( Obstaculo *o );

/**
 * @brief Desenha um obstáculo.
 */
void desenharObstaculo( Obstaculo *o );