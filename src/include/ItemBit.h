/**
 * @file ItemBit.h
 * @author @EddiePricefield & @rogerioccastro
 * @brief Declarações das funções do Item (Bit).
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo item (bit).
 */
ItemBit *criarItemBit( Rectangle ret, Color cor );

/**
 * @brief Destroi um item (bit).
 */
void destruirItemBit( ItemBit *item );

/**
 * @brief Atualiza um item (bit).
 */
void atualizarItemBit( ItemBit *item, float delta );

/**
 * @brief Desenha um item (bit).
 */
void desenharItemBit( ItemBit *item );

/**
 * @brief Obtém o quadro de animação atual de um item (bit).
 */
QuadroAnimacao *getQuadroAnimacaoAtualItemBit( ItemBit *item );