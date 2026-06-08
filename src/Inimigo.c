/**
 * @file Inimigo.c
 * @author @EddiePricefield & @rogerioccastro
 * @brief Implementação do Inimigo.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Inimigo.h"
#include "InimigoMalware.h"
#include "InimigoSpyware.h"
#include "Tipos.h"

/**
 * @brief Cria um novo Inimigo.
 */
Inimigo *criarInimigo( TipoInimigo tipo ) {

    Inimigo *novoInimigo = (Inimigo*) malloc( sizeof( Inimigo ) );
    novoInimigo->objeto = NULL;
    novoInimigo->tipo = tipo;

    return novoInimigo;

}

/**
 * @brief Destroi um inimigo.
 */
void destruirInimigo( Inimigo *inimigo ) {
    if ( inimigo != NULL ) {
        switch ( inimigo->tipo ) {
            case TIPO_INIMIGO_MALWARE:
                destruirInimigoMalware( (InimigoMalware*) inimigo->objeto );
                break;
            case TIPO_INIMIGO_SPYWARE:
                destruirInimigoSpyware( (InimigoSpyware*) inimigo->objeto );
                break;
            default:
                break;
        }
        free( inimigo );
    }
}

/**
 * @brief Atualiza um inimigo.
 */
void atualizarInimigo( Inimigo *inimigo, GameWorld *gw, float delta ) {

    switch ( inimigo->tipo ) {
        case TIPO_INIMIGO_MALWARE:
            atualizarInimigoMalware( (InimigoMalware*) inimigo->objeto, gw, delta );
            break;
        case TIPO_INIMIGO_SPYWARE:
            atualizarInimigoSpyware( (InimigoSpyware*) inimigo->objeto, gw, delta );
            break;
        default:
            return;
    }

}

/**
 * @brief Desenha um inimigo.
 */
void desenharInimigo( Inimigo *inimigo ) {

    switch ( inimigo->tipo ) {
        case TIPO_INIMIGO_MALWARE:
            desenharInimigoMalware( (InimigoMalware*) inimigo->objeto );
            break;
        case TIPO_INIMIGO_SPYWARE:
            desenharInimigoSpyware( (InimigoSpyware*) inimigo->objeto );
            break;
        default:
            return;
    }

}

/**
 * @brief Resolve colisões do inimigo com o mapa no eixo X.
 */
void resolverColisaoInimigoObstaculosMapaX( Inimigo *inimigo, Mapa *mapa ) {

    ElementoMapa *el = mapa->obstaculos;

    while ( el != NULL ) {

        QuadroAnimacao *qa = NULL;

        bool *olhandoParaDireita = NULL;
        Rectangle *ret = NULL;

        if ( inimigo->tipo == TIPO_INIMIGO_MALWARE ) {
            InimigoMalware *malware = (InimigoMalware*) inimigo->objeto;
            qa = getQuadroAnimacaoAtualInimigoMalware( malware );
            olhandoParaDireita = &malware->olhandoParaDireita;
            ret = &malware->ret;
        } else if ( inimigo->tipo == TIPO_INIMIGO_SPYWARE ) {
            InimigoSpyware *spyware = (InimigoSpyware*) inimigo->objeto;
            qa = getQuadroAnimacaoAtualInimigoSpyware( spyware );
            olhandoParaDireita = &spyware->olhandoParaDireita;
            ret = &spyware->ret;
        } else {
            el = el->proximo;
            continue;
        }

        float deslocamentoX = *olhandoParaDireita
            ? ret->width - qa->retColisao.x - qa->retColisao.width
            : qa->retColisao.x;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            ret->x + deslocamentoX,
            ret->y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        Obstaculo *o = (Obstaculo*) el->objeto;

        if ( CheckCollisionRecs( retColCalculado, o->ret ) ) {
            if ( retColCalculado.x + retColCalculado.width / 2 < o->ret.x + o->ret.width / 2 ) {
                ret->x = o->ret.x - qa->retColisao.width - deslocamentoX;
                *olhandoParaDireita = !*olhandoParaDireita;
            } else {
                ret->x = o->ret.x + o->ret.width - deslocamentoX;
                *olhandoParaDireita = !*olhandoParaDireita;
            }
        }

        el = el->proximo;

    }

}

/**
 * @brief Resolve colisões do inimigo com o mapa no eixo Y.
 */
void resolverColisaoInimigoObstaculosMapaY( Inimigo *inimigo, Mapa *mapa ) {

    ElementoMapa *el = mapa->obstaculos;

    while ( el != NULL ) {

        Obstaculo *o = (Obstaculo*) el->objeto;
        QuadroAnimacao *qa = NULL;

        bool *olhandoParaDireita = NULL;
        Rectangle *ret = NULL;
        Vector2 *vel = NULL;

        if ( inimigo->tipo == TIPO_INIMIGO_MALWARE ) {
            InimigoMalware *malware = (InimigoMalware*) inimigo->objeto;
            qa = getQuadroAnimacaoAtualInimigoMalware( malware );
            olhandoParaDireita = &malware->olhandoParaDireita;
            ret = &malware->ret;
            vel = &malware->vel;
        } else if ( inimigo->tipo == TIPO_INIMIGO_SPYWARE ) {
            InimigoSpyware *spyware = (InimigoSpyware*) inimigo->objeto;
            qa = getQuadroAnimacaoAtualInimigoSpyware( spyware );
            olhandoParaDireita = &spyware->olhandoParaDireita;
            ret = &spyware->ret;
            vel = &spyware->vel;
        } else {
            el = el->proximo;
            continue;
        }

        float deslocamentoX = *olhandoParaDireita
            ? ret->width - qa->retColisao.x - qa->retColisao.width
            : qa->retColisao.x;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            ret->x + deslocamentoX,
            ret->y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        if ( CheckCollisionRecs( retColCalculado, o->ret ) ) {
            if ( retColCalculado.y + retColCalculado.height / 2 < o->ret.y + o->ret.height / 2 ) {
                ret->y = o->ret.y - qa->retColisao.height - deslocamentoY;
            } else {
                ret->y = o->ret.y + o->ret.height - deslocamentoY;
            }
            vel->y = 0;
        }

        el = el->proximo;

    }

}