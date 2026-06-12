/**
 * @file GameWorld.c
 * @author @EddiePricefield & @rogerioccastro
 * @brief Implementação do GameWorld.
 *
 * @copyright Copyright (c) 2026
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "GameWorld.h"
#include "Jogador.h"
#include "Macros.h"
#include "Mapa.h"
#include "Obstaculo.h"
#include "Tipos.h"
#include "ResourceManager.h"

#include "raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

static void desenharFundo( GameWorld *gw );
static void desenharHud( GameWorld *gw );
static void atualizarCamera( GameWorld *gw );

static void inicializar( GameWorld *gw );
static void reiniciar( GameWorld *gw );

static void desenharNumGrande( const char *texto, Vector2 posicao, int tipoDeTexto );
static void desenharNumPequeno( const char *texto, Vector2 posicao );

static void verificarJogadorMorto( GameWorld *gw );

EstadoJogo estadoJogoAtual = ESTADO_JOGO_MAPA1;
bool mudarFase = false;

/**
 * @brief Cria uma instância alocada dinamicamente da struct GameWorld.
 */
GameWorld *createGameWorld( void ) {
    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );
    inicializar( gw );
    reiniciar( gw );
    return gw;
}

/**
 * @brief Destrói um objeto GameWorld e suas dependências.
 */
void destroyGameWorld( GameWorld *gw ) {
    if ( gw != NULL ) {
        destruirMapa( gw->mapa );
        destruirJogador( gw->jogador );
        free( gw );
    }
}

/**
 * @brief Lê a entrada do usuário e atualiza o estado do jogo.
 */
void updateGameWorld( GameWorld *gw, float delta ) {

    if ( !IsMusicStreamPlaying( rm.musicaFase01 ) ) {
        PlayMusicStream( rm.musicaFase01 );
    } else {
        UpdateMusicStream( rm.musicaFase01 );
    }

    if ( IsKeyPressed( KEY_R ) || mudarFase ) {
        reiniciar( gw );
        mudarFase = false;
        return;
    }

    if ( IsKeyPressed( KEY_P ) ) {
        printf("Posicao X: %f || Posicao Y: %f\n", gw->jogador->ret.x, gw->jogador->ret.y);
    }

    Jogador *j = gw->jogador;
    atualizarMapa( gw->mapa, gw, delta );
    entradaJogador( j, delta );
    atualizarJogador( j, gw, delta );
    atualizarCamera( gw );
    verificarJogadorMorto( gw );

}

/**
 * @brief Desenha o estado do jogo.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( (Color) { 36, 0, 180, 255 } );

    BeginMode2D( gw->camera );
    desenharFundo( gw );
    desenharMapa( gw->mapa );
    desenharJogador( gw->jogador );
    EndMode2D();

    desenharHud( gw );

    // DrawText( TextFormat( "Anéis: %d", gw->jogador->quantidadeBits ), 10, 10, 20, ORANGE );
    // DrawText( TextFormat( "Vidas: %d", gw->jogador->quantidadeVidas ), 10, 30, 20, ORANGE );
    // DrawText( 
    //     TextFormat( 
    //         "Invulnerável: %s%s", 
    //         gw->jogador->invulneravel ? "sim" : "não",
    //         gw->jogador->invulneravel ? TextFormat( " (%.2fs/%.2fs)", gw->jogador->contadorTempoInvulnerabilidade, gw->jogador->tempoInvulnerabilidade ) : ""
    //     ), 
    //     10, 50, 20, ORANGE
    // );
    DrawFPS( 10, 120 );

    EndDrawing();

}

static void desenharHud( GameWorld *gw ) {
    
    // Desenhando os Valores Fixos //

    // DrawTextureRec( rm.texturaHud, (Rectangle){ 2, 36, 79, 99 }, (Vector2){ 10, 10 }, WHITE ); //Hud Superior
    DrawTextureRec( rm.texturaHud, (Rectangle){ 2, 2, 95, 31 }, (Vector2){ 10, 410 }, WHITE ); //Hud Inferior

    // Desenhando os Valores Variáveis //

    // int esquerda = -1;
    // int direita = 1;

    // // Score //
    // const char *textoScore = TextFormat("%d", gw->jogador->scoreTotal);
    // Vector2 posicaoScore = (Vector2){ 700, 10 };
    // desenharNumGrande( textoScore, posicaoScore, -1 );

    // // Tempo //
    // int minutos = (int)GetTime() / 60;
    // int segundos = (int)GetTime() % 60;

    // const char *textoTempo = TextFormat( "%d %02d", minutos, segundos);
    // Vector2 posicaoTempo = (Vector2){ 90, 45 };
    // desenharNumGrande( textoTempo, posicaoTempo, direita );

    // // Anéis //
    const char *textoAneis = TextFormat("%d", gw->jogador->quantidadeBits);
    // Vector2 posicaoAneis = { 154, 78 };
    Vector2 posicaoAneis = { 750, 15 };
    desenharNumGrande( textoAneis, posicaoAneis, -1 );

    // Vidas //
    int vidas = gw->jogador->quantidadeVidas;

    if (vidas < 0) {
        vidas = 99;
    }

    const char *textoVidas = TextFormat("%d", vidas);
    Vector2 posicaoVidas = { 106, 428 };
    desenharNumPequeno(textoVidas, posicaoVidas);

    // HP //
    int hp = gw->jogador->quantidadeHP;
    DrawTextureRec( rm.texturaHP, (Rectangle){ 2, 205 - (48 * (hp + 1) + (3 * hp)), 152, 48 }, (Vector2){ 10, 10 }, (Color){255, 255, 255, 200} );

}

static void desenharNumGrande( const char *texto, Vector2 posicao, int direcaoTexto ) {

    if ( direcaoTexto == -1 ) {
        posicao.x -= TextLength(texto) * 16;
    }

    for (int i = 0; texto[i] != '\0'; i++) {

        if (texto[i] == ' ') {
            DrawTextureRec( rm.texturaHud, (Rectangle){ 95, 178, 5, 13 }, (Vector2){ posicao.x + 6, posicao.y + 8 }, WHITE ); //Dois Pontos do Tempo
            posicao.x += 16;
            continue;
        }
        
        int digito = texto[i] - '0';

        int spriteX = 2 + (digito * (16 + 2));
        
        DrawTextureRec( rm.texturaHud, (Rectangle){ spriteX, 138, 15, 31 }, posicao, WHITE );
        
        posicao.x += 16;
    }

}

static void desenharNumPequeno( const char *texto, Vector2 posicao ) {

    posicao.x -= TextLength(texto) * 16;

    for (int i = 0; texto[i] != '\0'; i++) {

        int digito = texto[i] - '0';
        int spriteX = 2;
        int spriteY = 172;
        
        if (digito >= 0 && digito <= 4) {
            spriteX = 2 + (digito * (16 + 2));
            spriteY = 172;
        } else {
            spriteX = 2 + ((digito - 5) * (16 + 2));
            spriteY = 190;
        }

        DrawTextureRec( rm.texturaHud, (Rectangle){ spriteX, spriteY, 15, 15 }, posicao, WHITE );
        
        posicao.x += 16;
    }

}

static void desenharFundo( GameWorld *gw ) {

    int larguraFundo = rm.texturaFundo.width;
    int larguraCeu = rm.texturaCeu.width;
    int larguraMapa = calcularLarguraMapa( gw->mapa );
    int alturaMapa = calcularAlturaMapa( gw->mapa );
    int repeticoesFundo = larguraMapa / larguraFundo;
    int repeticoesCeu = larguraMapa / larguraCeu;

    int deslocamentoParallax = (int) ( ( gw->camera.target.x / (float) larguraMapa ) * -1200 );
    int deslocamentoParallaxCeu = (int) ( ( gw->camera.target.x / (float) larguraMapa ) * -1600 );

    for ( int i = 0; i <= repeticoesCeu; i++ ) {
        DrawTexture( rm.texturaCeu, (larguraCeu * i - deslocamentoParallaxCeu) - 400, alturaMapa - rm.texturaCeu.height, WHITE );
    }

    for ( int i = 0; i <= repeticoesFundo; i++ ) {
        DrawTexture( rm.texturaFundo, (larguraFundo * i - deslocamentoParallax) - 400, alturaMapa - rm.texturaFundo.height, WHITE );
    }

}

static void atualizarCamera( GameWorld *gw ) {

    Jogador *j = gw->jogador;
    Camera2D *c = &gw->camera;

    c->offset.x = GetScreenWidth() / 2;
    c->offset.y = GetScreenHeight() / 2;

    // O target é arredondado para o inteiro mais próximo para garantir que a
    // translação da câmera ocorra sempre em posições inteiras de pixel. Sem esse
    // arredondamento, o valor float contínuo de ret.x faz os tiles serem
    // renderizados em posições subpixel, causando frestas visíveis entre eles.
    c->target.x = roundf( j->ret.x + j->ret.width / 2.0f );
    c->target.y = roundf( j->ret.y + j->ret.height / 2.0f );

    int minX = GetScreenWidth() / 2;
    int maxX = calcularLarguraMapa( gw->mapa ) - GetScreenWidth() / 2;
    int maxY = calcularAlturaMapa( gw->mapa ) - GetScreenHeight() / 2;

    if ( c->target.x < minX ) {
        c->target.x = minX;
    } else if ( c->target.x > maxX ) {
        c->target.x = maxX;
    }

    if ( c->target.y > maxY ) {
        c->target.y = maxY;
    }

}

static void inicializar( GameWorld *gw ) {

    switch ( estadoJogoAtual ) {
        case ESTADO_JOGO_NADA:
            return;
            break;
        case ESTADO_JOGO_MAPA1:
            gw->mapa = carregarMapa( "resources/mapas/mapa01.txt" );
            gw->jogador = criarJogador( 144, 144, 96, 96 );
            break;
            case ESTADO_JOGO_MAPA2:
            gw->mapa = carregarMapa( "resources/mapas/mapaTeste.txt" );
            gw->jogador = criarJogador( GetScreenWidth() / 2 + 144, calcularAlturaMapa( gw->mapa ) - 1000, 96, 96 );
            break;
    }
    

    gw->camera = (Camera2D) {
        .offset = { 0 },    // deslocamento relativo da câmera em relação ao alvo
        .target = { 0 },    // o alvo da câmera, ou seja, a coordenada em que ela está centralizada
        .rotation = 0.0f,   // rotação da câmera em graus. o pivô é o alvo.
        .zoom = 1.0f        // zoom da câmera. 1.0f significa sem escala
    };

    gw->gravidade = 1200;

    printf("Altura: %d\n", calcularAlturaMapa( gw->mapa ) );

}

static void reiniciar( GameWorld *gw ) {

    destruirMapa( gw->mapa );
    destruirJogador( gw->jogador );

    if ( IsMusicStreamPlaying( rm.musicaFase01 ) ) {
        StopMusicStream( rm.musicaFase01 );
    }

    inicializar( gw );

}

void alterarEstadoJogo(EstadoJogo novoEstado ){
    estadoJogoAtual = novoEstado;
    mudarFase = true;
}

static void verificarJogadorMorto( GameWorld *gw ) {
    if(gw->jogador->morreu) {
        gw->jogador->quantidadeVidas--;
        reiniciar(gw);
    }
}
