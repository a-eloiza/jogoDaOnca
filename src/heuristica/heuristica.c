#include "heuristica.h"

int encontrar_peca (char *tabuleiro, char peca, int *linha, int *coluna) {
    for (int l = 1; l <= 7; l++) {
        for (int c = 1; c <= 5; c++) {
            if (eh_posicao_valida (l, c)) {
                if (tabuleiro[POS(l, c)] == peca) {
                    *linha = l;
                    *coluna = c;
                    return 1;
                }
            }
        }
    }
    return 0;
}

int contar_caes (char *tabuleiro) {
    int count = 0;
    for (int l = 1; l <= 7; l++) {
        for (int c = 1; c <= 5; c++) {
            if (eh_posicao_valida (l, c) && tabuleiro[POS(l, c)] == 'c')
                count++;
        }
    }
    return count;
}

int calcular_cerco (char *tabuleiro) {
    int l_onca, c_onca;
    if (!encontrar_peca (tabuleiro, 'o', &l_onca, &c_onca))
        return 0;

    int cerco_total = 0;
    for (int l = 1; l <= 7; l++) {
        for (c = 1; c <= 5; c++) {
            if (eh_posicao_valida (l, c) && tabuleiro[POS(l, c)] == 'c') {
                int dist = abs (l - l_onca) + abs (c - c_onca);
                if (dist > 0)
                    cerco_total += (100 / dist);
            }
        }
    }
    return cerco_total;
}

int calcular_mobilidade_onca (char *tabuleiro) {
    jogada_t lista_jogadas[MAX_JOGADAS_BUFFER];
    int qtd_movimentos = gerar_movimentos(tabuleiro, LADO_ONCA, lista_jogadas);
    return qtd_movimentos;
}

int heuristica (char *tabuleiro) {
    int caes_restantes = contar_caes (tabuleiro);
    if (caes_restantes <= 9)
        return VITORIA_ONCA;

    int mobilidade_onca = calcular_mobilidade_onca (tabuleiro);
    if (mobilidade_onca == 0)
        return VITORIA_CAES;

    int pontuacao = (14 - caes_restantes) * PESO_CAPTURA;
    pontuacao += mobilidade_onca * PESO_MOBILIDADE;
    pontuacao -= calcular_cerco (tabuleiro) * PESO_CERCO;
    return pontuacao;
}