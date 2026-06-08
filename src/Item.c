/**
 * @file Item.c
 * @author @EddiePricefield & @rogerioccastro
 * @brief Implementação do Item.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Item.h"
#include "ItemBit.h"
#include "ItemByte.h"
#include "Tipos.h"

/**
 * @brief Cria um novo Item.
 */
Item *criarItem( TipoItem tipo ) {

    Item *novoItem = (Item*) malloc( sizeof( Item ) );
    novoItem->objeto = NULL;
    novoItem->tipo = tipo;

    return novoItem;

}

/**
 * @brief Destroi um item.
 */
void destruirItem( Item *item ) {
    if ( item != NULL ) {
        switch ( item->tipo ) {
            case TIPO_ITEM_BIT:
                destruirItemBit( (ItemBit*) item->objeto );
                break;
            case TIPO_ITEM_BYTE:
                destruirItemByte( (ItemByte*) item->objeto );
                break;
            default:
                break;
        }
        free( item );
    }
}

/**
 * @brief Atualiza um item.
 */
void atualizarItem( Item *item, float delta ) {

    switch ( item->tipo ) {
        case TIPO_ITEM_BIT:
            atualizarItemBit( (ItemBit*) item->objeto, delta );
            break;
        case TIPO_ITEM_BYTE:
            atualizarItemByte( (ItemByte*) item->objeto, delta );
            break;
        default:
            return;
    }

}

/**
 * @brief Desenha um item.
 */
void desenharItem( Item *item ) {

    switch ( item->tipo ) {
        case TIPO_ITEM_BIT:
            desenharItemBit( (ItemBit*) item->objeto );
            break;
        case TIPO_ITEM_BYTE:
            desenharItemByte( (ItemByte*) item->objeto );
            break;
        default:
            return;
    }

}