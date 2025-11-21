#ifndef __JOGO_H__
#define __JOGO_H__

#include <stdio.h>
#include <string.h>

#define LADO_ONCA 'o'
#define LADO_CACHORROS 'c'

#define TIPO_MOVIMENTO_UNICO 'm'
#define TIPO_SEQUENCIA 's'

#define LINHA_INICIAL_TABULEIRO 1
#define LINHA_FINAL_TABULEIRO 7
#define COLUNA_INICIAL_TABULEIRO 1
#define COLUNA_FINAL_TABULEIRO 5

#define OUTRO(lado) ((lado) == LADO_ONCA ? LADO_CACHORROS : LADO_ONCA)
#define POS(linha, coluna) ((linha) * 8 + (coluna))
#define ABS(x) ((x < 0)?(-(x)):(x))

#define MAXINT 16
#define TAMANHO_BUFFER_TABULEIRO 512
typedef struct {
    char tipo;
    int num_mov;
    int linhas[MAXINT];
    int colunas[MAXINT];
    int valor;           
} jogada_t;

int interpretar_jogada(char *entrada, char *lado_jogador, char *tipo_jogada, int *num_mov, int *linhas_mov, int *colunas_mov);
int eh_posicao_valida(int linha, int coluna);
int eh_movimento_possivel(char tipo_jogada, int linha_origem, int coluna_origem, int linha_destino, int coluna_destino);
int aplicar_jogada(char *tabuleiro_resultante, char *tabuleiro_inicial, char lado_jogador, char tipo_jogada, int num_mov, int *linhas_mov, int *colunas_mov);
int eh_vencedor(char lado_jogador, char *tabuleiro);
void ler_mensagem(char *buf, char *lado_meu, char *lado_adv, char *tabuleiro);
void formatar_jogada(char *buf_envio, char lado, jogada_t jogada);

#endif