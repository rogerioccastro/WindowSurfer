/**
 * @file Macros.h
 * @author @EddiePricefield & @rogerioccastro
 * @brief Macros utilizados no jogo.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

/**
 * @brief Macro que empacota a função TraceLog, gerando uma chamada a TraceLog com o nível LOG_INFO
 */
#define trace( ... ) TraceLog( LOG_INFO, __VA_ARGS__ );