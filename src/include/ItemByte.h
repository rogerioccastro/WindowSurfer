/**
 * @file ItemByte.h
 * @author @EddiePricefield & @rogerioccastro
 * @brief Declarações das funções do Item (Byte).
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo item (byte).
 */
ItemByte *criarItemByte( Rectangle ret, Color cor );

/**
 * @brief Destroi um item (byte).
 */
void destruirItemByte( ItemByte *item );

/**
 * @brief Atualiza um item (byte).
 */
void atualizarItemByte( ItemByte *item, float delta );

/**
 * @brief Desenha um item (byte).
 */
void desenharItemByte( ItemByte *item );

/**
 * @brief Obtém o quadro de animação atual de um item (byte).
 */
QuadroAnimacao *getQuadroAnimacaoAtualItemByte( ItemByte *item );