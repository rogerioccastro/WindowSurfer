/**
 * @file ResourceManager.c
 * @author @EddiePricefield & @rogerioccastro
 * @brief ResourceManager implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "ResourceManager.h"
#include "Utils.h"

ResourceManager rm = { 0 };

void loadResourcesResourceManager( void ) {

    rm.texturaJogador = LoadTexture( "resources/imagens/sprites/run/thickRunSheet.png" );

    rm.texturaJogadorParado = LoadTexture( "resources/imagens/sprites/idle/thickIdleSheet.png" );

    rm.texturaInimigos = carregarTexturaAlterandoCores( 
        "resources/imagens/sprites/spritesheet-virus.png",
        (Color[]) {
            { 64, 64, 127, 255 },
        },
        (Color[]) {
            BLANK,
        },
        1
    );

    rm.texturaMorte = carregarTexturaAlterandoCores( 
        "resources/imagens/sprites/badniks.png",
        (Color[]) {
            { 13, 72, 7, 255 },
        },
        (Color[]) {
            BLANK,
        },
        1
    );

    rm.texturaItens = carregarTexturaAlterandoCores( 
        "resources/imagens/itens/itens.png",
        (Color[]) {
            { 62, 62, 130, 255 },
        },
        (Color[]) {
            BLANK,
        },
        1
    );

    rm.texturaHud = carregarTexturaAlterandoCores(
        "resources/imagens/hud/hud.png",
        (Color[]) {
            { 0, 102, 153, 255 },
        },
        (Color[]) {
            BLANK,
        },
        1
    );

    rm.texturaFundo = carregarTexturaAlterandoCores(
        "resources/imagens/fundo/montanhas.png",
        (Color[]) {
            { 255, 255, 255, 255 },
        },
        (Color[]) {
            BLANK,
        },
        1
    );


    rm.texturaTerreno = LoadTexture( "resources/imagens/tiles/terreno.png" );
    rm.texturaCeu = LoadTexture( "resources/imagens/fundo/ceu.png" );

    rm.texturaJanela1 = LoadTexture("resources/imagens/janelas/janela1.png");
    rm.texturaJanela = LoadTexture("resources/imagens/tiles/janelatile.png"); 


    // Filtro de textura nearest-neighbor (ponto) para todas as texturas do jogo.
    // Evita interpolação bilinear nas bordas dos tiles e sprites, que causaria
    // bleeding de cor entre pixels adjacentes — efeito indesejável em pixel art.
    SetTextureFilter( rm.texturaJogador, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaJogadorParado, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaInimigos, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaItens, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaTerreno, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaFundo, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaCeu, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaHud, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaJanela1, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaJanela, TEXTURE_FILTER_POINT );

    rm.somColeta = LoadSound( "resources/sons/efeitos/bit.wav" );
    rm.somFrenagem = LoadSound( "resources/sons/efeitos/frenagem.wav" );
    rm.somHit = LoadSound( "resources/sons/efeitos/somHit2.wav" );
    rm.somHitInimigo = LoadSound( "resources/sons/efeitos/hitInimigo.wav" );
    rm.somMorte = LoadSound( "resources/sons/efeitos/morte.wav" );
    rm.somPulo = LoadSound( "resources/sons/efeitos/pulo.wav" );

    rm.musicaFase01 = LoadMusicStream( "resources/sons/musicas/golf-central.mp3" );

    SetSoundVolume( rm.somColeta, 0.8f );
    SetSoundVolume( rm.somFrenagem, 0.8f );
    SetSoundVolume( rm.somHit, 1.2f );
    SetSoundVolume( rm.somHitInimigo, 1.3f );
    SetSoundVolume( rm.somPulo, 0.8f );
    
    SetMusicVolume( rm.musicaFase01, 0.0f );


    SetMasterVolume( 0.2f);

}

void unloadResourcesResourceManager( void ) {

    UnloadTexture( rm.texturaJogador );
    UnloadTexture( rm.texturaJogadorParado );
    UnloadTexture( rm.texturaInimigos );
    UnloadTexture( rm.texturaItens );
    UnloadTexture( rm.texturaTerreno );
    UnloadTexture( rm.texturaFundo );
    UnloadTexture( rm.texturaCeu );
    UnloadTexture( rm.texturaHud );
    UnloadTexture( rm.texturaJanela1 );
    UnloadTexture( rm.texturaJanela );
    UnloadSound( rm.somColeta );
    UnloadSound( rm.somFrenagem );
    UnloadSound( rm.somHit );
    UnloadSound( rm.somHitInimigo );
    UnloadSound( rm.somMorte );
    UnloadSound( rm.somPulo );

    UnloadMusicStream( rm.musicaFase01 );

}