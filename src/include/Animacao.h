/**
 * @file Animacao.h
 * @author @EddiePricefield & @rogerioccastro
 * @brief Declaração das funções para gerenciamento de animações.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Aloca o array de quadros de uma animação.
 */
void criarQuadrosAnimacao( Animacao *anim, int quantidadeQuadros );

/**
 * @brief Inicializa os quadros de uma animação a partir de uma spritesheet.
 * Os quadros são distribuídos horizontalmente a partir da posição inicial
 * informada, com espaçamento fixo entre eles. O retângulo de colisão padrão
 * é aplicado a todos os quadros.
 */
void inicializarQuadrosAnimacao(
    QuadroAnimacao *quadros,
    int quantidadeQuadros,
    int duracaoPadrao,
    int xInicioFonte,
    int yInicioFonte,
    int larguraFonte,
    int alturaFonte,
    int espacamentoFonte,
    bool deTrasParaFrente,
    Rectangle retColisaoPadrao
);

/**
 * @brief Libera o array de quadros de uma animação.
 */
void destruirQuadrosAnimacao( Animacao *anim );

/**
 * @brief Avança a animação com base no tempo decorrido. Respeita os modos
 * pararNoUltimoQuadro e executarUmaVez, marcando a animação como finalizada
 * quando apropriado.
 */
void atualizarAnimacao( Animacao *anim, float delta );

/**
 * @brief Retorna o índice do quadro atual da animação.
 */
int getNumeroQuadroAtualAnimacao( Animacao *anim );

/**
 * @brief Retorna um ponteiro para o quadro atual da animação,
 * ou NULL se a animação for inválida.
 */
QuadroAnimacao *getQuadroAtualAnimacao( Animacao *anim );

/**
 * @brief Reinicia a animação para o primeiro quadro, zerando o contador
 * de tempo e o flag de finalizada.
 */
void reiniciarAnimacao( Animacao *anim );

/**
 * @brief Sincroniza o quadro atual de duas animações, copiando o quadro
 * da origem para o destino. Só executa se ambas tiverem a mesma quantidade
 * de quadros; caso contrário, emite um aviso e retorna sem alteração.
 */
void sincronizarAnimacao( Animacao *destino, Animacao *origem );
