/**
 * @file Obstaculo.c
 * @author @EddiePricefield & @rogerioccastro
 * @brief Implementação do Obstáculo.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Obstaculo.h"
#include "Tipos.h"

/**
 * @brief Cria um novo obstáculo.
 */
Obstaculo *criarObstaculo( Rectangle ret, Color cor, Rectangle fonte, Texture2D *textura ) {

    Obstaculo *novoObstaculo = (Obstaculo*) malloc( sizeof( Obstaculo ) );

    novoObstaculo->ret = ret;
    novoObstaculo->cor = cor;
    novoObstaculo->fonte = fonte;
    novoObstaculo->textura = textura;

    return novoObstaculo;

}

/**
 * @brief Destroi um obstáculo.
 */
void destruirObstaculo( Obstaculo *o ) {
    free( o );
}

/**
 * @brief Desenha um obstáculo.
 */
void desenharObstaculo( Obstaculo *o ) {

    if ( o->textura == NULL ) {
        DrawRectangleRec( o->ret, o->cor );
        DrawRectangleLines( o->ret.x, o->ret.y, o->ret.width, o->ret.height, BLACK );
        return;
    }

    DrawTexturePro(
        *o->textura, 
        o->fonte,
        o->ret,
        (Vector2) { 0 },
        0.0f,
        WHITE
    );

}