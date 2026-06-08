/**
 * @file Inimigo.c
 * @author @EddiePricefield & @rogerioccastro
 * @brief Implementação do Inimigo (Spyware).
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "Inimigo.h"
#include "InimigoSpyware.h"
#include "Macros.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void desenharQuadroAnimacaoInimigoSpyware( InimigoSpyware *inimigo, QuadroAnimacao *qa, Color tonalidade );
static void desenharQuadroAnimacaoInimigoSpywareMorrendo( InimigoSpyware *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade );
static Animacao *getAnimacaoAtualInimigoSpyware( InimigoSpyware *inimigo );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria um novo Inimigo (spyware).
 */
InimigoSpyware *criarInimigoSpyware( Rectangle ret, Color cor ) {

    InimigoSpyware *novoInimigo = (InimigoSpyware*) malloc( sizeof( InimigoSpyware ) );

    novoInimigo->ret = ret;
    novoInimigo->vel = (Vector2) { 0 };
    novoInimigo->cor = cor;

    novoInimigo->velAndando = 100;
    novoInimigo->velMaxQueda = 600;

    novoInimigo->estado = ESTADO_INIMIGO_SPYWARE_ANDANDO;
    novoInimigo->ativo = true;
    novoInimigo->olhandoParaDireita = false;

    int quantidadeAnimacoes = 0;

    novoInimigo->animacaoAndando.quantidadeQuadros = 4;
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
        2, 34,           // início
        36, 30,          // dimensões
        2,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            15, 8, 50, 53
        }
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

    novoInimigo->animacoes[ESTADO_INIMIGO_SPYWARE_ANDANDO] = &novoInimigo->animacaoAndando; quantidadeAnimacoes++;
    novoInimigo->animacoes[ESTADO_INIMIGO_SPYWARE_MORRENDO] = &novoInimigo->animacaoMorrendo; quantidadeAnimacoes++;
    novoInimigo->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoInimigo;

}

/**
 * @brief Destroi um inimigo (spyware).
 */
void destruirInimigoSpyware( InimigoSpyware *inimigo ) {
    if ( inimigo != NULL ) {
        for ( int i = 0; i < inimigo->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( inimigo->animacoes[i] );
        }
        free( inimigo );
    }
}

/**
 * @brief Atualiza um inimigo (spyware).
 */
void atualizarInimigoSpyware( InimigoSpyware *inimigo, GameWorld *gw, float delta ) {

    if ( inimigo->ativo ) {

        if ( inimigo->estado == ESTADO_INIMIGO_SPYWARE_ANDANDO ) {

            Animacao *animacaoAtual = getAnimacaoAtualInimigoSpyware( inimigo );
            atualizarAnimacao( animacaoAtual, delta );

            Inimigo ini = {
                .objeto = inimigo,
                .tipo = TIPO_INIMIGO_SPYWARE
            };

            if ( inimigo->olhandoParaDireita ) {
                inimigo->vel.x = inimigo->velAndando;
            } else {
                inimigo->vel.x = -inimigo->velAndando;
            }

            // fase X
            inimigo->ret.x += inimigo->vel.x * delta;
            resolverColisaoInimigoObstaculosMapaX( &ini, gw->mapa );

            inimigo->vel.y += gw->gravidade * delta;
            if ( inimigo->vel.y > inimigo->velMaxQueda ) {
                inimigo->vel.y = inimigo->velMaxQueda;
            }

            // fase Y
            inimigo->ret.y += inimigo->vel.y * delta;
            resolverColisaoInimigoObstaculosMapaY( &ini, gw->mapa );

        } else if ( inimigo->estado == ESTADO_INIMIGO_SPYWARE_MORRENDO ) {

            atualizarAnimacao( &inimigo->animacaoMorrendo, delta );

            if ( inimigo->animacaoMorrendo.finalizada ) {
                inimigo->ativo = false;
            }

        }

    }

}

/**
 * @brief Desenha um inimigo (spyware).
 */
void desenharInimigoSpyware( InimigoSpyware *inimigo ) {

    if ( inimigo->ativo ) {

        if ( inimigo->estado == ESTADO_INIMIGO_SPYWARE_ANDANDO ) {
            QuadroAnimacao *qa = getQuadroAnimacaoAtualInimigoSpyware( inimigo );
            desenharQuadroAnimacaoInimigoSpyware( inimigo, qa, (Color){255, 255, 255, 50} );
        } else if ( inimigo->estado == ESTADO_INIMIGO_SPYWARE_MORRENDO ) {
            desenharQuadroAnimacaoInimigoSpywareMorrendo( inimigo, getQuadroAtualAnimacao( &inimigo->animacaoMorrendo ), 2.0f, WHITE );
        }

        if ( MOSTRAR_RETANGULOS ) {
            DrawRectangleRec( inimigo->ret, Fade( inimigo->cor, 0.5f ) );
            DrawRectangleLines( inimigo->ret.x, inimigo->ret.y, inimigo->ret.width, inimigo->ret.height, BLACK );
        }

    }

}

/**
 * @brief Obtém o quadro de animação atual de um inimigo (spyware).
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoSpyware( InimigoSpyware *inimigo ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualInimigoSpyware( inimigo ) );
}

static void desenharQuadroAnimacaoInimigoSpyware( InimigoSpyware *inimigo, QuadroAnimacao *qa, Color tonalidade ) {

    if ( qa != NULL ) {
        
        DrawTexturePro(
            rm.texturaInimigos,
            (Rectangle) {
                qa->fonte.x,
                qa->fonte.y,
                inimigo->olhandoParaDireita ? qa->fonte.width : -qa->fonte.width,
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

static void desenharQuadroAnimacaoInimigoSpywareMorrendo( InimigoSpyware *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade ) {

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

static Animacao *getAnimacaoAtualInimigoSpyware( InimigoSpyware *inimigo ) {
    return inimigo->animacoes[inimigo->estado];
}