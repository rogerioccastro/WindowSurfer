/**
 * @file ItemByte.c
 * @author @EddiePricefield & @rogerioccastro
 * @brief Implementação do Item (Byte).
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "ItemByte.h"
#include "Macros.h"
#include "ResourceManager.h"
#include "Tipos.h"
#include "GameWorld.h"

static void desenharQuadroAnimacaoItemByte( ItemByte *item, QuadroAnimacao *qa, Color tonalidade );
static Animacao *getAnimacaoAtualItemByte( ItemByte *item );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria um novo Item (byte).
 */
ItemByte *criarItemByte( Rectangle ret, Color cor ) {

    ItemByte *novoItem = (ItemByte*) malloc( sizeof( ItemByte ) );

    novoItem->ret = ret;
    novoItem->cor = cor;
    novoItem->estado = ESTADO_ITEM_BYTE_PARADO;
    novoItem->ativo = true;

    int quantidadeAnimacoes = 0;

    novoItem->animacaoParado.quantidadeQuadros = 4;
    novoItem->animacaoParado.quadroAtual = 0;
    novoItem->animacaoParado.contadorTempoQuadro = 0.0f;
    novoItem->animacaoParado.pararNoUltimoQuadro = false;
    novoItem->animacaoParado.executarUmaVez = false;
    novoItem->animacaoParado.finalizada = false;
    criarQuadrosAnimacao( &novoItem->animacaoParado, novoItem->animacaoParado.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoItem->animacaoParado.quadros,
        novoItem->animacaoParado.quantidadeQuadros,
        250,             // duração padrão para todos os quadros
        2, 26,           // início
        30, 22,          // dimensões
        2,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            0, 0, 32, 32
        }
    );

    novoItem->animacaoColetando.quantidadeQuadros = 4;
    novoItem->animacaoColetando.quadroAtual = 0;
    novoItem->animacaoColetando.contadorTempoQuadro = 0.0f;
    novoItem->animacaoColetando.pararNoUltimoQuadro = false;
    novoItem->animacaoColetando.executarUmaVez = true;
    novoItem->animacaoColetando.finalizada = false;
    criarQuadrosAnimacao( &novoItem->animacaoColetando, novoItem->animacaoColetando.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoItem->animacaoColetando.quadros,
        novoItem->animacaoColetando.quantidadeQuadros,
        80,               // duração padrão para todos os quadros
        1, 52,            // início
        16, 16,           // dimensões
        1,                // separação
        false,            // de trás para frente
        (Rectangle) { 0 } // retângulo de colisão padrão para cada quadro
    );

    novoItem->animacoes[ESTADO_ITEM_BYTE_PARADO] = &novoItem->animacaoParado; quantidadeAnimacoes++;
    novoItem->animacoes[ESTADO_ITEM_BYTE_COLETADO] = &novoItem->animacaoColetando; quantidadeAnimacoes++;
    novoItem->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoItem;

}

/**
 * @brief Destroi um item (byte).
 */
void destruirItemByte( ItemByte *item ) {
    if ( item != NULL ) {
        for ( int i = 0; i < item->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( item->animacoes[i] );
        }
        free( item );
    }
}

/**
 * @brief Atualiza um item (byte).
 */
void atualizarItemByte( ItemByte *item, float delta ) {
    if ( item->ativo ) {
        Animacao *animacaoAtual = getAnimacaoAtualItemByte( item );
        atualizarAnimacao( animacaoAtual, delta );
        if ( item->estado == ESTADO_ITEM_BYTE_COLETADO && animacaoAtual->finalizada ) {
            item->ativo = false;
            alterarEstadoJogo( ESTADO_JOGO_MAPA2 );
            return;
        }
    }
}

/**
 * @brief Desenha um item (byte).
 */
void desenharItemByte( ItemByte *item ) {
    if ( item->ativo ) {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualItemByte( item );
        desenharQuadroAnimacaoItemByte( item, qa, WHITE );
        if ( MOSTRAR_RETANGULOS ) {
            DrawRectangleRec( item->ret, Fade( item->cor, 0.5f ) );
            DrawRectangleLines( item->ret.x, item->ret.y, item->ret.width, item->ret.height, BLACK );
        }
    }
}

/**
 * @brief Obtém o quadro de animação atual de um item (byte).
 */
QuadroAnimacao *getQuadroAnimacaoAtualItemByte( ItemByte *item ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualItemByte( item ) );
}

static void desenharQuadroAnimacaoItemByte( ItemByte *item, QuadroAnimacao *qa, Color tonalidade ) {

    if ( qa != NULL ) {

        DrawTexturePro(
            rm.texturaItens,
            qa->fonte,
            item->ret,
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULOS ) {
            float xDesenho = item->ret.x + qa->retColisao.x;
            float yDesenho = item->ret.y + qa->retColisao.y;
            DrawRectangle( xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade( GREEN, 0.5f ) );
        }

    }

}

static Animacao *getAnimacaoAtualItemByte( ItemByte *item ) {
    return item->animacoes[item->estado];
}