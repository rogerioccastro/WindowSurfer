/**
 * @file Tipos.h
 * @author @EddiePricefield & @rogerioccastro
 * @brief Definição dos tipos utilizados no jogo.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include <stdbool.h>

#include "raylib/raylib.h"

/**
 * @brief Representa o estado do jogador.
 */
typedef enum EstadoJogador {
    ESTADO_JOGADOR_PARADO,
    ESTADO_JOGADOR_ANDANDO,
    ESTADO_JOGADOR_ANDANDO_RAPIDO,
    ESTADO_JOGADOR_CORRENDO,
    ESTADO_JOGADOR_PULANDO,
    ESTADO_JOGADOR_PULANDO_RAPIDO,
    ESTADO_JOGADOR_PULANDO_CORRENDO,
    ESTADO_JOGADOR_MORTO,
} EstadoJogador;

/**
 * @brief Representa o estado do inimigo do tipo Motobug.
 */
typedef enum EstadoInimigoMalware {
    ESTADO_INIMIGO_MALWARE_ANDANDO,
    ESTADO_INIMIGO_MALWARE_MORRENDO,
} EstadoInimigoMalware;

typedef enum EstadoInimigoSpyware {
    ESTADO_INIMIGO_SPYWARE_ANDANDO,
    ESTADO_INIMIGO_SPYWARE_MORRENDO,
} EstadoInimigoSpyware;

/**
 * @brief Representa o tipo de um inimigo.
 */
typedef enum TipoInimigo {
    TIPO_INIMIGO_MALWARE,
    TIPO_INIMIGO_SPYWARE,
} TipoInimigo;

/**
 * @brief Representa o estado do item do tipo bit.
 */
typedef enum EstadoItemBit {
    ESTADO_ITEM_BIT_PARADO,
    ESTADO_ITEM_BIT_COLETADO,
} EstadoItemBit;

/**
 * @brief Representa o estado do item do tipo byte.
 */
typedef enum EstadoItemByte {
    ESTADO_ITEM_BYTE_PARADO,
    ESTADO_ITEM_BYTE_COLETADO,
} EstadoItemByte;

/**
 * @brief Representa o tipo de um item.
 */
typedef enum TipoItem {
    TIPO_ITEM_BIT,
    TIPO_ITEM_BYTE,
} TipoItem;

/**
 * @brief Representa o tipo de um elemento do mapa
 */
typedef enum TipoElementoMapa {
    TIPO_ELEMENTO_MAPA_OBSTACULO,
    TIPO_ELEMENTO_MAPA_ITEM,
    TIPO_ELEMENTO_MAPA_INIMIGO,
} TipoElementoMapa;

/**
 * @brief Representa o estado atual do jogo
 */
typedef enum EstadoJogo {
    ESTADO_JOGO_NADA,
    ESTADO_JOGO_MAPA1,
    ESTADO_JOGO_MAPA2,
} EstadoJogo;

/**
 * @brief Representa um quadro de animação com imagem.
 */
typedef struct QuadroAnimacao {
    Rectangle fonte;
    int duracao;          // milisegundos
    Rectangle retColisao; // deve ser interpretado como posicionamento relativo
} QuadroAnimacao;

/**
 * @brief Representa uma animação.
 */
typedef struct Animacao {
    QuadroAnimacao *quadros;
    int quantidadeQuadros;
    int quadroAtual;
    int contadorTempoQuadro;
    bool pararNoUltimoQuadro;
    bool executarUmaVez;
    bool finalizada;
} Animacao;

/**
 * @brief Representa o jogador controlado pelo usuário.
 */
typedef struct Jogador {

    Rectangle ret;
    Vector2 vel;
    Color cor;

    float velAndando;
    float velAndandoRapido;
    float velCorrendo;
    float velPulo;
    float velMaxQueda;

    float aceleracao;
    float desaceleracao;
    float frenagem;

    int quantidadePulos;
    int quantidadeMaxPulos;

    int quantidadeAneis;
    int quantidadeVidas;
    int quantidadeHP;
    int quantidadeMaxHP;
    int scoreTotal;

    bool invulneravel;
    float tempoInvulnerabilidade;
    float contadorTempoInvulnerabilidade;

    bool piscaPisca;
    float tempoPiscaPisca;
    float contadorTempoPiscaPisca;

    bool freando;

    bool noChao;
    float coyoteTime;
    float contadorCoyoteTime;

    EstadoJogador estado;
    bool olhandoParaDireita;

    bool morreu;

    Animacao *animacoes[20];
    int quantidadeAnimacoes;

    Animacao animacaoParado;
    Animacao animacaoAndando;
    Animacao animacaoAndandoRapido;
    Animacao animacaoCorrendo;
    Animacao animacaoPulando;
    Animacao animacaoPulandoRapido;
    Animacao animacaoPulandoCorrendo;

} Jogador;

/**
 * @brief Representa um inimigo do tipo Motobug.
 */
typedef struct InimigoMalware {

    Rectangle ret;
    Vector2 vel;
    Color cor;

    float velAndando;
    float velMaxQueda;

    EstadoInimigoMalware estado;
    bool ativo;
    bool olhandoParaDireita;     // *cuidado! a reflexão dos inimigos é ao contrário
                                 // do jogador! eles começam olhando para a esquerda
                                 // e as sprites são orientadas para a esquerda inicialmente
    Animacao *animacoes[2];
    int quantidadeAnimacoes;

    Animacao animacaoAndando;
    Animacao animacaoMorrendo;

} InimigoMalware;

typedef struct InimigoSpyware {

    Rectangle ret;
    Vector2 vel;
    Color cor;

    float velAndando;
    float velMaxQueda;

    EstadoInimigoSpyware estado;
    bool ativo;
    bool olhandoParaDireita;

    Animacao *animacoes[2];
    int quantidadeAnimacoes;

    Animacao animacaoAndando;
    Animacao animacaoMorrendo;

} InimigoSpyware;

/**
 * @brief Representa um inimigo.
 * O inimigo de fato é endereçado via membro "objeto".
 */
typedef struct Inimigo {
    void *objeto;
    TipoInimigo tipo;
} Inimigo;

/**
 * @brief Representa um item do tipo bit.
 */
typedef struct ItemBit {

    Rectangle ret;
    Color cor;

    EstadoItemBit estado;
    bool ativo;

    Animacao *animacoes[2];
    int quantidadeAnimacoes;

    Animacao animacaoParado;
    Animacao animacaoColetando;

} ItemBit;

/**
 * @brief Representa um item do tipo byte.
 */
typedef struct ItemByte {

    Rectangle ret;
    Color cor;

    EstadoItemByte estado;
    bool ativo;

    Animacao *animacoes[2];
    int quantidadeAnimacoes;

    Animacao animacaoParado;
    Animacao animacaoColetando;

} ItemByte;

/**
 * @brief Representa um item estático do mapa.
 * O item de fato é endereçado via membro "objeto".
 */
typedef struct Item {
    void *objeto;
    TipoItem tipo;
} Item;

/**
 * @brief Representa um obstáculo estático do mapa.
 */
typedef struct Obstaculo {
    Rectangle ret;
    Color cor;
    Rectangle fonte;
    Texture2D *textura;
} Obstaculo;

/**
 * @brief Representa um elemento do mapa.
 * O elemento de fato é endereçado via membro "objeto".
 */
typedef struct ElementoMapa ElementoMapa;
struct ElementoMapa {
    void *objeto;
    TipoElementoMapa tipo;
    ElementoMapa *proximo;
};

/**
 * @brief Representa um mapa de fase do jogo.
 */
typedef struct Mapa {

    // listas ligadas de elementos do mapa
    ElementoMapa *obstaculos; // marca o fim da lista
    int quantidadeObstaculos;

    ElementoMapa *itens;      // marca o fim da lista
    int quantidadeItens;

    ElementoMapa *inimigos;   // marca o fim da lista
    int quantidadeInimigos;

    float dimensaoPadraoElementos;
    int linhas;
    int colunas;

} Mapa;

/**
 * @brief Representa o mundo do jogo e seus elementos.
 */
typedef struct GameWorld {

    Mapa *mapa;
    Jogador *jogador;

    Camera2D camera;

    float gravidade;

} GameWorld;