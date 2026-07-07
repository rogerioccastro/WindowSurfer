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
#include "GameWindow.h"
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

static void DesenharPopup(Texture2D texture, Rectangle source, Vector2 position, Color tint);
static void DesenharTitulo(Texture2D texture, Rectangle source, Vector2 position, Color tint);
static void DesenharTelaMorte(Texture2D texture, Rectangle source, Vector2 position, Color tint);

EstadoJogo estadoJogoAtual = ESTADO_JOGO_MENU_INICIAL;
EstadoJogo estadoJogoAnterior = ESTADO_JOGO_MAPA1;

// Menu Inicial //
EstadoBotao botaoIniciar = BOTAO_SELECIONADO;
EstadoBotao botaoSair = BOTAO_PARADO;

// Menu de Pausa //
EstadoBotao botaoContinuar = BOTAO_SELECIONADO;
EstadoBotao botaoGuia = BOTAO_PARADO;
EstadoBotao botaoOpcoes = BOTAO_PARADO;
EstadoBotao botaoTelaInicial = BOTAO_PARADO;

// Menu de Guia //
EstadoBotao botaoVoltarDoGuia = BOTAO_PARADO;

// Menu de Opções //
EstadoBotao botaoMusica = BOTAO_SELECIONADO;
EstadoBotao botaoSons = BOTAO_PARADO;
EstadoBotao botaoTelaCheia = BOTAO_PARADO;
EstadoBotao botaoDebug = BOTAO_PARADO;
EstadoBotao botaoVoltarDasOpcoes = BOTAO_PARADO;

float velMusica = 1.0f;
float volMusica = 10;
float volSons = 10;

bool mudarFase = false;
bool iniciarTransicao = false;

bool exibirGuia = false;
bool exibirOpcoes = false;
bool telaCheia = false;
bool debug = false;
bool tocarMusicaBoss = false;
bool iniciarCutsceneVitoria = false;

int tempBits = 0;

float fade = 0;
float tempoFade = 0.5;

float cronometro = 0;
float tempoInicioAnimacaoMorte = 0.0f;

static float cameraShakeDuracao    = 0.0f;
static float cameraShakeIntensidade = 0.0f;

/**
 * @brief Cria uma instância alocada dinamicamente da struct GameWorld.
 */
GameWorld *createGameWorld( void ) {
    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );
    gw->mapa = NULL;
    gw->jogador = NULL;
    inicializar( gw );
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

    if ( botaoIniciar == BOTAO_CLICADO ){

        cronometro += delta;

        if (cronometro >= 0.1 ){
            tempBits = 0;
            estadoJogoAnterior = ESTADO_JOGO_DERROTA;
            alterarEstadoJogo(ESTADO_JOGO_TRANSICAO);
            cronometro = 0;
            botaoIniciar = BOTAO_PARADO;
        }

    }else if ( botaoSair == BOTAO_CLICADO ){

        cronometro += delta;

        if ( cronometro >= 0.1 ){
            CloseWindow();
            cronometro = 0;
            botaoSair = BOTAO_PARADO;
        }

    } else if ( botaoContinuar == BOTAO_CLICADO ){

        cronometro += delta;

        if (cronometro >= 0.1 ){
            estadoJogoAtual = estadoJogoAnterior;
            cronometro = 0;
            botaoContinuar = BOTAO_SELECIONADO;
        }

    } else if ( botaoGuia == BOTAO_CLICADO ){

        cronometro += delta;

        if (cronometro >= 0.1 ){
            exibirGuia = true;
            cronometro = 0;
            botaoGuia = BOTAO_PARADO;
            botaoVoltarDoGuia = BOTAO_SELECIONADO;
        }

    } else if ( botaoOpcoes == BOTAO_CLICADO ){

        cronometro += delta;

        if (cronometro >= 0.1 ){
            exibirOpcoes = true;
            cronometro = 0;
            botaoOpcoes = BOTAO_PARADO;
            botaoMusica = BOTAO_SELECIONADO;
        }

    } else if ( botaoTelaInicial == BOTAO_CLICADO ){

        cronometro += delta;

        if (cronometro >= 0.1 ){
            estadoJogoAnterior = -1;
            alterarEstadoJogo(ESTADO_JOGO_TRANSICAO);
            cronometro = 0;
            botaoTelaInicial = BOTAO_PARADO;
            botaoIniciar = BOTAO_SELECIONADO;
            botaoContinuar = BOTAO_SELECIONADO;
        }

    } else if ( botaoVoltarDoGuia == BOTAO_CLICADO ){

        cronometro += delta;

        if (cronometro >= 0.1 ){
            exibirGuia = false;
            cronometro = 0;
            botaoGuia = BOTAO_SELECIONADO;
            botaoVoltarDoGuia = BOTAO_PARADO;
        }

    } else if ( botaoVoltarDasOpcoes == BOTAO_CLICADO ){

        cronometro += delta;

        if (cronometro >= 0.1 ){
            exibirOpcoes = false;
            cronometro = 0;
            botaoOpcoes = BOTAO_SELECIONADO;
            botaoMusica = BOTAO_PARADO;
            botaoSons = BOTAO_PARADO;
            botaoTelaCheia = BOTAO_PARADO;
            botaoDebug = BOTAO_PARADO;
            botaoVoltarDasOpcoes = BOTAO_PARADO;
        }

    }

    switch (estadoJogoAtual){
        case ESTADO_JOGO_MENU_INICIAL:

            if (fade > 0){
                fade -= 255 / tempoFade * delta;
            } else{
                fade = 0;
            }

            if (botaoIniciar == BOTAO_SELECIONADO){

                if ( IsKeyPressed( KEY_ENTER ) ) { 

                    botaoIniciar = BOTAO_CLICADO;
                    PlaySound(rm.somColeta);
                    
                } else if ( IsKeyPressed( KEY_DOWN ) || IsKeyPressed( KEY_S ) ){
                    PlaySound(rm.somClick);
                    botaoIniciar = BOTAO_PARADO;
                    botaoSair = BOTAO_SELECIONADO;
                }

            }

            if (botaoSair == BOTAO_SELECIONADO){

                if ( IsKeyPressed( KEY_ENTER ) ) { 

                    botaoSair = BOTAO_CLICADO;
                    PlaySound(rm.somColeta);
                    
                } else if ( IsKeyPressed( KEY_UP ) || IsKeyPressed( KEY_W ) ){
                    PlaySound(rm.somClick);
                    botaoSair = BOTAO_PARADO;
                    botaoIniciar = BOTAO_SELECIONADO;
                }

            }
            
            break;
        case ESTADO_JOGO_MENU_PAUSA:

            if ( IsKeyPressed ( KEY_ESCAPE ) ){
                if (exibirGuia){
                    PlaySound(rm.somColeta);
                    botaoVoltarDoGuia = BOTAO_CLICADO;
                }else if (exibirOpcoes){
                    PlaySound(rm.somColeta);
                    botaoVoltarDasOpcoes = BOTAO_CLICADO;
                }else{
                    PlaySound(rm.somFecharMenu);
                    estadoJogoAtual = estadoJogoAnterior;
                }
            }
        

            if (botaoContinuar == BOTAO_SELECIONADO){

                if ( IsKeyPressed( KEY_ENTER ) ) { 
                    botaoContinuar = BOTAO_CLICADO;
                    PlaySound(rm.somColeta);
                } else if ( IsKeyPressed( KEY_UP ) || IsKeyPressed( KEY_W ) ){
                    PlaySound(rm.somClick);
                    botaoContinuar = BOTAO_PARADO;
                    botaoTelaInicial = BOTAO_SELECIONADO;
                } else if ( IsKeyPressed( KEY_DOWN ) || IsKeyPressed( KEY_S ) ){
                    PlaySound(rm.somClick);
                    botaoContinuar = BOTAO_PARADO;
                    botaoGuia = BOTAO_SELECIONADO;
                } 

            }else if (botaoGuia == BOTAO_SELECIONADO){

                if ( IsKeyPressed( KEY_ENTER ) ) { 

                    botaoGuia = BOTAO_CLICADO;
                    PlaySound(rm.somColeta);
                    
                } else if ( IsKeyPressed( KEY_UP ) || IsKeyPressed( KEY_W ) ){
                    PlaySound(rm.somClick);
                    botaoGuia = BOTAO_PARADO;
                    botaoContinuar = BOTAO_SELECIONADO;
                } else if ( IsKeyPressed( KEY_DOWN ) || IsKeyPressed( KEY_S ) ){
                    PlaySound(rm.somClick);
                    botaoGuia = BOTAO_PARADO;
                    botaoOpcoes = BOTAO_SELECIONADO;
                }

            }else if (botaoOpcoes == BOTAO_SELECIONADO){

                if ( IsKeyPressed( KEY_ENTER ) ) { 

                    botaoOpcoes = BOTAO_CLICADO;
                    PlaySound(rm.somColeta);
                    
                } else if ( IsKeyPressed( KEY_UP ) || IsKeyPressed( KEY_W ) ){
                    PlaySound(rm.somClick);
                    botaoOpcoes = BOTAO_PARADO;
                    botaoGuia = BOTAO_SELECIONADO;
                } else if ( IsKeyPressed( KEY_DOWN ) || IsKeyPressed( KEY_S ) ){
                    PlaySound(rm.somClick);
                    botaoOpcoes = BOTAO_PARADO;
                    botaoTelaInicial = BOTAO_SELECIONADO;
                }

            }else if (botaoTelaInicial == BOTAO_SELECIONADO){

                if ( IsKeyPressed( KEY_ENTER ) ) { 

                    botaoTelaInicial = BOTAO_CLICADO;
                    PlaySound(rm.somColeta);
                    
                } else if ( IsKeyPressed( KEY_UP ) || IsKeyPressed( KEY_W ) ){
                    PlaySound(rm.somClick);
                    botaoTelaInicial = BOTAO_PARADO;
                    botaoOpcoes = BOTAO_SELECIONADO;
                } else if ( IsKeyPressed( KEY_DOWN ) || IsKeyPressed( KEY_S ) ){
                    PlaySound(rm.somClick);
                    botaoTelaInicial = BOTAO_PARADO;
                    botaoContinuar = BOTAO_SELECIONADO;
                }

            }else if (botaoVoltarDoGuia == BOTAO_SELECIONADO){

                if ( IsKeyPressed( KEY_ENTER ) ) { 

                    botaoVoltarDoGuia = BOTAO_CLICADO;
                    PlaySound(rm.somColeta);
                    
                }

            } else if (botaoMusica == BOTAO_SELECIONADO){

                SetMusicVolume( rm.musicaFase01, 0.75f * volMusica / 10.0f );
                SetMusicVolume( rm.musicaFase02, 0.75f * volMusica / 10.0f );

                if ( IsKeyPressed( KEY_RIGHT ) || IsKeyPressed( KEY_D )  ) { 

                    PlaySound(rm.somColeta);

                    if (volMusica < 10){
                        volMusica++;
                    }
                    
                } else if ( IsKeyPressed( KEY_LEFT ) || IsKeyPressed( KEY_A ) ){

                    PlaySound(rm.somColeta);

                    if (volMusica > 0){
                        volMusica--;
                    }

                } else if ( IsKeyPressed( KEY_UP ) || IsKeyPressed( KEY_W ) ){
                    PlaySound(rm.somClick);
                    botaoMusica = BOTAO_PARADO;
                    botaoVoltarDasOpcoes = BOTAO_SELECIONADO;
                } else if ( IsKeyPressed( KEY_DOWN ) || IsKeyPressed( KEY_S ) ){
                    PlaySound(rm.somClick);
                    botaoMusica = BOTAO_PARADO;
                    botaoSons = BOTAO_SELECIONADO;
                }

            } else if (botaoSons == BOTAO_SELECIONADO){

                SetSoundVolume( rm.somColeta, 0.8f * volSons / 10.0f );
                SetSoundVolume( rm.somFrenagem, 0.0f * volSons / 10.0f  );
                SetSoundVolume( rm.somHit, 1.2f * volSons / 10.0f  );
                SetSoundVolume( rm.somHitInimigo, 1.3f * volSons / 10.0f  );
                SetSoundVolume( rm.somPulo, 0.8f * volSons / 10.0f  );
                SetSoundVolume( rm.somClick, 0.7f * volSons / 10.0f  );
                SetSoundVolume( rm.somGameOver, 0.7f * volSons / 10.0f  );
                SetSoundVolume( rm.somAbrirMenu, 0.7f * volSons / 10.0f );
                SetSoundVolume( rm.somFecharMenu, 1.0f * volSons / 10.0f );
                SetSoundVolume( rm.somMorte, 1.0f * volSons / 10.0f );
                SetSoundVolume( rm.somBossRisada, 1.0f * volSons / 10.0f );

                if ( IsKeyPressed( KEY_RIGHT ) || IsKeyPressed( KEY_D )  ) { 

                    PlaySound(rm.somColeta);

                    if (volSons < 10){
                        volSons++;
                    }
                    
                } else if ( IsKeyPressed( KEY_LEFT ) || IsKeyPressed( KEY_A ) ){
                    
                    PlaySound(rm.somColeta);

                    if (volSons > 0){
                        volSons--;
                    }

                } else if ( IsKeyPressed( KEY_UP ) || IsKeyPressed( KEY_W ) ){
                    PlaySound(rm.somClick);
                    botaoSons = BOTAO_PARADO;
                    botaoMusica = BOTAO_SELECIONADO;
                } else if ( IsKeyPressed( KEY_DOWN ) || IsKeyPressed( KEY_S ) ){
                    PlaySound(rm.somClick);
                    botaoSons = BOTAO_PARADO;
                    botaoTelaCheia = BOTAO_SELECIONADO;
                }

            } else if (botaoTelaCheia == BOTAO_SELECIONADO){

                if ( IsKeyPressed( KEY_ENTER ) ) { 

                    telaCheia = !telaCheia;
                    habilitarTelaCheia();
                    PlaySound(rm.somColeta);
                    
                } else if ( IsKeyPressed( KEY_UP ) || IsKeyPressed( KEY_W ) ){
                    PlaySound(rm.somClick);
                    botaoTelaCheia = BOTAO_PARADO;
                    botaoSons = BOTAO_SELECIONADO;
                } else if ( IsKeyPressed( KEY_DOWN ) || IsKeyPressed( KEY_S ) ){
                    PlaySound(rm.somClick);
                    botaoTelaCheia = BOTAO_PARADO;
                    botaoDebug = BOTAO_SELECIONADO;
                }

            } else if (botaoDebug == BOTAO_SELECIONADO){

                if ( IsKeyPressed( KEY_ENTER ) ) { 

                    debug = !debug;
                    PlaySound(rm.somColeta);
                    
                } else if ( IsKeyPressed( KEY_UP ) || IsKeyPressed( KEY_W ) ){
                    PlaySound(rm.somClick);
                    botaoDebug = BOTAO_PARADO;
                    botaoTelaCheia = BOTAO_SELECIONADO;
                } else if ( IsKeyPressed( KEY_DOWN ) || IsKeyPressed( KEY_S ) ){
                    PlaySound(rm.somClick);
                    botaoDebug = BOTAO_PARADO;
                    botaoVoltarDasOpcoes = BOTAO_SELECIONADO;
                }

            } else if (botaoVoltarDasOpcoes == BOTAO_SELECIONADO){

                if ( IsKeyPressed( KEY_ENTER ) ) { 

                    botaoVoltarDasOpcoes = BOTAO_CLICADO;
                    PlaySound(rm.somColeta);
                    
                } else if ( IsKeyPressed( KEY_UP ) || IsKeyPressed( KEY_W ) ){
                    PlaySound(rm.somClick);
                    botaoVoltarDasOpcoes = BOTAO_PARADO;
                    botaoDebug = BOTAO_SELECIONADO;
                } else if ( IsKeyPressed( KEY_DOWN ) || IsKeyPressed( KEY_S ) ){
                    PlaySound(rm.somClick);
                    botaoVoltarDasOpcoes = BOTAO_PARADO;
                    botaoMusica = BOTAO_SELECIONADO;
                }

            }

            break;
        case ESTADO_JOGO_DERROTA:
            cronometro += delta;
            
            if(cronometro <= 0.1f){
                SetMusicVolume( rm.musicaFase01, 0.0f );
                PlaySound( rm.somGameOver );
            }

            if (cronometro >= 3){
                estadoJogoAnterior = ESTADO_JOGO_DERROTA;
                alterarEstadoJogo(ESTADO_JOGO_TRANSICAO);
                inicializar(gw);
                cronometro = 0;
                SetMusicVolume( rm.musicaFase01, 0.75f * volMusica / 10.0f );
            }

            break;
        case ESTADO_JOGO_TRANSICAO:
            cronometro += delta;

            if (cronometro <= tempoFade){
                fade += 255 / tempoFade * delta;
            } else{

                cronometro = 0;

                if(estadoJogoAnterior == ESTADO_JOGO_DERROTA){
                    inicializar(gw);
                }

                alterarEstadoJogo(++estadoJogoAnterior);
            }
            break;
        case ESTADO_JOGO_VITORIA:
            SetMusicVolume(rm.musicaFase02, 0);

            int maxX = calcularLarguraMapa( gw->mapa ) - LARGURA_VIRTUAL / 2;
            
            if ( gw->camera.target.x < maxX ) {
                gw->camera.target.x += 300 * delta;
                

                if ( gw->camera.target.x >= maxX ) {
                    gw->camera.target.x = maxX;
                    iniciarCutsceneVitoria = true;
                    PlaySound(rm.somVitoria);
                }
            } 
            
            if ( iniciarCutsceneVitoria ) {
                
                rm.frameCounter++;
                if (rm.frameCounter >= rm.frameDelay) {
                    rm.frameAtual++; 
                    
                    if (rm.frameAtual >= rm.animFrames) {
                        CloseWindow();
                        return; 
                    }
                    
                    unsigned int nextFrameDataOffset = rm.imagemCutsceneVitoria.width * rm.imagemCutsceneVitoria.height * 4 * rm.frameAtual;
                    
                    UpdateTexture(rm.texturaCutsceneVitoria, ((unsigned char *)rm.imagemCutsceneVitoria.data) + nextFrameDataOffset);
                    
                    rm.frameCounter = 0;
                }
            }

            break;
        default:
            
            if (fade > 0){
                fade -= 255 / tempoFade * delta;
            } else{
                fade = 0;
            }
            
            if (estadoJogoAtual == ESTADO_JOGO_MAPA1){
                if ( !IsMusicStreamPlaying( rm.musicaFase01 ) ) {
                    PlayMusicStream( rm.musicaFase01 );
                } else {
                    UpdateMusicStream( rm.musicaFase01 );
                }
            }
            

            if (estadoJogoAtual == ESTADO_JOGO_MAPA2){
                if ( tocarMusicaBoss ){
                    if ( !IsMusicStreamPlaying( rm.musicaFase02 ) ) {
                        PlayMusicStream( rm.musicaFase02 );
                    } else {
                        UpdateMusicStream( rm.musicaFase02 );
                    }
                }
            }

            if ( IsKeyPressed( KEY_R ) || mudarFase ) {
                reiniciar( gw );
                mudarFase = false;
                return;
            }

            if ( IsKeyPressed( KEY_P ) ) {
                printf("Posicao X: %f || Posicao Y: %f\n", gw->jogador->ret.x, gw->jogador->ret.y);
            }

            if ( IsKeyPressed( KEY_ESCAPE ) ) {
                PlaySound( rm.somAbrirMenu );
                estadoJogoAnterior = estadoJogoAtual;
                estadoJogoAtual = ESTADO_JOGO_MENU_PAUSA;
            }

            Jogador *j = gw->jogador;
            atualizarMapa( gw->mapa, gw, delta );
            entradaJogador( j, delta );
            atualizarJogador( j, gw, delta );
            atualizarCamera( gw );
            verificarJogadorMorto( gw );
            
            break;
    }

}

/**
 * @brief Desenha o estado do jogo.
 */
void drawGameWorld( GameWorld *gw ) {

    switch (estadoJogoAtual){
        case ESTADO_JOGO_MENU_INICIAL:
            ClearBackground(BLACK);
            DrawTexture(rm.texturaMenuInicial, 0, 0, WHITE);
            DesenharTitulo( rm.texturaMenuInicial, (Rectangle){801, 0, 338, 208}, (Vector2){ 420, 30 }, WHITE );

            DrawTextureRec(rm.texturaMenuInicial, (Rectangle){801, 313, 164, 137}, (Vector2){ 580, 280 }, WHITE);

            switch (botaoIniciar){
                case BOTAO_PARADO:
                    DrawTextureRec(rm.texturaMenuInicial, (Rectangle){966, 402, 112, 23}, (Vector2){ 608, 330 }, WHITE);
                    break;
                case BOTAO_SELECIONADO:
                    DrawTextureRec(rm.texturaMenuInicial, (Rectangle){1079, 402, 112, 23}, (Vector2){ 608, 330 }, WHITE);
                    break;
                case BOTAO_CLICADO:
                    DrawTextureRec(rm.texturaMenuInicial, (Rectangle){1192, 402, 112, 23}, (Vector2){ 608, 330 }, WHITE);
                    break;
            }
            switch (botaoSair){
                Vector2 posBotaoSair = (Vector2){ 608, 430 };
                case BOTAO_PARADO:
                    DrawTextureRec(rm.texturaMenuInicial, (Rectangle){966, 426, 112, 23}, (Vector2){ 608, 365 }, WHITE);
                    break;
                case BOTAO_SELECIONADO:
                    DrawTextureRec(rm.texturaMenuInicial, (Rectangle){1079, 426, 112, 23}, (Vector2){ 608, 365 }, WHITE);
                    break;
                case BOTAO_CLICADO:
                    DrawTextureRec(rm.texturaMenuInicial, (Rectangle){1192, 426, 112, 23}, (Vector2){ 608, 365 }, WHITE);
                    break;
            }

            break;
        case ESTADO_JOGO_DERROTA:
            ClearBackground(BLACK);
            DesenharTelaMorte( rm.texturaTelaMorte, (Rectangle){0, 0, 800, 450}, (Vector2){ 0, 0 }, WHITE );
            break;
        default:

            if(gw->jogador != NULL){

                BeginMode2D( gw->camera );
                desenharFundo( gw );
                desenharMapa( gw->mapa );
                desenharJogador( gw->jogador );
                EndMode2D();
                desenharHud( gw );

                if (estadoJogoAtual == ESTADO_JOGO_VITORIA && iniciarCutsceneVitoria) {
                    
                    int posX = (LARGURA_VIRTUAL / 2) - (rm.texturaCutsceneVitoria.width / 2);
                    int posY = (ALTURA_VIRTUAL / 2) - (rm.texturaCutsceneVitoria.height / 2);
                    
                    DrawTexture( rm.texturaCutsceneVitoria, posX, posY, WHITE );

                }

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

                if (estadoJogoAtual == ESTADO_JOGO_MENU_PAUSA){

                    if ( IsMusicStreamPlaying( rm.musicaFase01 ) ) {
                        PauseMusicStream( rm.musicaFase01 );
                    } else if ( IsMusicStreamPlaying( rm.musicaFase02 ) ) {
                        PauseMusicStream( rm.musicaFase02 );
                    }

                    DrawRectangle(0, 0, 800, 450, (Color) { 0, 0, 0, 175 });
                    DrawTextureRec(rm.texturaMenuPausa, (Rectangle){1, 1, 165, 194}, (Vector2){318, 128}, WHITE);

                    switch (botaoContinuar){
                        case BOTAO_PARADO:
                            DrawTextureRec(rm.texturaMenuPausa, (Rectangle){167, 1, 112, 23}, (Vector2){ 345, 170 }, WHITE);
                            break;
                        case BOTAO_SELECIONADO:
                            DrawTextureRec(rm.texturaMenuPausa, (Rectangle){280, 1, 112, 23}, (Vector2){ 345, 170 }, WHITE);
                            break;
                        case BOTAO_CLICADO:
                            DrawTextureRec(rm.texturaMenuPausa, (Rectangle){393, 1, 112, 23}, (Vector2){ 345, 170 }, WHITE);
                            break;
                    }  

                    switch (botaoGuia){
                        case BOTAO_PARADO:
                            DrawTextureRec(rm.texturaMenuPausa, (Rectangle){167, 25, 112, 23}, (Vector2){ 345, 205 }, WHITE);
                            break;
                        case BOTAO_SELECIONADO:
                            DrawTextureRec(rm.texturaMenuPausa, (Rectangle){280, 25, 112, 23}, (Vector2){ 345, 205 }, WHITE);
                            break;
                        case BOTAO_CLICADO:
                            DrawTextureRec(rm.texturaMenuPausa, (Rectangle){393, 25, 112, 23}, (Vector2){ 345, 205 }, WHITE);
                            break;
                    } 

                    switch (botaoOpcoes){
                        case BOTAO_PARADO:
                            DrawTextureRec(rm.texturaMenuPausa, (Rectangle){167, 49, 112, 23}, (Vector2){ 345, 240 }, WHITE);
                            break;
                        case BOTAO_SELECIONADO:
                            DrawTextureRec(rm.texturaMenuPausa, (Rectangle){280, 49, 112, 23}, (Vector2){ 345, 240 }, WHITE);
                            break;
                        case BOTAO_CLICADO:
                            DrawTextureRec(rm.texturaMenuPausa, (Rectangle){393, 49, 112, 23}, (Vector2){ 345, 240 }, WHITE);
                            break;
                    } 

                    switch (botaoTelaInicial){
                        case BOTAO_PARADO:
                            DrawTextureRec(rm.texturaMenuPausa, (Rectangle){167, 73, 112, 23}, (Vector2){ 345, 275 }, WHITE);
                            break;
                        case BOTAO_SELECIONADO:
                            DrawTextureRec(rm.texturaMenuPausa, (Rectangle){280, 73, 112, 23}, (Vector2){ 345, 275 }, WHITE);
                            break;
                        case BOTAO_CLICADO:
                            DrawTextureRec(rm.texturaMenuPausa, (Rectangle){393, 73, 112, 23}, (Vector2){ 345, 275 }, WHITE);
                            break;
                    } 

                    if (exibirGuia){

                        DrawTextureRec(rm.texturaMenuGuia, (Rectangle){1, 1, 509, 364}, (Vector2){145, 43}, WHITE);

                        switch (botaoVoltarDoGuia){
                            case BOTAO_PARADO:
                                DrawTextureRec(rm.texturaMenuGuia, (Rectangle){1, 366, 112, 23}, (Vector2){ 345, 360 }, WHITE);
                                break;
                            case BOTAO_SELECIONADO:
                                DrawTextureRec(rm.texturaMenuGuia, (Rectangle){114, 366, 112, 23}, (Vector2){ 345, 360 }, WHITE);
                                break;
                            case BOTAO_CLICADO:
                                DrawTextureRec(rm.texturaMenuGuia, (Rectangle){227, 366, 112, 23}, (Vector2){ 345, 360 }, WHITE);
                                break;
                        } 

                    }

                    if (exibirOpcoes){

                        DrawTextureRec(rm.texturaMenuOpcoes, (Rectangle){1, 1, 165, 194}, (Vector2){318, 128}, WHITE);

                        switch (botaoMusica){
                            case BOTAO_PARADO:
                                DrawTextureRec(rm.texturaMenuOpcoes, (Rectangle){168, 2, 7, 13}, (Vector2){ 389 + (7 * volMusica), 167 }, WHITE);
                                break;
                            case BOTAO_SELECIONADO:
                                DrawTextureRec(rm.texturaMenuOpcoes, (Rectangle){168, 17, 7, 13}, (Vector2){ 389 + (7 * volMusica), 167 }, WHITE);
                                break;
                        }

                        switch (botaoSons){
                            case BOTAO_PARADO:
                                DrawTextureRec(rm.texturaMenuOpcoes, (Rectangle){168, 2, 7, 13}, (Vector2){ 389 + (7 * volSons), 188 }, WHITE);
                                break;
                            case BOTAO_SELECIONADO:
                                DrawTextureRec(rm.texturaMenuOpcoes, (Rectangle){168, 17, 7, 13}, (Vector2){ 389 + (7 * volSons), 188 }, WHITE);
                                break;
                        }

                        switch (botaoTelaCheia){
                            case BOTAO_PARADO:
                                if (telaCheia){
                                    DrawTextureRec(rm.texturaMenuOpcoes, (Rectangle){226, 1, 25, 25}, (Vector2){ 438, 211 }, WHITE);
                                }else{
                                    DrawTextureRec(rm.texturaMenuOpcoes, (Rectangle){176, 1, 25, 25}, (Vector2){ 438, 211 }, WHITE);
                                }
                                break;
                            case BOTAO_SELECIONADO:
                                if (telaCheia){
                                    DrawTextureRec(rm.texturaMenuOpcoes, (Rectangle){251, 1, 25, 25}, (Vector2){ 438, 211 }, WHITE);
                                }else{
                                    DrawTextureRec(rm.texturaMenuOpcoes, (Rectangle){201, 1, 25, 25}, (Vector2){ 438, 211 }, WHITE);
                                }
                                break;
                        }

                        switch (botaoDebug){
                            case BOTAO_PARADO:
                                if (debug){
                                    DrawTextureRec(rm.texturaMenuOpcoes, (Rectangle){226, 1, 25, 25}, (Vector2){ 438, 243 }, WHITE);
                                }else{
                                    DrawTextureRec(rm.texturaMenuOpcoes, (Rectangle){176, 1, 25, 25}, (Vector2){ 438, 243 }, WHITE);
                                }
                                break;
                            case BOTAO_SELECIONADO:
                                if (debug){
                                    DrawTextureRec(rm.texturaMenuOpcoes, (Rectangle){251, 1, 25, 25}, (Vector2){ 438, 243 }, WHITE);
                                }else{
                                    DrawTextureRec(rm.texturaMenuOpcoes, (Rectangle){201, 1, 25, 25}, (Vector2){ 438, 243 }, WHITE);
                                }
                                break;
                        }

                        switch (botaoVoltarDasOpcoes){
                            case BOTAO_PARADO:
                                DrawTextureRec(rm.texturaMenuOpcoes, (Rectangle){167, 32, 112, 23}, (Vector2){ 344, 280 }, WHITE);
                                break;
                            case BOTAO_SELECIONADO:
                                DrawTextureRec(rm.texturaMenuOpcoes, (Rectangle){280, 32, 112, 23}, (Vector2){ 344, 280 }, WHITE);
                                break;
                            case BOTAO_CLICADO:
                                DrawTextureRec(rm.texturaMenuOpcoes, (Rectangle){393, 32, 112, 23}, (Vector2){ 344, 280 }, WHITE);
                                break;
                        } 


                    }
                    
                }

            }

            if (fade > 0){
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, fade});
            }

            break;
    }

}

static void desenharHud( GameWorld *gw ) {
    
    // Desenhando os Valores Fixos //

    // DrawTextureRec( rm.texturaHud, (Rectangle){ 2, 36, 79, 99 }, (Vector2){ 10, 10 }, WHITE ); //Hud Superior
    DrawTextureRec( rm.texturaHud, (Rectangle){ 0, 0, 95, 38 }, (Vector2){ 10, 400 }, WHITE ); //Hud Inferior

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

    const char *textoVidas = TextFormat("%d", vidas);
    Vector2 posicaoVidas = { 90, 413 };
    desenharNumPequeno(textoVidas, posicaoVidas);

    // HP //
    int hp = gw->jogador->quantidadeHP;

    // Desenho dos Pop-Ups //
    Rectangle popUpVisitante = { 0, 0, 167, 144 };
    Rectangle popUpCaveira = { 168, 27, 207, 117 };
    Rectangle popUpAviso = { 376, 48, 144, 96 };
    Rectangle popUpTelaAzul = { 521, 27, 207, 117 };

    if (hp <= 2 && estadoJogoAtual != ESTADO_JOGO_VITORIA){
        DesenharPopup( rm.texturaPopUp, popUpCaveira, (Vector2){ -10, 360 }, WHITE );
        DesenharPopup( rm.texturaPopUp, popUpAviso, (Vector2){ 700, 10 }, WHITE );
    }

    if (hp <= 1 && estadoJogoAtual != ESTADO_JOGO_VITORIA){
        DesenharPopup( rm.texturaPopUp, popUpAviso, (Vector2){ 30, 300 }, WHITE );
        DesenharPopup( rm.texturaPopUp, popUpCaveira, (Vector2){ 620, 70 }, WHITE );
        DesenharPopup( rm.texturaPopUp, popUpVisitante, (Vector2){ 600, 320 }, WHITE );
        DesenharPopup( rm.texturaPopUp, popUpTelaAzul, (Vector2){ -100, 20 }, WHITE );
    }

    if (hp <= 0 && estadoJogoAtual != ESTADO_JOGO_VITORIA){
        DesenharPopup( rm.texturaPopUp, popUpTelaAzul, (Vector2){ 750, 370 }, WHITE );
        DesenharPopup( rm.texturaPopUp, popUpVisitante, (Vector2){ 600, 320 }, WHITE );
        DesenharPopup( rm.texturaPopUp, popUpAviso, (Vector2){ 700, 280 }, WHITE );
        DesenharPopup( rm.texturaPopUp, popUpCaveira, (Vector2){ 480, 360 }, WHITE );
        DesenharPopup( rm.texturaPopUp, popUpVisitante, (Vector2){ 120, 360 }, WHITE );
        DesenharPopup( rm.texturaPopUp, popUpTelaAzul, (Vector2){ 550, 30 }, WHITE );
        DesenharPopup( rm.texturaPopUp, popUpCaveira, (Vector2){ 20, -20 }, WHITE );
        DesenharPopup( rm.texturaPopUp, popUpAviso, (Vector2){ -70, 100 }, WHITE );
    }

    DrawTextureRec( rm.texturaHP, (Rectangle){ 2, 205 - (48 * (hp + 1) + (3 * hp)), 152, 48 }, (Vector2){ 10, 10 }, (Color){255, 255, 255, 200} );

}

static void desenharNumGrande( const char *texto, Vector2 posicao, int direcaoTexto ) {

    if ( direcaoTexto == -1 ) {
        posicao.x -= TextLength(texto) * 16;
    }

    for (int i = 0; texto[i] != '\0'; i++) {

        if (texto[i] == ' ') {
            DrawTextureRec( rm.texturaHud, (Rectangle){ 92, 178, 5, 13 }, (Vector2){ posicao.x + 6, posicao.y + 8 }, WHITE ); //Dois Pontos do Tempo
            posicao.x += 16;
            continue;
        }
        
        int digito = texto[i] - '0';

        int spriteX = (digito * (16 + 2)) + 1;
        
        DrawTextureRec( rm.texturaHud, (Rectangle){ spriteX, 138, 19, 31 }, posicao, WHITE );
        
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
            spriteX = 1 + (digito * (16 + 2));
            spriteY = 138;
        } else {
            spriteX = 2 + ((digito - 5) * (16 + 2));
            spriteY = 190;
        }

        DrawTextureRec( rm.texturaHud, (Rectangle){ spriteX, spriteY, 18, 25 }, posicao, WHITE );
        
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

    int deslocamentoParallax = (int) ( ( gw->camera.target.x / (float) larguraMapa ) * -800 );
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

    c->offset.x = LARGURA_VIRTUAL / 2;
    c->offset.y = ALTURA_VIRTUAL / 2;

    // O target é arredondado para o inteiro mais próximo para garantir que a
    // translação da câmera ocorra sempre em posições inteiras de pixel. Sem esse
    // arredondamento, o valor float contínuo de ret.x faz os tiles serem
    // renderizados em posições subpixel, causando frestas visíveis entre eles.
    c->target.x = roundf( j->ret.x + j->ret.width / 2.0f );
    c->target.y = roundf( j->ret.y + j->ret.height / 2.0f );

    int minX = LARGURA_VIRTUAL / 2;
    int maxX = calcularLarguraMapa( gw->mapa ) - LARGURA_VIRTUAL / 2;
    int maxY = calcularAlturaMapa( gw->mapa ) - ALTURA_VIRTUAL / 2;

    if ( c->target.x < minX ) {
        c->target.x = minX;
    } else if ( c->target.x > maxX ) {
        c->target.x = maxX;
    }

    if ( c->target.y > maxY ) {
        c->target.y = maxY;
    }

    if (cameraShakeDuracao > 0.0f) {
        c->offset.x += GetRandomValue(-1, 1) * cameraShakeIntensidade;
        c->offset.y += GetRandomValue(-1, 1) * cameraShakeIntensidade;
        cameraShakeDuracao -= GetFrameTime(); 
    }

}

static void DesenharPopup(Texture2D texture, Rectangle source, Vector2 position, Color tint) {

    int shakeX = GetRandomValue(-1, 1);
    int shakeY = GetRandomValue(-1, 1);

    DrawTextureRec(texture, source, (Vector2){ position.x + shakeX, position.y + shakeY }, tint);
}

static void DesenharTelaMorte(Texture2D texture, Rectangle source, Vector2 position, Color tint) {
    float frameHeight = 450.0f;

    int numFrames = 13;
    float frameWidth = texture.width / numFrames;
    
    int frameDurations[] = {1, 1, 1, 6, 5, 24, 14, 8, 5, 9, 15, 4, 80};
    float frameDuration = 0.03f;

    float tempoDecorrido = GetTime() - tempoInicioAnimacaoMorte;
    int frameUnits = (int)(tempoDecorrido / frameDuration);

    int totalDuration = 0;
    for (int i = 0; i < numFrames; i++) {
        totalDuration += frameDurations[i];
    }

    int posicaoCiclo = frameUnits % totalDuration;
    
    int frameIndex = 0;
    int accumulatedDuration = 0;
    for (int i = 0; i < numFrames; i++) {
        accumulatedDuration += frameDurations[i];
        if (posicaoCiclo < accumulatedDuration) {
            frameIndex = i;
            break;
        }
    }
    
    Rectangle frameSource = {
        frameIndex * frameWidth,
        0,
        frameWidth,
        frameHeight
    };
    
    DrawTexturePro(
        texture,
        frameSource,
        (Rectangle) {
            position.x,
            position.y,
            frameWidth,
            frameHeight
        },
        (Vector2) { 0 },
        0.0f,
        tint
    );
}

static void DesenharTitulo(Texture2D texture, Rectangle source, Vector2 position, Color tint) {
    
    float tempo = GetTime() * 3.0f; 
    float escala = 1.0f + sinf(tempo) * 0.03f;

    Rectangle dest = {
        position.x,
        position.y,
        source.width * escala,
        source.height * escala
    };

    Vector2 origin = { 0.0f, 0.0f };

    dest.x -= (dest.width - source.width) / 2.0f;
    dest.y -= (dest.height - source.height) / 2.0f;

    DrawTexturePro(texture, source, dest, origin, 0.0f, tint);
}

static void inicializar( GameWorld *gw ) {

    switch ( estadoJogoAtual ) {
        case ESTADO_JOGO_MENU_PAUSA:
            return;
            break;
        case ESTADO_JOGO_MAPA1:
            gw->mapa = carregarMapa( "resources/mapas/mapa01.txt" );
            gw->jogador = criarJogador( 50, 450, 96, 96 );

            if (tempBits > 0){
                gw->jogador->quantidadeBits = tempBits;
            }

            break;
        case ESTADO_JOGO_MAPA2:
            gw->mapa = carregarMapa( "resources/mapas/mapa02.txt" );
            gw->jogador = criarJogador( 194, 144, 96, 96 );
            SetMusicVolume( rm.musicaFase01, 0 );

            if (tempBits > 0){
                gw->jogador->quantidadeBits = tempBits;
            }

            break;
        case ESTADO_JOGO_MAPA3:
            gw->mapa = carregarMapa( "resources/mapas/mapa03.txt" );
            gw->jogador = criarJogador( GetScreenWidth() / 2 + 144, calcularAlturaMapa( gw->mapa ) - 1000, 96, 96 );
            break;
    }
    

    gw->camera = (Camera2D) {
        .offset = { 0 },    // deslocamento relativo da câmera em relação ao alvo
        .target = { 0 },    // o alvo da câmera, ou seja, a coordenada em que ela está centralizada
        .rotation = 0.0f,   // rotação da câmera em graus. o pivô é o alvo.
        .zoom = 0.90f        // zoom da câmera. 1.0f significa sem escala
    };

    gw->gravidade = 1200;

    if (gw->mapa != NULL){
        printf("Altura: %d\n", calcularAlturaMapa( gw->mapa ) );
    }
    

}

void ativarCameraShake(float duracao, float intensidade) {
    cameraShakeDuracao    = duracao;
    cameraShakeIntensidade = intensidade;
}

static void reiniciar( GameWorld *gw ) {

    if( gw->mapa != NULL ){
        destruirMapa( gw->mapa );
    }
    
    if( gw->jogador != NULL){
        destruirJogador( gw->jogador );
    }
    

    if ( IsMusicStreamPlaying( rm.musicaFase01 ) ) {
        StopMusicStream( rm.musicaFase01 );
    }

    if ( IsMusicStreamPlaying( rm.musicaFase02 ) ) {
        StopMusicStream( rm.musicaFase02 );
    }

    inicializar( gw );

}

void alterarEstadoJogo(EstadoJogo novoEstado ){
    estadoJogoAtual = novoEstado;
    mudarFase = true;
}

static void verificarJogadorMorto( GameWorld *gw ) {
    if(gw->jogador->morreu) {
        tocarMusicaBoss = false;
        if(gw->jogador->quantidadeVidas > 0){
            gw->jogador->quantidadeVidas--;
            int vidaPosMorte = gw->jogador->quantidadeVidas;
            reiniciar(gw);
            gw->jogador->quantidadeVidas = vidaPosMorte;
        }else{
            cronometro = 0;
            tempoInicioAnimacaoMorte = GetTime();
            tempBits = 0;
            alterarEstadoJogo(ESTADO_JOGO_DERROTA);
        }
        
    }
}
