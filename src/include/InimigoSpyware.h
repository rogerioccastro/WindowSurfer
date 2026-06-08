/**
 * @file InimigoSpyware.h
 * @author @EddiePricefield & @rogerioccastro
 * @brief Declarações das funções do Inimigo (Spyware).
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo inimigo (spyware).
 */
InimigoSpyware *criarInimigoSpyware( Rectangle ret, Color cor );

/**
 * @brief Destroi um inimigo (spyware).
 */
void destruirInimigoSpyware( InimigoSpyware *inimigo );

/**
 * @brief Atualiza um inimigo (spyware).
 */
void atualizarInimigoSpyware( InimigoSpyware *inimigo, GameWorld *gw,float delta );

/**
 * @brief Desenha um inimigo (spyware).
 */
void desenharInimigoSpyware( InimigoSpyware *inimigo );

/**
 * @brief Obtém o quadro de animação atual de um inimigo (spyware).
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoSpyware( InimigoSpyware *inimigo );