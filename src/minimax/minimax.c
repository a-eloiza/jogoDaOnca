#include "minimax.h"

int teste_terminal(int profundidade, char *tabuleiro){
    if(profundidade >= MAX_PROFUNDIDADE || eh_vencedor(LADO_ONCA, tabuleiro) || eh_vencedor(LADO_CACHORROS, tabuleiro))
        return 1;
    return 0;
}

int calcula_MIN (char *tabuleiro, int alpha, int beta, int profundidade){
    if (teste_terminal(profundidade, tabuleiro))
        return heuristica(tabuleiro);

    jogada_t lista_jogadas[MAX_MOVIMENTOS_POSSIVEIS];
    char tabuleiro_temp[TAMANHO_BUFFER_TABULEIRO];
        
    int qtd = gerar_movimentos(tabuleiro, LADO_CACHORROS, lista_jogadas);
        
    int nota = INFINITO;
    if (qtd == 0) return VITORIA_ONCA;

    for(int i = 0; i < qtd; i++){
        strcpy(tabuleiro_temp, tabuleiro);
        aplicar_jogada(tabuleiro_temp, tabuleiro, LADO_CACHORROS, lista_jogadas[i].tipo, lista_jogadas[i].num_mov, lista_jogadas[i].linhas, lista_jogadas[i].colunas);
        
        int nota_filho = calcula_MAX(tabuleiro_temp, alpha, beta, profundidade + 1);
        if (nota_filho < nota) nota = nota_filho;
        if (nota <= alpha) return nota; // poda
        if (nota < beta) beta = nota;
    }

    return nota;
}

int calcula_MAX (char *tabuleiro, int alpha, int beta, int profundidade){
    if (teste_terminal(profundidade, tabuleiro))
        return heuristica(tabuleiro);

    int nota = -INFINITO;
    jogada_t lista_jogadas[MAX_MOVIMENTOS_POSSIVEIS];
    char tabuleiro_temp[TAMANHO_BUFFER_TABULEIRO];

    int qtd = gerar_movimentos(tabuleiro, LADO_ONCA, lista_jogadas);

    if (qtd == 0) return VITORIA_CAES;

    for(int i = 0; i < qtd; i++){
        strcpy(tabuleiro_temp, tabuleiro);
        aplicar_jogada(tabuleiro_temp, tabuleiro, LADO_ONCA, lista_jogadas[i].tipo, lista_jogadas[i].num_mov, lista_jogadas[i].linhas, lista_jogadas[i].colunas);
        
        int nota_filho = calcula_MIN(tabuleiro_temp, alpha, beta, profundidade + 1);
        if (nota_filho > nota) nota = nota_filho;
        if (nota >= beta) return nota; // poda
        if (nota > alpha) beta = nota;
    }

    return nota;
}

jogada_t minimax (char *tabuleiro, char meu_lado){
    jogada_t lista_jogadas[MAX_MOVIMENTOS_POSSIVEIS];
    char tabuleiro_temp[TAMANHO_BUFFER_TABULEIRO];
    
    int alpha = -INFINITO;
    int beta = INFINITO;
    jogada_t melhor_jogada;

    int qtd_jogadas = gerar_movimentos(tabuleiro, meu_lado, lista_jogadas);
    if (qtd_jogadas == 0) melhor_jogada.num_mov = 0;

    
    if (meu_lado == LADO_ONCA) 
        melhor_jogada.valor = -INFINITO;
    else                        
        melhor_jogada.valor = INFINITO;
    
    int nota;
    for(int i = 0; i < qtd_jogadas; i++){
        strcpy(tabuleiro_temp, tabuleiro);
        aplicar_jogada(tabuleiro_temp, tabuleiro, meu_lado, lista_jogadas[i].tipo, lista_jogadas[i].num_mov, lista_jogadas[i].linhas, lista_jogadas[i].colunas);
        
        if(meu_lado == LADO_ONCA){
            nota = calcula_MIN(tabuleiro_temp, alpha, beta, 1);
            if (nota > melhor_jogada.valor){
                melhor_jogada = lista_jogadas[i];
                melhor_jogada.valor = nota; 
            }
            if(nota >  alpha) alpha = nota;
        }
        else{
            nota = calcula_MAX(tabuleiro_temp, alpha, beta, 1);
            if (nota < melhor_jogada.valor){
                melhor_jogada = lista_jogadas[i];
                melhor_jogada.valor = nota; 
            }
            if(nota < beta) beta = nota;
        }
    }

    return melhor_jogada;
}