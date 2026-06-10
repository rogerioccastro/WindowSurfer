/**
 * @file InimigoAdware.h
 * @author @EddiePricefield & @rogerioccastro
 * @brief Declarações das funções do Inimigo (Adware).
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo inimigo (adware).
 */
InimigoAdware *criarInimigoAdware( Rectangle ret, Color cor );

/**
 * @brief Destroi um inimigo (adware).
 */
void destruirInimigoAdware( InimigoAdware *inimigo );

/**
 * @brief Atualiza um inimigo (adware).
 */
void atualizarInimigoAdware( InimigoAdware *inimigo, GameWorld *gw,float delta );

/**
 * @brief Desenha um inimigo (adware).
 */
void desenharInimigoAdware( InimigoAdware *inimigo );

/**
 * @brief Obtém o quadro de animação atual de um inimigo (adware).
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoAdware( InimigoAdware *inimigo );