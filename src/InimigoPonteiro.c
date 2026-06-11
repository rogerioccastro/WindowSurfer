/**
 * @file InimigoPonteiro.c
 * @author @EddiePricefield & @rogerioccastro
 * @brief Implementação do Inimigo (Ponteiro).
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "Inimigo.h"
#include "InimigoPonteiro.h"
#include "Macros.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void desenharQuadroAnimacaoInimigoPonteiro( InimigoPonteiro *inimigo, QuadroAnimacao *qa, Color tonalidade );
static void desenharQuadroAnimacaoInimigoPonteiroMorrendo( InimigoPonteiro *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade );
static Animacao *getAnimacaoAtualInimigoPonteiro( InimigoPonteiro *inimigo );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria um novo Inimigo (ponteiro).
 */
InimigoPonteiro *criarInimigoPonteiro( Rectangle ret, Color cor ) {

    InimigoPonteiro *novoInimigo = (InimigoPonteiro*) malloc( sizeof( InimigoPonteiro ) );

    novoInimigo->ret = ret;
    novoInimigo->vel = (Vector2) { 0 };
    novoInimigo->cor = cor;

    novoInimigo->velAndando = 100;
    novoInimigo->velMaxQueda = 600;
    novoInimigo->timer = 1.0f;

    novoInimigo->estado = ESTADO_INIMIGO_PONTEIRO_ANDANDO;
    novoInimigo->ativo = true;
    novoInimigo->olhandoParaDireita = false;
    novoInimigo->vistoPeloJogador = false;

    int quantidadeAnimacoes = 0;

    novoInimigo->animacaoAndando.quantidadeQuadros = 1;
    novoInimigo->animacaoAndando.quadroAtual = 0;
    novoInimigo->animacaoAndando.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoAndando.pararNoUltimoQuadro = false;
    novoInimigo->animacaoAndando.executarUmaVez = false;
    novoInimigo->animacaoAndando.finalizada = false;
    criarQuadrosAnimacao( &novoInimigo->animacaoAndando, novoInimigo->animacaoAndando.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoInimigo->animacaoAndando.quadros,
        novoInimigo->animacaoAndando.quantidadeQuadros,
        250,             // duração padrão para todos os quadros
        2, 98,           // início
        36, 42,          // dimensões
        2,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            0, 0, 35, 40
        }
    );

    novoInimigo->animacaoClicando.quantidadeQuadros = 4;
    novoInimigo->animacaoClicando.quadroAtual = 0;
    novoInimigo->animacaoClicando.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoClicando.pararNoUltimoQuadro = false;
    novoInimigo->animacaoClicando.executarUmaVez = true;
    novoInimigo->animacaoClicando.finalizada = false;
    criarQuadrosAnimacao( &novoInimigo->animacaoClicando, novoInimigo->animacaoClicando.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoInimigo->animacaoClicando.quadros,
        novoInimigo->animacaoClicando.quantidadeQuadros,
        100,             // duração padrão para todos os quadros
        2, 98,           // início
        36, 42,         // dimensões
        2,               // separação
        false,           // de trás para frente
        (Rectangle) {0, 0, 35, 40} // retângulo de colisão padrão para cada quadro
    );

    novoInimigo->animacaoMorrendo.quantidadeQuadros = 2;
    novoInimigo->animacaoMorrendo.quadroAtual = 0;
    novoInimigo->animacaoMorrendo.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoMorrendo.pararNoUltimoQuadro = false;
    novoInimigo->animacaoMorrendo.executarUmaVez = true;
    novoInimigo->animacaoMorrendo.finalizada = false;
    criarQuadrosAnimacao( &novoInimigo->animacaoMorrendo, novoInimigo->animacaoMorrendo.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoInimigo->animacaoMorrendo.quadros,
        novoInimigo->animacaoMorrendo.quantidadeQuadros,
        100,              // duração padrão para todos os quadros
        169, 1,           // início
        32, 32,           // dimensões
        1,                // separação
        false,            // de trás para frente
        (Rectangle) { 0 } // retângulo de colisão padrão para cada quadro
    );

    novoInimigo->animacoes[ESTADO_INIMIGO_PONTEIRO_ANDANDO] = &novoInimigo->animacaoAndando; quantidadeAnimacoes++;
    novoInimigo->animacoes[ESTADO_INIMIGO_PONTEIRO_MORRENDO] = &novoInimigo->animacaoMorrendo; quantidadeAnimacoes++;
    novoInimigo->animacoes[ESTADO_INIMIGO_PONTEIRO_CLICANDO] = &novoInimigo->animacaoClicando; quantidadeAnimacoes++;
    novoInimigo->quantidadeAnimacoes = quantidadeAnimacoes;

    novoInimigo->posDestino = (Vector2) { novoInimigo->ret.x, novoInimigo->ret.y };

    return novoInimigo;

}

/**
 * @brief Destroi um inimigo (ponteiro).
 */
void destruirInimigoPonteiro( InimigoPonteiro *inimigo ) {
    if ( inimigo != NULL ) {
        for ( int i = 0; i < inimigo->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( inimigo->animacoes[i] );
        }
        free( inimigo );
    }
}

/**
 * @brief Atualiza um inimigo (ponteiro).
 */
void atualizarInimigoPonteiro( InimigoPonteiro *inimigo, GameWorld *gw, float delta ) {

    if ( inimigo->ativo ) {

        if ( inimigo->estado == ESTADO_INIMIGO_PONTEIRO_ANDANDO ) {

            Animacao *animacaoAtual = getAnimacaoAtualInimigoPonteiro( inimigo );
            atualizarAnimacao( animacaoAtual, delta );

            Inimigo ini = {
                .objeto = inimigo,
                .tipo = TIPO_INIMIGO_PONTEIRO
            };

            if ( inimigo->olhandoParaDireita ) {
                inimigo->vel.x = inimigo->velAndando;
            } else {
                inimigo->vel.x = -inimigo->velAndando;
            }

            Vector2 meioInimigo = (Vector2) { inimigo->ret.x + inimigo->ret.width / 2, inimigo->ret.y + inimigo->ret.height / 2 };
            Vector2 meioJogador = (Vector2) { gw->jogador->ret.x + gw->jogador->ret.width / 2, gw->jogador->ret.y + gw->jogador->ret.height / 2 };

            if((meioInimigo.x >= gw->camera.target.x - (GetScreenWidth() / 2) && meioInimigo.x <= gw->camera.target.x + GetScreenWidth() / 2) && (meioInimigo.y >= gw->camera.target.y - (GetScreenHeight() / 2)&& meioInimigo.y <= gw->camera.target.y + GetScreenHeight() / 2)) {
                inimigo->vistoPeloJogador = true;
            }

            if(inimigo->vistoPeloJogador){

                inimigo->timer += delta;

                if (inimigo->timer >= 1.5f) {
                    inimigo->timer = 0.0f;
                    inimigo->posDestino = meioJogador;
                    inimigo->estado = ESTADO_INIMIGO_PONTEIRO_CLICANDO;
                }

                if (inimigo->estado == !ESTADO_INIMIGO_PONTEIRO_CLICANDO){
                    inimigo->ret.x += 0.02 * (inimigo->posDestino.x - meioInimigo.x);
                    inimigo->ret.y += 0.02 * (inimigo->posDestino.y - meioInimigo.y);
                }
                
            } else{
                inimigo->ret.x = inimigo->posDestino.x;
                inimigo->ret.y = inimigo->posDestino.y;
            }

            // printf("Posicao %f %f\n", posAnteriorJogador.x, posAnteriorJogador.y);


        } else if ( inimigo->estado == ESTADO_INIMIGO_PONTEIRO_CLICANDO ) {

            atualizarAnimacao( &inimigo->animacaoClicando, delta );

            if ( inimigo->animacaoClicando.finalizada ) {
                inimigo->animacaoClicando.quadroAtual = 0;
                inimigo->animacaoClicando.contadorTempoQuadro = 0.0f;
                inimigo->animacaoClicando.finalizada = false;
                inimigo->estado = ESTADO_INIMIGO_PONTEIRO_ANDANDO;
            }

        } else if ( inimigo->estado == ESTADO_INIMIGO_PONTEIRO_MORRENDO ) {

            atualizarAnimacao( &inimigo->animacaoMorrendo, delta );

            if ( inimigo->animacaoMorrendo.finalizada ) {
                inimigo->ativo = false;
            }

        }

    }

}



/**
 * @brief Desenha um inimigo (ponteiro).
 */
void desenharInimigoPonteiro( InimigoPonteiro *inimigo ) {

    if ( inimigo->ativo ) {

        if ( inimigo->estado == ESTADO_INIMIGO_PONTEIRO_ANDANDO ) {
            QuadroAnimacao *qa = getQuadroAnimacaoAtualInimigoPonteiro( inimigo );
            desenharQuadroAnimacaoInimigoPonteiro( inimigo, qa, WHITE );
        } else if ( inimigo->estado == ESTADO_INIMIGO_PONTEIRO_MORRENDO ) {
            desenharQuadroAnimacaoInimigoPonteiroMorrendo( inimigo, getQuadroAtualAnimacao( &inimigo->animacaoMorrendo ), 2.0f, WHITE );
        } else if ( inimigo->estado == ESTADO_INIMIGO_PONTEIRO_CLICANDO ) {
            desenharQuadroAnimacaoInimigoPonteiro( inimigo, getQuadroAtualAnimacao( &inimigo->animacaoClicando ), WHITE );
        }

        if ( MOSTRAR_RETANGULOS ) {
            DrawRectangleRec( inimigo->ret, Fade( inimigo->cor, 0.5f ) );
            DrawRectangleLines( inimigo->ret.x, inimigo->ret.y, inimigo->ret.width, inimigo->ret.height, BLACK );
        }

    }

}

/**
 * @brief Obtém o quadro de animação atual de um inimigo (ponteiro).
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoPonteiro( InimigoPonteiro *inimigo ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualInimigoPonteiro( inimigo ) );
}

static void desenharQuadroAnimacaoInimigoPonteiro( InimigoPonteiro *inimigo, QuadroAnimacao *qa, Color tonalidade ) {

    if ( qa != NULL ) {
        
        DrawTexturePro(
            rm.texturaInimigos,
            (Rectangle) {
                qa->fonte.x,
                qa->fonte.y,
                inimigo->olhandoParaDireita ? -qa->fonte.width : qa->fonte.width,
                qa->fonte.height
            },
            inimigo->ret,
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULOS ) {
            float xDesenho = inimigo->olhandoParaDireita
                ? inimigo->ret.x + inimigo->ret.width - qa->retColisao.x - qa->retColisao.width
                : inimigo->ret.x + qa->retColisao.x;
            float yDesenho = inimigo->ret.y + qa->retColisao.y;
            DrawRectangle( xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade( GREEN, 0.5f ) );
        }

    }

}

static void desenharQuadroAnimacaoInimigoPonteiroMorrendo( InimigoPonteiro *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade ) {

    if ( qa != NULL ) {
        
        DrawTexturePro(
            rm.texturaMorte,
            qa->fonte,
            (Rectangle) {
                inimigo->ret.x,
                inimigo->ret.y,
                qa->fonte.width * escala,
                qa->fonte.height * escala
            },
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

    }

}

static Animacao *getAnimacaoAtualInimigoPonteiro( InimigoPonteiro *inimigo ) {
    return inimigo->animacoes[inimigo->estado];
}