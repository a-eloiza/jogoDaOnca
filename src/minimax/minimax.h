#ifndef __MINIMAX_H__
#define __MINIMAX_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jogo/jogo.h"

#define INFINITO 2000000
#define MAX_PROFUNDIDADE 6

int calcula_MIN (char *tabuleiro, int alpha, int beta, int profunidade);
int calcula_MAX (char *tabuleiro, int alpha, int beta, int profunidade);

jogada_t minimax (char *tabuleiro, char meu_lado);

#endif