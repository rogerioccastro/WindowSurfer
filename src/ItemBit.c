/**
 * @file ItemBit.c
 * @author @EddiePricefield & @rogerioccastro
 * @brief Implementação do Item (Bit).
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "ItemBit.h"
#include "Macros.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void desenharQuadroAnimacaoItemBit( ItemBit *item, QuadroAnimacao *qa, Color tonalidade );
static Animacao *getAnimacaoAtualItemBit( ItemBit *item );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria um novo Item (bit).
 */
ItemBit *criarItemBit( Rectangle ret, Color cor ) {

    ItemBit *novoItem = (ItemBit*) malloc( sizeof( ItemBit ) );

    novoItem->ret = ret;
    novoItem->cor = cor;
    novoItem->estado = ESTADO_ITEM_BIT_PARADO;
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
        175,             // duração padrão para todos os quadros
        2, 2,            // início
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
        1, 18,            // início
        16, 16,           // dimensões
        1,                // separação
        false,            // de trás para frente
        (Rectangle) { 0 } // retângulo de colisão padrão para cada quadro
    );

    novoItem->animacoes[ESTADO_ITEM_BIT_PARADO] = &novoItem->animacaoParado; quantidadeAnimacoes++;
    novoItem->animacoes[ESTADO_ITEM_BIT_COLETADO] = &novoItem->animacaoColetando; quantidadeAnimacoes++;
    novoItem->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoItem;

}

/**
 * @brief Destroi um item (bit).
 */
void destruirItemBit( ItemBit *item ) {
    if ( item != NULL ) {
        for ( int i = 0; i < item->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( item->animacoes[i] );
        }
        free( item );
    }
}

/**
 * @brief Atualiza um item (bit).
 */
void atualizarItemBit( ItemBit *item, float delta ) {
    if ( item->ativo ) {
        Animacao *animacaoAtual = getAnimacaoAtualItemBit( item );
        atualizarAnimacao( animacaoAtual, delta );
        if ( item->estado == ESTADO_ITEM_BIT_COLETADO && animacaoAtual->finalizada ) {
            item->ativo = false;
        }
    }
}

/**
 * @brief Desenha um item (bit).
 */
void desenharItemBit( ItemBit *item ) {
    if ( item->ativo ) {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualItemBit( item );
        desenharQuadroAnimacaoItemBit( item, qa, WHITE );
        if ( MOSTRAR_RETANGULOS ) {
            DrawRectangleRec( item->ret, Fade( item->cor, 0.5f ) );
            DrawRectangleLines( item->ret.x, item->ret.y, item->ret.width, item->ret.height, BLACK );
        }
    }
}

/**
 * @brief Obtém o quadro de animação atual de um item (bit).
 */
QuadroAnimacao *getQuadroAnimacaoAtualItemBit( ItemBit *item ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualItemBit( item ) );
}

static void desenharQuadroAnimacaoItemBit( ItemBit *item, QuadroAnimacao *qa, Color tonalidade ) {

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

static Animacao *getAnimacaoAtualItemBit( ItemBit *item ) {
    return item->animacoes[item->estado];
}