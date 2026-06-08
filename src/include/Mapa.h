/**
 * @file Mapa.h
 * @author @EddiePricefield & @rogerioccastro
 * @brief Declarações das funções do Mapa.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Carrega um mapa a partir de uma arquivo.
 */
Mapa *carregarMapa( const char *caminhoArquivo );

/**
 * @brief Destroi um mapa.
 */
void destruirMapa( Mapa *m );

/**
 * @brief Atualiza um mapa.
 */
void atualizarMapa( Mapa *m, GameWorld *gw, float delta );

/**
 * @brief Desenha um mapa.
 */
void desenharMapa( Mapa *m );

/**
 * @brief Calcula a largura do mapa.
 */
int calcularLarguraMapa( Mapa *m );

/**
 * @brief Calcula a altura do mapa.
 */
int calcularAlturaMapa( Mapa *m );