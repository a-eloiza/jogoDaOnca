#ifndef __HEURISTICA_H__
#define __HEURISTICA_H__

#include <stdlib.h>
#include <math.h>
#include "../jogo/jogo.h"

#define VITORIA_ONCA  999999
#define VITORIA_CAES -999999
#define PESO_CAPTURA    1000
#define PESO_MOBILIDADE   50
#define PESO_CERCO         1

int encontrar_peca (char *tabuleiro, char peca, int *linha, int *coluna);

int contar_caes (char *tabuleiro);

int calcular_cerco (char *tabuleiro);

int calcular_mobilidade_onca (char *tabuleiro);

int heuristica (char *tabuleiro);

#endif