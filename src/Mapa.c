/**
 * @file Mapa.c
 * @author @EddiePricefield & @rogerioccastro
 * @brief Implementação do Mapa.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Macros.h"
#include "Mapa.h"
#include "Inimigo.h"
#include "InimigoMalware.h"
#include "InimigoSpyware.h"
#include "InimigoAdware.h"
#include "Item.h"
#include "ItemBit.h"
#include "ItemByte.h"
#include "ItemAtalho.h"
#include "ItemDefender.h"
#include "Obstaculo.h"
#include "BlocoInvisivel.h"
#include "Tipos.h"
#include "ResourceManager.h"

static void inserirObstaculo(Mapa *mapa, ElementoMapa *obstaculo);
static void inserirBlocoInvisivel(Mapa *mapa, ElementoMapa *blocoInvis);
static void inserirItem(Mapa *mapa, ElementoMapa *item);
static void inserirInimigo(Mapa *mapa, ElementoMapa *inimigo);

/**
 * @brief Carrega um mapa a partir de uma arquivo.
 */
Mapa *carregarMapa(const char *caminhoArquivo)
{

    // aloca um novo mapa
    Mapa *novoMapa = (Mapa *)malloc(sizeof(Mapa));

    novoMapa->blocoInvis = NULL;
    novoMapa->quantidadeBlocoInvis = 0;

    novoMapa->obstaculos = NULL;
    novoMapa->quantidadeObstaculos = 0;

    novoMapa->itens = NULL;
    novoMapa->quantidadeItens = 0;

    novoMapa->inimigos = NULL;
    novoMapa->quantidadeInimigos = 0;

    novoMapa->dimensaoPadraoElementos = 48;
    novoMapa->linhas = 0;
    novoMapa->colunas = 0;

    // carrega dados do arquivo de texto
    char *dadosMapa = LoadFileText(caminhoArquivo);

    // marcadores para processamento do mapa
    char *caractereAtual = dadosMapa;
    int linhaAtual = 0;
    int colunaAtual = 0;

    // caractere atual marca inicialmente a primeira posição de dadosMapa
    // C-strings terminam em '\0', sendo assim, caminhamos caractere por
    // caractere até o fim
    while (*caractereAtual != '\0')
    {

        char c = *caractereAtual;

        // fim de linha?
        if (c == '\n')
        {

            linhaAtual++;
            colunaAtual = 0;

            novoMapa->linhas = linhaAtual;
        }
        else
        {

            if (c != ' ')
            {

                ElementoMapa *el = (ElementoMapa *)malloc(sizeof(ElementoMapa));
                el->proximo = NULL;

                if (c >= 'A' && c <= 'Z')
                {

                    int deslocamento = c - 'A';
                    Texture2D *texturaUsada = &rm.texturaTerreno;
                    int tamanhoDaTile = novoMapa->dimensaoPadraoElementos; // 48x48
                    int espacamento = 1;

                    // Seleciona a textura baseado na letra
                    switch (c)
                    {
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'E':
                    case 'F':
                    case 'G':
                        // Usa texturaTerreno (padrão) - 48x48 com espaçamento
                        texturaUsada = &rm.texturaTerreno;
                        deslocamento = c - 'A';
                        tamanhoDaTile = 48;
                        espacamento = 1;
                        break;
                    
                    case 'H':
                    case 'I':
                    case 'J':
                    case 'K':
                    case 'L':
                    case 'M':
                    case 'N':
                    case 'O':
                    case 'P':
                    case 'Q':
                    case 'R':
                    case 'S':
                    case 'T':
                    case 'U':
                    case 'V':
                    case 'W':
                    case 'X':
                    case 'Y':
                    case 'Z':
                        // Usa texturaNovasTiles - 16x16 sem espaçamento
                        texturaUsada = &rm.texturaJanela;
                        deslocamento = c - 'H';
                        tamanhoDaTile = 16;
                        espacamento = 0;
                        break;

                    default:
                        break;
                    }

                    el->objeto = criarObstaculo(
                        (Rectangle){
                            .x = novoMapa->dimensaoPadraoElementos * colunaAtual,
                            .y = novoMapa->dimensaoPadraoElementos * linhaAtual,
                            .width = novoMapa->dimensaoPadraoElementos,
                            .height = novoMapa->dimensaoPadraoElementos},
                        GRAY,
                        (Rectangle){
                            0 + (tamanhoDaTile + espacamento) * deslocamento,
                            0,
                            tamanhoDaTile,
                            tamanhoDaTile},
                        texturaUsada);

                    el->tipo = TIPO_ELEMENTO_MAPA_OBSTACULO;

                    inserirObstaculo(novoMapa, el);
                }

                else if (c >= 'a' && c <= 'z')
                {

                    Item *item = NULL;

                    switch (c)
                    {

                    case 'a':

                        item = criarItem(TIPO_ITEM_BIT);

                        item->objeto = criarItemBit(
                            (Rectangle){
                                .x = novoMapa->dimensaoPadraoElementos * colunaAtual + 8,
                                .y = novoMapa->dimensaoPadraoElementos * linhaAtual + 5,
                                .width = 32,
                                .height = 32},
                            YELLOW);

                        el->objeto = item;
                        el->tipo = TIPO_ELEMENTO_MAPA_ITEM;

                        break;

                    case 'b':

                        item = criarItem(TIPO_ITEM_BYTE);

                        item->objeto = criarItemByte(
                            (Rectangle){
                                .x = novoMapa->dimensaoPadraoElementos * colunaAtual + 8,
                                .y = novoMapa->dimensaoPadraoElementos * linhaAtual + 5,
                                .width = 32,
                                .height = 32},
                            YELLOW);

                        el->objeto = item;
                        el->tipo = TIPO_ELEMENTO_MAPA_ITEM;

                        break;
                    
                    case 'c':

                        item = criarItem(TIPO_ITEM_ATALHO);

                        item->objeto = criarItemAtalho(
                            (Rectangle){
                                .x = novoMapa->dimensaoPadraoElementos * colunaAtual + 8,
                                .y = novoMapa->dimensaoPadraoElementos * linhaAtual - 12,
                                .width = 62,
                                .height = 54},
                            YELLOW);

                        el->objeto = item;
                        el->tipo = TIPO_ELEMENTO_MAPA_ITEM;

                        break;
                    
                    case 'd':

                        item = criarItem(TIPO_ITEM_DEFENDER);

                        item->objeto = criarItemDefender(
                            (Rectangle){
                                .x = novoMapa->dimensaoPadraoElementos * colunaAtual + 8,
                                .y = novoMapa->dimensaoPadraoElementos * linhaAtual - 12,
                                .width = 40,
                                .height = 46},
                            YELLOW);

                        el->objeto = item;
                        el->tipo = TIPO_ELEMENTO_MAPA_ITEM;

                        break;

                    default:
                        TraceLog(LOG_ERROR, "Tipo de item desconhecido.");
                        abort();
                        break;
                    }

                    inserirItem(novoMapa, el);
                }
                else if (c >= '0' && c <= '9')
                {

                    Inimigo *inimigo = NULL;

                    switch (c)
                    {

                    case '0':

                        inimigo = criarInimigo(TIPO_INIMIGO_MALWARE);

                        inimigo->objeto = criarInimigoMalware(
                            (Rectangle){
                                .x = novoMapa->dimensaoPadraoElementos * colunaAtual,
                                .y = novoMapa->dimensaoPadraoElementos * linhaAtual - 12,
                                .width = 80,
                                .height = 60},
                            YELLOW);

                        el->objeto = inimigo;
                        el->tipo = TIPO_ELEMENTO_MAPA_INIMIGO;

                        break;

                    case '1':

                        inimigo = criarInimigo(TIPO_INIMIGO_SPYWARE);

                        inimigo->objeto = criarInimigoSpyware(
                            (Rectangle){
                                .x = novoMapa->dimensaoPadraoElementos * colunaAtual,
                                .y = novoMapa->dimensaoPadraoElementos * linhaAtual - 21,
                                .width = 80,
                                .height = 60},
                            BLUE);

                        el->objeto = inimigo;
                        el->tipo = TIPO_ELEMENTO_MAPA_INIMIGO;

                        break;

                    case '2':

                        inimigo = criarInimigo(TIPO_INIMIGO_ADWARE);

                        inimigo->objeto = criarInimigoAdware(
                            (Rectangle){
                                .x = novoMapa->dimensaoPadraoElementos * colunaAtual,
                                .y = novoMapa->dimensaoPadraoElementos * linhaAtual - 21,
                                .width = 80,
                                .height = 60},
                            YELLOW);

                        el->objeto = inimigo;
                        el->tipo = TIPO_ELEMENTO_MAPA_INIMIGO;

                        break;

                    default:
                        TraceLog(LOG_ERROR, "Tipo de inimigo desconhecido.");
                        abort();
                        break;
                    }

                    inserirInimigo(novoMapa, el);
                }
                else if (c == '#'){

                    int tamanhoDaTile = novoMapa->dimensaoPadraoElementos; // 48x48

                    el->objeto = criarBlocoInvisivel(
                        (Rectangle){
                            .x = novoMapa->dimensaoPadraoElementos * colunaAtual,
                            .y = novoMapa->dimensaoPadraoElementos * linhaAtual,
                            .width = novoMapa->dimensaoPadraoElementos,
                            .height = novoMapa->dimensaoPadraoElementos});

                    el->tipo = TIPO_ELEMENTO_MAPA_BLOCO_INVISIVEL;

                    inserirBlocoInvisivel(novoMapa, el);

                }
                else
                {
                    TraceLog(LOG_ERROR, "Entidade inválida no mapa.");
                    abort();
                }
            }

            colunaAtual++;

            if (novoMapa->colunas < colunaAtual)
            {
                novoMapa->colunas = colunaAtual;
            }
        }

        caractereAtual++;
    }

    novoMapa->linhas++;

    // descarrega os dados
    UnloadFileText(dadosMapa);

    return novoMapa;
}

/**
 * @brief Destroi um mapa.
 */
void destruirMapa(Mapa *m)
{

    if (m != NULL)
    {

        ElementoMapa *el = NULL;

        el = m->obstaculos;
        while (el != NULL)
        {
            destruirObstaculo((Obstaculo *)el->objeto);
            ElementoMapa *t = el;
            el = el->proximo;
            free(t);
        }

        el = m->itens;
        while (el != NULL)
        {
            destruirItem((Item *)el->objeto);
            ElementoMapa *t = el;
            el = el->proximo;
            free(t);
        }

        el = m->inimigos;
        while (el != NULL)
        {
            destruirInimigo((Inimigo *)el->objeto);
            ElementoMapa *t = el;
            el = el->proximo;
            free(t);
        }

        el = m->blocoInvis;
        while (el != NULL)
        {
            destruirBlocoInvisivel((BlocoInvisivel *)el->objeto);
            ElementoMapa *t = el;
            el = el->proximo;
            free(t);
        }
    }
}

/**
 * @brief Atualiza um mapa.
 */
void atualizarMapa(Mapa *m, GameWorld *gw, float delta)
{

    ElementoMapa *el = NULL;

    el = m->itens;
    while (el != NULL)
    {
        atualizarItem((Item *)el->objeto, delta);
        el = el->proximo;
    }

    el = m->inimigos;
    while (el != NULL)
    {
        atualizarInimigo((Inimigo *)el->objeto, gw, delta);
        el = el->proximo;
    }
}

/**
 * @brief Desenha um mapa.
 */
void desenharMapa(Mapa *m)
{

    ElementoMapa *el = NULL;

    el = m->obstaculos;
    while (el != NULL)
    {
        desenharObstaculo((Obstaculo *)el->objeto);
        el = el->proximo;
    }

    el = m->itens;
    while (el != NULL)
    {
        desenharItem((Item *)el->objeto);
        el = el->proximo;
    }

    el = m->inimigos;
    while (el != NULL)
    {
        desenharInimigo((Inimigo *)el->objeto);
        el = el->proximo;
    }

    el = m->blocoInvis;
    while (el != NULL)
    {
        desenharBlocoInvisivel((BlocoInvisivel *)el->objeto);
        el = el->proximo;
    }
}

/**
 * @brief Calcula a largura do mapa.
 */
int calcularLarguraMapa(Mapa *m)
{
    return (int)(m->dimensaoPadraoElementos * m->colunas);
}

/**
 * @brief Calcula a altura do mapa.
 */
int calcularAlturaMapa(Mapa *m)
{
    return (int)(m->dimensaoPadraoElementos * m->linhas);
}

/**
 * @brief Insere um obstáculo na lista de obstáculos.
 */
static void inserirObstaculo(Mapa *mapa, ElementoMapa *obstaculo)
{
    if (mapa->obstaculos == NULL)
    {
        mapa->obstaculos = obstaculo;
    }
    else
    {
        obstaculo->proximo = mapa->obstaculos;
        mapa->obstaculos = obstaculo;
    }
    mapa->quantidadeObstaculos++;
}

/**
 * @brief Insere um bloco invisível.
 */
static void inserirBlocoInvisivel(Mapa *mapa, ElementoMapa *blocoInvis)
{
    if (mapa->blocoInvis == NULL)
    {
        mapa->blocoInvis = blocoInvis;
    }
    else
    {
        blocoInvis->proximo = mapa->blocoInvis;
        mapa->blocoInvis = blocoInvis;
    }
    mapa->quantidadeBlocoInvis++;
}

/**
 * @brief Insere um item na lista de itens.
 */
static void inserirItem(Mapa *mapa, ElementoMapa *item)
{
    if (mapa->itens == NULL)
    {
        mapa->itens = item;
    }
    else
    {
        item->proximo = mapa->itens;
        mapa->itens = item;
    }
    mapa->quantidadeItens++;
}

/**
 * @brief Insere um inimigo na lista de inimigos.
 */
static void inserirInimigo(Mapa *mapa, ElementoMapa *inimigo)
{
    if (mapa->inimigos == NULL)
    {
        mapa->inimigos = inimigo;
    }
    else
    {
        inimigo->proximo = mapa->inimigos;
        mapa->inimigos = inimigo;
    }
    mapa->quantidadeInimigos++;
}