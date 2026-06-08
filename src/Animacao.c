/**
 * @file Animacao.c
 * @author @EddiePricefield & @rogerioccastro
 * @brief Implementação das funções para gerenciamento de animações.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "Tipos.h"

/**
 * @brief Aloca o array de quadros de uma animação.
 */
void criarQuadrosAnimacao( Animacao *anim, int quantidadeQuadros ) {
    anim->quadros = (QuadroAnimacao*) malloc( sizeof( QuadroAnimacao ) * quantidadeQuadros );
}

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
    Rectangle retColisaoPadrao ) {

    int passo = abs( larguraFonte ) + espacamentoFonte;

    for ( int i = 0; i < quantidadeQuadros; i++ ) {

        int p = !deTrasParaFrente ? i : quantidadeQuadros - i - 1;

        quadros[i] = (QuadroAnimacao) {
            .fonte = {
                xInicioFonte + passo * p,
                yInicioFonte,
                larguraFonte,
                alturaFonte
            },
            .duracao = duracaoPadrao,
            .retColisao = retColisaoPadrao
        };

    }

}

/**
 * @brief Libera o array de quadros de uma animação.
 */
void destruirQuadrosAnimacao( Animacao *anim ) {
    if ( anim != NULL ) {
        free( anim->quadros );
    }
}

/**
 * @brief Avança a animação com base no tempo decorrido. Respeita os modos
 * pararNoUltimoQuadro e executarUmaVez, marcando a animação como finalizada
 * quando apropriado.
 */
void atualizarAnimacao( Animacao *anim, float delta ) {

    if ( anim->finalizada ) {
        return;
    }

    anim->contadorTempoQuadro += (int) ( delta * 1000 );

    QuadroAnimacao *quadro = getQuadroAtualAnimacao( anim );

    if ( anim->contadorTempoQuadro >= quadro->duracao ) {

        anim->contadorTempoQuadro = 0;
        anim->quadroAtual++;

        if ( anim->pararNoUltimoQuadro && anim->quadroAtual >= anim->quantidadeQuadros ) {
            anim->quadroAtual = anim->quantidadeQuadros - 1;
            anim->finalizada = true;
            return;
        }

        if ( anim->executarUmaVez && anim->quadroAtual >= anim->quantidadeQuadros ) {
            anim->quadroAtual = 0;
            anim->finalizada = true;
            return;
        }

        anim->quadroAtual %= anim->quantidadeQuadros;

    }

}

/**
 * @brief Retorna o índice do quadro atual da animação.
 */
int getNumeroQuadroAtualAnimacao( Animacao *anim ) {
    return anim->quadroAtual % anim->quantidadeQuadros;
}

/**
 * @brief Retorna um ponteiro para o quadro atual da animação,
 * ou NULL se a animação for inválida.
 */
QuadroAnimacao *getQuadroAtualAnimacao( Animacao *anim ) {
    if ( anim != NULL ) {
        return &anim->quadros[getNumeroQuadroAtualAnimacao(anim)];
    }
    return NULL;
}

/**
 * @brief Reinicia a animação para o primeiro quadro, zerando o contador
 * de tempo e o flag de finalizada.
 */
void reiniciarAnimacao( Animacao *anim ) {
    anim->quadroAtual = 0;
    anim->contadorTempoQuadro = 0;
    anim->finalizada = false;
}

/**
 * @brief Sincroniza o quadro atual de duas animações, copiando o quadro
 * da origem para o destino. Só executa se ambas tiverem a mesma quantidade
 * de quadros; caso contrário, emite um aviso e retorna sem alteração.
 */
void sincronizarAnimacao( Animacao *destino, Animacao *origem ) {
    if ( destino->quantidadeQuadros != origem->quantidadeQuadros ) {
        TraceLog( 
            LOG_WARNING, "sincronizarAnimacao: animações com quantidade de quadros diferente (%d != %d)",
            destino->quantidadeQuadros, origem->quantidadeQuadros
        );
        return;
    }
    destino->quadroAtual = origem->quadroAtual;
    destino->contadorTempoQuadro = 0.0f;
}
