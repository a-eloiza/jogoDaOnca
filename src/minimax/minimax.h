#ifndef __MINIMAX_H__
#define __MINIMAX_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../heuristica/heuristica.h"

#define INFINITO 2000000
#define MAX_PROFUNDIDADE 6
#define MAX_MOVIMENTOS_POSSIVEIS 200

int calcula_MIN (char *tabuleiro, int alpha, int beta, int profunidade);
int calcula_MAX (char *tabuleiro, int alpha, int beta, int profunidade);

jogada_t minimax (char *tabuleiro, char meu_lado);

#endif