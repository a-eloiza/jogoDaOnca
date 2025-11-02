#ifndef JOGO_H
#define JOGO_H

#define POS(l,c) ((l) * 8 + (c))

char** gerar_movimentos_possiveis (char* tabuleiro, char lado); 

char* aplicar_jogada (char* tabuleiro_antigo, char* jogada);

void liberar_movimentos (char** movimentos);

int pos_valida (int l, int c);

int mov_possivel (char tipo, int lo, int co, int ld, int cd);

int vitoria (char lado, char* tab);

#endif