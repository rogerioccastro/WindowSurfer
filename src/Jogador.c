/**
 * @file Jogador.c
 * @author @EddiePricefield & @rogerioccastro
 * @brief Implementação do Jogador.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "Inimigo.h"
#include "InimigoMalware.h"
#include "InimigoSpyware.h"
#include "InimigoAdware.h"
#include "InimigoPonteiro.h"
#include "Item.h"
#include "ItemBit.h"
#include "ItemByte.h"
#include "ItemAtalho.h"
#include "ItemDefender.h"
#include "Macros.h"
#include "Jogador.h"
#include "ResourceManager.h"
#include "Tipos.h"
#include "Mapa.h"

static void desenharQuadroAnimacaoJogador( Jogador *j, QuadroAnimacao *qa, Color tonalidade );
static QuadroAnimacao *getQuadroAnimacaoAtualJogador( Jogador *j );
static Animacao *getAnimacaoAtualJogador( Jogador *j );

static void resolverColisaoJogadorObstaculosMapaX( Jogador *j, Mapa *mapa );
static void resolverColisaoJogadorObstaculosMapaY( Jogador *j, Mapa *mapa );

static void resolverColisaoJogadorItensMapa( Jogador *j, Mapa *mapa );
static void resolverColisaoJogadorInimigosMapa( Jogador *j, Mapa *mapa );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria uma instância alocada dinamicamente da struct Jogador.
 */
Jogador *criarJogador( float x, float y, float w, float h ) {

    Jogador *novoJogador = (Jogador*) malloc( sizeof( Jogador ) );

    novoJogador->ret.x = x;
    novoJogador->ret.y = y;
    novoJogador->ret.width = w;
    novoJogador->ret.height = h;
    novoJogador->vel = (Vector2) { 0 };

    novoJogador->cor = BLUE;

    novoJogador->velAndando = 50;
    novoJogador->velAndandoRapido = 200;
    novoJogador->velCorrendo = 500;
    novoJogador->velPulo = -650;
    novoJogador->velMaxQueda = 1000;

    novoJogador->aceleracao = 800;
    novoJogador->desaceleracao = 3000;
    novoJogador->frenagem = 8000;

    novoJogador->quantidadePulos = 0;
    novoJogador->quantidadeMaxPulos = 2;

    novoJogador->quantidadeBits = 0;
    novoJogador->quantidadeVidas = 3;
    novoJogador->quantidadeHP = 3;
    novoJogador->quantidadeMaxHP = 3;
    novoJogador->scoreTotal = 0;

    novoJogador->invulneravel = false;
    novoJogador->tempoInvulnerabilidade = 3.0f;
    novoJogador->contadorTempoInvulnerabilidade = 0.0f;

    novoJogador->piscaPisca = false;
    novoJogador->tempoPiscaPisca = 0.05f;
    novoJogador->contadorTempoPiscaPisca = 0.0f;

    novoJogador->freando = false;
    novoJogador->noChao = false;

    novoJogador->coyoteTime = 0.12f;
    novoJogador->contadorCoyoteTime = 0.0f;

    novoJogador->estado = ESTADO_JOGADOR_PARADO;
    novoJogador->olhandoParaDireita = true;

    novoJogador->morreu = false;

    int quantidadeAnimacoes = 0;

    novoJogador->animacaoParado.quantidadeQuadros = 6;
    novoJogador->animacaoParado.quadroAtual = 0;
    novoJogador->animacaoParado.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoParado.pararNoUltimoQuadro = false;
    novoJogador->animacaoParado.executarUmaVez = false;
    novoJogador->animacaoParado.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoParado, novoJogador->animacaoParado.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoJogador->animacaoParado.quadros,
        novoJogador->animacaoParado.quantidadeQuadros,
        200,            // duração padrão para todos os quadros
        0, 0,         // início
        64, 64,          // dimensões
        0,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            30, 20, 30, 76
            //18, 20, 54, 76
        }
    );

    novoJogador->animacaoAndando.quantidadeQuadros = 8;
    novoJogador->animacaoAndando.quadroAtual = 0;
    novoJogador->animacaoAndando.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoAndando.pararNoUltimoQuadro = false;
    novoJogador->animacaoAndando.executarUmaVez = false;
    novoJogador->animacaoAndando.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoAndando, novoJogador->animacaoAndando.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoJogador->animacaoAndando.quadros,
        novoJogador->animacaoAndando.quantidadeQuadros,
        80,              // duração padrão para cada quadro
        0, 0,        // início
        64, 64,          // dimensões
        0,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            20, 20, 45, 76
            //18, 20, 54, 76
        }
    );

    novoJogador->animacaoAndandoRapido.quantidadeQuadros = 8;
    novoJogador->animacaoAndandoRapido.quadroAtual = 0;
    novoJogador->animacaoAndandoRapido.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoAndandoRapido.pararNoUltimoQuadro = false;
    novoJogador->animacaoAndandoRapido.executarUmaVez = false;
    novoJogador->animacaoAndandoRapido.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoAndandoRapido, novoJogador->animacaoAndandoRapido.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoJogador->animacaoAndandoRapido.quadros,
        novoJogador->animacaoAndandoRapido.quantidadeQuadros,
        40,              // duração padrão para cada quadro
        0, 0,        // início
        64, 64,          // dimensões
        0,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            20, 20, 45, 76
            //18, 20, 54, 76
        }
    );

    novoJogador->animacaoCorrendo.quantidadeQuadros = 8;
    novoJogador->animacaoCorrendo.quadroAtual = 0;
    novoJogador->animacaoCorrendo.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoCorrendo.pararNoUltimoQuadro = false;
    novoJogador->animacaoCorrendo.executarUmaVez = false;
    novoJogador->animacaoCorrendo.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoCorrendo, novoJogador->animacaoCorrendo.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoJogador->animacaoCorrendo.quadros,
        novoJogador->animacaoCorrendo.quantidadeQuadros,
        20,              // duração padrão para cada quadro
        0, 0,         // início
        64, 64,          // dimensões
        0,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            20, 20, 45, 76
            //18, 20, 54, 76
        }
    );

    novoJogador->animacaoPulando.quantidadeQuadros = 8;
    novoJogador->animacaoPulando.quadroAtual = 0;
    novoJogador->animacaoPulando.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoPulando.pararNoUltimoQuadro = true;
    novoJogador->animacaoPulando.executarUmaVez = false;
    novoJogador->animacaoPulando.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoPulando, novoJogador->animacaoPulando.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoJogador->animacaoPulando.quadros,
        novoJogador->animacaoPulando.quantidadeQuadros,
        40,              // duração padrão para cada quadro
        0, 0,        // início
        64, 64,          // dimensões
        0,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            20, 20, 45, 76
            //18, 36, 60, 60
        }
    );

    novoJogador->animacaoPulandoRapido.quantidadeQuadros = 8;
    novoJogador->animacaoPulandoRapido.quadroAtual = 0;
    novoJogador->animacaoPulandoRapido.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoPulandoRapido.pararNoUltimoQuadro = true;
    novoJogador->animacaoPulandoRapido.executarUmaVez = false;
    novoJogador->animacaoPulandoRapido.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoPulandoRapido, novoJogador->animacaoPulandoRapido.quantidadeQuadros );
    inicializarQuadrosAnimacao(
        novoJogador->animacaoPulandoRapido.quadros,
        novoJogador->animacaoPulandoRapido.quantidadeQuadros,
        25,              // duração padrão para cada quadro
        0, 0,        // início
        64, 64,          // dimensões
        0,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            20, 20, 45, 76
            //18, 36, 60, 60
        }
    );

    novoJogador->animacaoPulandoCorrendo.quantidadeQuadros = 8;
    novoJogador->animacaoPulandoCorrendo.quadroAtual = 0;
    novoJogador->animacaoPulandoCorrendo.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoPulandoCorrendo.pararNoUltimoQuadro = true;
    novoJogador->animacaoPulandoCorrendo.executarUmaVez = false;
    novoJogador->animacaoPulandoCorrendo.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoPulandoCorrendo, novoJogador->animacaoPulandoCorrendo.quantidadeQuadros );
    inicializarQuadrosAnimacao(
        novoJogador->animacaoPulandoCorrendo.quadros,
        novoJogador->animacaoPulandoCorrendo.quantidadeQuadros,
        15,              // duração padrão para cada quadro
        0, 0,        // início
        64, 64,          // dimensões
        0,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            20, 20, 45, 76
            //18, 36, 60, 60
        }
    );

    novoJogador->animacoes[ESTADO_JOGADOR_PARADO] = &novoJogador->animacaoParado; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_ANDANDO] = &novoJogador->animacaoAndando; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_ANDANDO_RAPIDO] = &novoJogador->animacaoAndandoRapido; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_CORRENDO] = &novoJogador->animacaoCorrendo; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_PULANDO] = &novoJogador->animacaoPulando; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_PULANDO_RAPIDO] = &novoJogador->animacaoPulandoRapido; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_PULANDO_CORRENDO] = &novoJogador->animacaoPulandoCorrendo; quantidadeAnimacoes++;
    novoJogador->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoJogador;

}

/**
 * @brief Destrói um objeto Jogador e libera seus recursos.
 */
void destruirJogador( Jogador *j ) {
    if ( j != NULL ) {
        for ( int i = 0; i < j->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( j->animacoes[i] );
        }
        free( j );
    }
}

/**
 * @brief Lê a entrada do usuário e atualiza as velocidades do jogador.
 */
void entradaJogador( Jogador *j, float delta ) {

    EstadoJogador estadoAnterior = j->estado;

    bool direitaDown  = IsKeyDown( KEY_RIGHT )     || ( IsGamepadAvailable( 0 ) && IsGamepadButtonDown( 0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT ) );
    bool esquerdaDown = IsKeyDown( KEY_LEFT )      || ( IsGamepadAvailable( 0 ) && IsGamepadButtonDown( 0, GAMEPAD_BUTTON_LEFT_FACE_LEFT ) );
    bool puloPressed  = IsKeyPressed( KEY_SPACE )  || ( IsGamepadAvailable( 0 ) && IsGamepadButtonDown( 0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN ) );

    if ( direitaDown ) {
        if ( j->vel.x < 0 ) {
            j->vel.x += j->frenagem * delta;
            if ( !j->freando && j->estado == ESTADO_JOGADOR_CORRENDO ) {
                PlaySound( rm.somFrenagem );
                j->freando = true;
            }
            if ( j->vel.x > 0 ) {
                j->vel.x = 0;
                j->freando = false;
            }
        } else {
            j->vel.x += j->aceleracao * delta;
            if ( j->vel.x > j->velCorrendo ) {
                j->vel.x = j->velCorrendo;
            }
        }
        j->olhandoParaDireita = true;
    } else if ( esquerdaDown ) {
        if ( j->vel.x > 0 ) {
            j->vel.x -= j->frenagem * delta;
            if ( !j->freando && j->estado == ESTADO_JOGADOR_CORRENDO ) {
                PlaySound( rm.somFrenagem );
                j->freando = true;
            }
            if ( j->vel.x < 0 ) {
                j->vel.x = 0;
                j->freando = false;
            }
        } else {
            j->vel.x -= j->aceleracao * delta;
            if ( j->vel.x < -j->velCorrendo ) {
                j->vel.x = -j->velCorrendo;
            }
        }
        j->olhandoParaDireita = false;
    } else {
        if ( j->vel.x > 0 ) {
            j->vel.x -= j->desaceleracao * delta;
            if ( j->vel.x < 0 ) {
                j->vel.x = 0;
            }
        } else if ( j->vel.x < 0 ) {
            j->vel.x += j->desaceleracao * delta;
            if ( j->vel.x > 0 ) {
                j->vel.x = 0;
            }
        }
    }



    float absVelX = fabsf( j->vel.x );
    if ( j->quantidadePulos > 0 ) {
        if ( absVelX <= j->velAndando ) {
            j->estado = ESTADO_JOGADOR_PULANDO;
        } else if ( absVelX <= j->velAndandoRapido ) {
            j->estado = ESTADO_JOGADOR_PULANDO_RAPIDO;
        } else {
            j->estado = ESTADO_JOGADOR_PULANDO_CORRENDO;
        }
    
    if (j -> quantidadePulos == 1) {
        j->velPulo = -500;
    } else {
        j->velPulo = -650;
    }
    
    } else if ( absVelX < 1.0f ) {
        j->estado = ESTADO_JOGADOR_PARADO;
    } else if ( absVelX <= j->velAndando ) {
        j->estado = ESTADO_JOGADOR_ANDANDO;
    } else if ( absVelX <= j->velAndandoRapido ) {
        j->estado = ESTADO_JOGADOR_ANDANDO_RAPIDO;
    } else {
        j->estado = ESTADO_JOGADOR_CORRENDO;
    }

    if ( puloPressed && j->quantidadePulos < j->quantidadeMaxPulos ) {
        j->vel.y = j->velPulo;
        j->quantidadePulos++;
        PlaySound( rm.somPulo );
    }

    // sincronização de animações andando e andando rápido
    if ( estadoAnterior == ESTADO_JOGADOR_ANDANDO && j->estado == ESTADO_JOGADOR_ANDANDO_RAPIDO ) {
        sincronizarAnimacao( &j->animacaoAndandoRapido, &j->animacaoAndando );
    } else if ( estadoAnterior == ESTADO_JOGADOR_ANDANDO_RAPIDO && j->estado == ESTADO_JOGADOR_ANDANDO ) {
        sincronizarAnimacao( &j->animacaoAndando, &j->animacaoAndandoRapido );
    }

}

/**
 * @brief Aplica física e resolve colisões do jogador com o mundo.
 */
void atualizarJogador( Jogador *j, GameWorld *gw, float delta ) {

    if ( j->invulneravel ) {

        j->contadorTempoPiscaPisca += delta;
        if ( j->contadorTempoPiscaPisca >= j->tempoPiscaPisca ) {
            j->contadorTempoPiscaPisca = 0.0f;
            j->piscaPisca = !j->piscaPisca;
        }

        j->contadorTempoInvulnerabilidade += delta;
        if ( j->contadorTempoInvulnerabilidade >= j->tempoInvulnerabilidade ) {
            j->contadorTempoInvulnerabilidade = 0.0f;
            j->invulneravel = false;
            j->contadorTempoPiscaPisca = 0.0f;
            j->piscaPisca = false;
        }

    }

    Animacao *animacaoAtual = getAnimacaoAtualJogador( j );
    atualizarAnimacao( animacaoAtual, delta );

    // fase X: move horizontalmente e resolve colisões laterais
    j->ret.x += j->vel.x * delta;
    resolverColisaoJogadorObstaculosMapaX( j, gw->mapa );

    // fase Y: aplica gravidade, move verticalmente e resolve colisões verticais
    j->vel.y += gw->gravidade * delta;
    if ( j->vel.y > j->velMaxQueda ) {
        j->vel.y = j->velMaxQueda;
    }
    j->ret.y += j->vel.y * delta;
    resolverColisaoJogadorObstaculosMapaY( j, gw->mapa );
    
    if(j->noChao) {
        j->contadorCoyoteTime = 0.0f;
    } else {
        j->contadorCoyoteTime += delta;
    }

    if(!j->noChao && j->contadorCoyoteTime >= j->coyoteTime && j->quantidadePulos == 0) {
        j->quantidadeMaxPulos = 0;
    } else {
        j->quantidadeMaxPulos = 2;
    }
    
    if(j->ret.y > calcularAlturaMapa(gw->mapa)) {
        j->morreu = true;
    }

    resolverColisaoJogadorItensMapa( j, gw->mapa );
    resolverColisaoJogadorInimigosMapa( j, gw->mapa );

}

/**
 * @brief Desenha o jogador.
 */
void desenharJogador( Jogador *j ) {

    if ( !j->piscaPisca ) {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );
        desenharQuadroAnimacaoJogador( j, qa, WHITE );
    }

    if ( MOSTRAR_RETANGULOS ) {
        DrawRectangleRec( j->ret, Fade( j->cor, 0.5f ) );
        DrawRectangleLines( j->ret.x, j->ret.y, j->ret.width, j->ret.height, BLACK );
    }

}

static void desenharQuadroAnimacaoJogador( Jogador *j, QuadroAnimacao *qa, Color tonalidade ) {

    if ( qa != NULL ) {

        Texture2D texturaUsada = ( j->estado == ESTADO_JOGADOR_PARADO ) 
            ? rm.texturaJogadorParado 
            : rm.texturaJogador;

        DrawTexturePro(
            texturaUsada,  // ALTERADO
            (Rectangle) {
                qa->fonte.x,
                qa->fonte.y,
                j->olhandoParaDireita ? qa->fonte.width : -qa->fonte.width,
                qa->fonte.height
            },
            j->ret,
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULOS ) {
            float xDesenho = j->olhandoParaDireita
                ? j->ret.x + qa->retColisao.x
                : j->ret.x + j->ret.width - qa->retColisao.x - qa->retColisao.width;
            float yDesenho = j->ret.y + qa->retColisao.y;
            DrawRectangle( xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade( GREEN, 0.5f ) );
        }

    }

}

static QuadroAnimacao *getQuadroAnimacaoAtualJogador( Jogador *j ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualJogador( j ) );
}

static Animacao *getAnimacaoAtualJogador( Jogador *j ) {
    return j->animacoes[j->estado];
}

/**
 * @brief Resolve colisões do jogador com o mapa no eixo X.
 */
static void resolverColisaoJogadorObstaculosMapaX( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->obstaculos;

    while ( el != NULL ) {

        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );

        float deslocamentoX = j->olhandoParaDireita
            ? qa->retColisao.x
            : j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            j->ret.x + deslocamentoX,
            j->ret.y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        Obstaculo *o = (Obstaculo*) el->objeto;

        if ( CheckCollisionRecs( retColCalculado, o->ret ) ) {
            if ( retColCalculado.x + retColCalculado.width / 2 < o->ret.x + o->ret.width / 2 ) {
                j->ret.x = o->ret.x - qa->retColisao.width - deslocamentoX;
            } else {
                j->ret.x = o->ret.x + o->ret.width - deslocamentoX;
            }
            j->vel.x = 0;
        }

        el = el->proximo;

    }

}

/**
 * @brief Resolve colisões do jogador com o mapa no eixo Y.
 */
static void resolverColisaoJogadorObstaculosMapaY( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->obstaculos;
    j->noChao = false;

    while ( el != NULL ) {

        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );

        float deslocamentoX = j->olhandoParaDireita
            ? qa->retColisao.x
            : j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            j->ret.x + deslocamentoX,
            j->ret.y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        Obstaculo *o = (Obstaculo*) el->objeto;

        if ( CheckCollisionRecs( retColCalculado, o->ret ) ) {
            if ( retColCalculado.y + retColCalculado.height / 2 < o->ret.y + o->ret.height / 2 ) {
                j->ret.y = o->ret.y - qa->retColisao.height - deslocamentoY;
                j->quantidadePulos = 0;
                j->noChao = true;
            } else {
                j->ret.y = o->ret.y + o->ret.height - deslocamentoY;
            }
            j->vel.y = 0;
        }

        el = el->proximo;

    }

}

static void resolverColisaoJogadorItensMapa( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->itens;

    while ( el != NULL ) {

        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );

        float deslocamentoX = j->olhandoParaDireita
            ? qa->retColisao.x
            : j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            j->ret.x + deslocamentoX,
            j->ret.y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        Item *item = (Item*) el->objeto;

        if ( item->tipo == TIPO_ITEM_BIT ) {

            ItemBit *itemBit = (ItemBit*) item->objeto;

            if ( !itemBit->ativo || itemBit->estado == ESTADO_ITEM_BIT_COLETADO ) {
                el = el->proximo;
                continue;
            }

            QuadroAnimacao *qaItem = getQuadroAnimacaoAtualItemBit( itemBit );
            
            Rectangle retColItemCalculado = {
                itemBit->ret.x + qaItem->retColisao.x,
                itemBit->ret.y + qaItem->retColisao.y,
                qaItem->retColisao.width,
                qaItem->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColItemCalculado ) ) {
                itemBit->estado = ESTADO_ITEM_BIT_COLETADO;
                j->quantidadeBits++;
                PlaySound( rm.somColeta );
            }

        } else if ( item->tipo == TIPO_ITEM_BYTE ) {

            ItemByte *itemByte = (ItemByte*) item->objeto;

            if ( !itemByte->ativo || itemByte->estado == ESTADO_ITEM_BYTE_COLETADO ) {
                el = el->proximo;
                continue;
            }

            QuadroAnimacao *qaItem = getQuadroAnimacaoAtualItemByte( itemByte );
            
            Rectangle retColItemCalculado = {
                itemByte->ret.x + qaItem->retColisao.x,
                itemByte->ret.y + qaItem->retColisao.y,
                qaItem->retColisao.width,
                qaItem->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColItemCalculado ) ) {
                itemByte->estado = ESTADO_ITEM_BYTE_COLETADO;
                j->quantidadeBits += 8;
                PlaySound( rm.somColeta );
            }

        } else if ( item->tipo == TIPO_ITEM_ATALHO ) {

            ItemAtalho *itemAtalho = (ItemAtalho*) item->objeto;

            if ( !itemAtalho->ativo || itemAtalho->estado == ESTADO_ITEM_ATALHO_COLETADO ) {
                el = el->proximo;
                continue;
            }

            QuadroAnimacao *qaItem = getQuadroAnimacaoAtualItemAtalho( itemAtalho );
            
            Rectangle retColItemCalculado = {
                itemAtalho->ret.x + qaItem->retColisao.x,
                itemAtalho->ret.y + qaItem->retColisao.y,
                qaItem->retColisao.width,
                qaItem->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColItemCalculado ) ) {
                itemAtalho->estado = ESTADO_ITEM_ATALHO_COLETADO;
                PlaySound( rm.somColeta );
            }

        } else if ( item->tipo == TIPO_ITEM_DEFENDER ) {

            ItemDefender *itemDefender = (ItemDefender*) item->objeto;

            if ( !itemDefender->ativo || itemDefender->estado == ESTADO_ITEM_DEFENDER_COLETADO ) {
                el = el->proximo;
                continue;
            }

            QuadroAnimacao *qaItem = getQuadroAnimacaoAtualItemDefender( itemDefender );
            
            Rectangle retColItemCalculado = {
                itemDefender->ret.x + qaItem->retColisao.x,
                itemDefender->ret.y + qaItem->retColisao.y,
                qaItem->retColisao.width,
                qaItem->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColItemCalculado ) ) {
                itemDefender->estado = ESTADO_ITEM_DEFENDER_COLETADO;
                if ( j->quantidadeHP < 3){
                    j->quantidadeHP += 1;
                }
                PlaySound( rm.somColeta );
            }

        }

        el = el->proximo;

    }

}

static void resolverColisaoJogadorInimigosMapa( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->inimigos;

    while ( el != NULL ) {

        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );

        float deslocamentoX = j->olhandoParaDireita
            ? qa->retColisao.x
            : j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            j->ret.x + deslocamentoX,
            j->ret.y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        Inimigo *inimigo = (Inimigo*) el->objeto;

        QuadroAnimacao *qaInimigo = NULL;
        bool *olhandoParaDireita = NULL;
        Rectangle *ret = NULL;

        if ( inimigo->tipo == TIPO_INIMIGO_MALWARE ) {

            InimigoMalware *malware = (InimigoMalware*) inimigo->objeto;

            if ( !malware->ativo || malware->estado == ESTADO_INIMIGO_MALWARE_MORRENDO ) {
                el = el->proximo;
                continue;
            }

            qaInimigo = getQuadroAnimacaoAtualInimigoMalware( malware );
            olhandoParaDireita = &malware->olhandoParaDireita;
            ret = &malware->ret;

            float deslocamentoX = *olhandoParaDireita
                ? ret->width - qaInimigo->retColisao.x - qaInimigo->retColisao.width
                : qaInimigo->retColisao.x;
            float deslocamentoY = qaInimigo->retColisao.y;

            Rectangle retColInimigoCalculado = {
                ret->x + deslocamentoX,
                ret->y + deslocamentoY,
                qaInimigo->retColisao.width,
                qaInimigo->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColInimigoCalculado ) ) {

                if ( (j->estado >= ESTADO_JOGADOR_PULANDO && j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO) || ((j->vel.y > 0) && (j->estado >= ESTADO_JOGADOR_PARADO && j->estado <= ESTADO_JOGADOR_CORRENDO)) ) {
                    j->vel.y = j->velPulo;
                    malware->estado = ESTADO_INIMIGO_MALWARE_MORRENDO;
                    j->scoreTotal += 100;
                    PlaySound( rm.somHitInimigo );
                } else if ( !j->invulneravel ) {
                    if ( j->quantidadeHP > 0 ) {
                        j->quantidadeHP--;
                        PlaySound( rm.somHit );
                    } else {
                        j->morreu = true;
                        PlaySound( rm.somMorte );
                    }
                    j->invulneravel = true;
                }

                return; // um inimigo de cada vez!

            }

        } else if ( inimigo->tipo == TIPO_INIMIGO_SPYWARE ) {

            InimigoSpyware *spyware = (InimigoSpyware*) inimigo->objeto;

            if ( !spyware->ativo || spyware->estado == ESTADO_INIMIGO_SPYWARE_MORRENDO ) {
                el = el->proximo;
                continue;
            }

            qaInimigo = getQuadroAnimacaoAtualInimigoSpyware( spyware );
            olhandoParaDireita = &spyware->olhandoParaDireita;
            ret = &spyware->ret;

            float deslocamentoX = *olhandoParaDireita
                ? ret->width - qaInimigo->retColisao.x - qaInimigo->retColisao.width
                : qaInimigo->retColisao.x;
            float deslocamentoY = qaInimigo->retColisao.y;

            Rectangle retColInimigoCalculado = {
                ret->x + deslocamentoX,
                ret->y + deslocamentoY,
                qaInimigo->retColisao.width,
                qaInimigo->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColInimigoCalculado ) ) {

                if ( (j->estado >= ESTADO_JOGADOR_PULANDO && j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO) || ((j->vel.y > 0) && (j->estado >= ESTADO_JOGADOR_PARADO && j->estado <= ESTADO_JOGADOR_CORRENDO)) ) {
                    j->vel.y = j->velPulo;
                    spyware->estado = ESTADO_INIMIGO_SPYWARE_MORRENDO;
                    j->scoreTotal += 100;
                    PlaySound( rm.somHitInimigo );
                } else if ( !j->invulneravel ) {
                    if ( j->quantidadeHP > 0 ) {
                        j->quantidadeHP--;
                        PlaySound( rm.somHit );
                    } else {
                        j->morreu = true;
                        PlaySound( rm.somMorte );
                    }
                    j->invulneravel = true;
                }

                return; // um inimigo de cada vez!

            }

        } else if ( inimigo->tipo == TIPO_INIMIGO_ADWARE ) {

            InimigoAdware *adware = (InimigoAdware*) inimigo->objeto;

            if ( !adware->ativo || adware->estado == ESTADO_INIMIGO_ADWARE_MORRENDO ) {
                el = el->proximo;
                continue;
            }

            qaInimigo = getQuadroAnimacaoAtualInimigoAdware( adware );
            olhandoParaDireita = &adware->olhandoParaDireita;
            ret = &adware->ret;

            float deslocamentoX = *olhandoParaDireita
                ? ret->width - qaInimigo->retColisao.x - qaInimigo->retColisao.width
                : qaInimigo->retColisao.x;
            float deslocamentoY = qaInimigo->retColisao.y;

            Rectangle retColInimigoCalculado = {
                ret->x + deslocamentoX,
                ret->y + deslocamentoY,
                qaInimigo->retColisao.width,
                qaInimigo->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColInimigoCalculado ) ) {

                if ( j->vel.y > 0 && ((retColCalculado.y + retColCalculado.height) <= (retColInimigoCalculado.y + retColInimigoCalculado.height * 0.75)) && j->estado >= ESTADO_JOGADOR_PULANDO && j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO ) {
                    j->vel.y = j->velPulo;
                    adware->estado = ESTADO_INIMIGO_ADWARE_MORRENDO;
                    j->scoreTotal += 100;
                    PlaySound( rm.somHitInimigo );
                } else if ( !j->invulneravel ) {
                    if ( j->quantidadeHP > 0 ) {
                        j->quantidadeHP--;
                        PlaySound( rm.somHit );
                    } else {
                        j->morreu = true;
                        PlaySound( rm.somMorte );
                    }
                    j->invulneravel = true;
                }

                return; // um inimigo de cada vez!

            }
        } else if ( inimigo->tipo == TIPO_INIMIGO_PONTEIRO ) {

            InimigoPonteiro *ponteiro = (InimigoPonteiro*) inimigo->objeto;

            if ( !ponteiro->ativo || ponteiro->estado == ESTADO_INIMIGO_PONTEIRO_MORRENDO ) {
                el = el->proximo;
                continue;
            }

            qaInimigo = getQuadroAnimacaoAtualInimigoPonteiro( ponteiro );
            olhandoParaDireita = &ponteiro->olhandoParaDireita;
            ret = &ponteiro->ret;

            float deslocamentoX = *olhandoParaDireita
                ? ret->width - qaInimigo->retColisao.x - qaInimigo->retColisao.width
                : qaInimigo->retColisao.x;
            float deslocamentoY = qaInimigo->retColisao.y;

            Rectangle retColInimigoCalculado = {
                ret->x + deslocamentoX,
                ret->y + deslocamentoY,
                qaInimigo->retColisao.width,
                qaInimigo->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColInimigoCalculado ) ) {

                if ( j->vel.y > 0 && ((retColCalculado.y + retColCalculado.height) <= (retColInimigoCalculado.y + retColInimigoCalculado.height * 0.75)) && j->estado >= ESTADO_JOGADOR_PULANDO && j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO ) {
                    j->vel.y = j->velPulo;
                    ponteiro->estado = ESTADO_INIMIGO_PONTEIRO_MORRENDO;
                    j->scoreTotal += 100;
                    PlaySound( rm.somHitInimigo );
                } else if ( !j->invulneravel ) {
                    if ( j->quantidadeHP > 0 ) {
                        j->quantidadeHP--;
                        PlaySound( rm.somHit );
                    } else {
                        j->morreu = true;
                        PlaySound( rm.somMorte );
                    }
                    j->invulneravel = true;
                }

                return; // um inimigo de cada vez!

            }
        }

        el = el->proximo;

    }

}
