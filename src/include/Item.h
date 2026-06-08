/**
 * @file Item.h
 * @author @EddiePricefield & @rogerioccastro
 * @brief Declarações das funções do Item.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo item.
 */
Item *criarItem( TipoItem tipo );

/**
 * @brief Destroi um item.
 */
void destruirItem( Item *item );

/**
 * @brief Atualiza um item.
 */
void atualizarItem( Item *item, float delta );

/**
 * @brief Desenha um item.
 */
void desenharItem( Item *item );