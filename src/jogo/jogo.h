#ifndef __JOGO_H__
#define __JOGO_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <hiredis/hiredis.h>

#define LADO_ONCA 'o'
#define LADO_CACHORROS 'c'
#define NUM_CACHORROS_INICIAL 14
#define NUM_CACHORROS_VITORIA_ONCA 9

#define TIPO_MOVIMENTO_UNICO 'm'
#define TIPO_SEQUENCIA 's'

#define LINHA_INICIAL_TABULEIRO 1
#define LINHA_FINAL_TABULEIRO 7
#define COLUNA_INICIAL_TABULEIRO 1
#define COLUNA_FINAL_TABULEIRO 5
#define TABULEIRO_INICIAL "#######\n#ccccc#\n#ccccc#\n#ccocc#\n#-----#\n#-----#\n# --- #\n#- - -#\n#######\n";

#define OUTRO(lado) ((lado) == LADO_ONCA ? LADO_CACHORROS : LADO_ONCA)
#define POS(linha, coluna) ((linha) * 8 + (coluna))
#define ABS(x) ((x < 0)?(-(x)):(x))

/* ------------------ Comunicação e inicialização ------------------ */
redisContext* iniciar(int argc, char **argv, char *lado, int *jogadas, char *tempo);

/* ------------------ Interpretação de jogadas ------------------ */
int interpretar_jogada(char *entrada, char *lado_jogador, char *tipo_jogada, int *num_mov, int *linhas_mov, int *colunas_mov);

/* ------------------ Regras e validação ------------------ */
int eh_posicao_valida(int linha, int coluna);
int eh_movimento_possivel(char tipo_jogada, int linha_origem, int coluna_origem, int linha_destino, int coluna_destino);
int aplicar_jogada(char *tabuleiro_resultante, char *tabuleiro_inicial, char lado_jogador, char tipo_jogada, int num_mov, int *linhas_mov, int *colunas_mov);
int eh_vencedor(char lado_jogador, char *tabuleiro);

/* ------------------ Utilitários ------------------ */
int contar_cachorros(char *tabuleiro);
void imprimir_tabuleiro(char *tabuleiro);

#endif