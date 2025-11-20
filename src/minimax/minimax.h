#ifndef __MINIMAX_H__
#define __MINIMAX_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heuristica/heuristica.h"

#define MAXINT 16
#define INFINITO 2000000
#define MAX_PROFUNDIDADE 6
#define MAX_MOVIMENTOS_POSSIVEIS 200
#define TAMANHO_BUFFER_TABULEIRO 512

typedef struct {
    char tipo;
    int num_mov;
    int linhas[MAXINT];
    int colunas[MAXINT];
    int valor;           
} jogada_t;

int calcula_MIN (char *tabuleiro, int alpha, int beta, int profunidade);
int calcula_MAX (char *tabuleiro, int alpha, int beta, int profunidade);

jogada_t minimax (char *tabuleiro, char meu_lado);

#endif