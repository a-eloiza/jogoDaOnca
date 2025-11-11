#include "jogo.h"

/* ------------------ Comunicação e inicialização ------------------ */
redisContext* iniciar(int argc, char **argv, char *lado, int *jogadas, char *tempo) {
    redisContext *rediscontext;
    char *ip;
    int porta;
    
    if(argc < 4) {
        printf("formato:\n");
        printf("         %s lado jogadas tempo [ip porta]\n\n", argv[0]);
        printf("   lado: indica que lado inicia o jogo, os valores possívies são o ou c\n");
        printf("   jogadas: número máximo do jogadas na partida\n");
        printf("   tempo: limite em segundos para cada jogada, 0 indica sem limite\n");
        printf("   ip: parâmetro opcional que indica o ip ou o hostname do servidor redis\n");
        printf("       o valor default é 127.0.0.1\n");
        printf("   porta: parâmetro opcional que indica a porta do servidor redis\n");
        printf("          o valor default é 10001\n");
        exit(1);
    }
    
    *lado = argv[1][0];
    *jogadas = atoi(argv[2]);
    strcpy(tempo, argv[3]);
    ip = (argc > 4) ? argv[4] : "127.0.0.1";
    porta = (argc > 5) ? atoi(argv[5]) : 10001;
    
    rediscontext = redisConnect(ip, porta);
    if (rediscontext == NULL || rediscontext->err) {
        if(rediscontext) {
        printf("Erro ao conectar com o servidor redis: %s\n", rediscontext->errstr);
        exit(1);
        } else {
        printf("Não foi possível conectar com o servidor redis\n");
        }
    }
    return rediscontext;
} 

/* ------------------ Interpretação de jogadas ------------------ */
int interpretar_jogada(char *entrada, char *lado_jogador, char *tipo_jogada, int *num_mov, int *linhas_mov, int *colunas_mov) {
    int i, p;
    char *s;

    if (!(s = strtok(entrada, " \n")) || sscanf(s , "%c", lado_jogador) != 1)
        return 0;
    if ((*lado_jogador != LADO_CACHORROS) && (*lado_jogador != LADO_ONCA))
        return 0;
    if (!(s = strtok(NULL, " \n")) || sscanf(s, "%c", tipo_jogada) != 1)
        return 0;
    if (*tipo_jogada == 'n')
        return 1;
    if (*tipo_jogada != TIPO_MOVIMENTO_UNICO && *tipo_jogada != TIPO_SEQUENCIA)
        return 0;

    if (*tipo_jogada == TIPO_MOVIMENTO_UNICO) {
        *num_mov = 1;
        for(i = 0; i <= *num_mov; i++) {
            if(!(s = strtok(NULL, " \n")) || sscanf(s, "%d", &(linhas_mov[i])) != 1)
                return 0;
            if(!(s = strtok(NULL, " \n")) || sscanf(s, "%d", &(colunas_mov[i])) != 1)
                return 0;
        }
    } else {
        if (*lado_jogador == LADO_CACHORROS)
            return 0;
        if (!(s = strtok(NULL, " \n")) || sscanf(s, "%d", num_mov) != 1)
            return 0;
        if (*num_mov < 1)
            return 0;
        for(i = 0; i <= *num_mov; i++) {
            if (!(s = strtok(NULL, " \n")) || sscanf(s, "%d", &(linhas_mov[i])) != 1)
                return 0;
            if (!(s = strtok(NULL, " \n")) || sscanf(s, "%d", &(colunas_mov[i])) != 1)
                return 0;
        }
    }    

    p = 0;
    p += sprintf(&(entrada[p]), "%c %c", *lado_jogador, *tipo_jogada);
    
    if (*tipo_jogada == TIPO_SEQUENCIA)
        p += sprintf(&(entrada[p]), " %d", *num_mov - 1);
    
    for (i = 0; i < *num_mov; i++)
        p += sprintf(&(entrada[p]), " %d %d", linhas_mov[i], colunas_mov[i]);
    
    return 1;
}

/* ------------------ Regras e validação ------------------ */
int eh_posicao_valida(int linha, int coluna) {
    if (linha < LINHA_INICIAL_TABULEIRO || linha > LINHA_FINAL_TABULEIRO || coluna < COLUNA_INICIAL_TABULEIRO || coluna > COLUNA_FINAL_TABULEIRO)
        return 0;
    
    if (linha == 6 && (coluna == 1 || coluna == 5))
        return 0;
    
    if (linha == 7 && (coluna == 2 || coluna == 4))
        return 0;
    
    return 1;
}

int eh_movimento_possivel(char tipo_jogada, int linha_origem, int coluna_origem, int linha_destino, int coluna_destino) {
    int dist_linha, dist_coluna;

    if (!eh_posicao_valida(linha_origem, coluna_origem))
        return 0;
    if (!eh_posicao_valida(linha_destino, coluna_destino))
        return 0;
    
    dist_linha = linha_origem - linha_destino;
    dist_linha = ABS(dist_linha);
    dist_coluna = coluna_origem - coluna_destino;
    dist_coluna = ABS(dist_coluna);
    
    if ((dist_linha + dist_coluna) == 0)
        return 0;
    
    if (tipo_jogada == TIPO_MOVIMENTO_UNICO) {
        if ((linha_origem == 7) && (dist_linha == 0)) { 
            if (dist_coluna == 2)
                return 1;
            else
                return 0;
        }
        
        if ((dist_linha > 1) || (dist_coluna > 1))
            return 0;
        
        if (((linha_origem + coluna_origem) % 2) && ((dist_linha + dist_coluna) > 1))
            return 0;
        
        if ((linha_origem == 5) && (linha_destino == 6) && (coluna_origem != 3))
            return 0;
        
        if ((linha_origem == 6) && ((coluna_origem % 2) == 0)) {
            if ((linha_destino == 5) && (coluna_destino != 3))
                return 0;
            if ((linha_destino == 7) && (coluna_destino == 3))
                return 0;
        }
        
        return 1;
    } else if (tipo_jogada == TIPO_SEQUENCIA) {
        if ((linha_origem == 7) && (dist_linha == 0)) {
            if (dist_coluna == 4)
                return 1;
            else
                return 0;
        }
        
        if ((dist_linha == 1) || (dist_coluna == 1) || (dist_linha + dist_coluna) > 4)
            return 0;
        
        if (((linha_origem + coluna_origem) % 2) && ((dist_linha + dist_coluna) > 2))
            return 0;
    
        if ((linha_origem == 5) && (linha_destino == 7) && (coluna_origem != 3))
            return 0;
    
    
        if ((linha_origem == 6) && (linha_destino == 4) && (((coluna_origem == 2) && (coluna_destino != 4)) || ((coluna_origem == 4) && (coluna_destino != 2))))
            return 0;
    
        if ((linha_origem == 7) && (coluna_destino != 3))
            return 0;
    
        return 1;
    }
    return 0;
}

int aplicar_jogada(char *tabuleiro_resultante, char *tabuleiro_inicial, char lado_jogador, char tipo_jogada, int num_mov, int *linhas_mov, int *colunas_mov) {
    int i, linha_atual, coluna_atual, pos_atual, linha_nova, coluna_nova, pos_nova;

    strcpy(tabuleiro_resultante, tabuleiro_inicial);
    if (tipo_jogada == 'n')
        return 1;
    if (tipo_jogada == TIPO_MOVIMENTO_UNICO) {
        linha_atual = linhas_mov[0];
        coluna_atual = colunas_mov[0];
        linha_nova = linhas_mov[1];
        coluna_nova = colunas_mov[1];
        if (!eh_movimento_possivel(TIPO_MOVIMENTO_UNICO, linha_atual, coluna_atual, linha_nova, coluna_nova))
            return 0;
        pos_atual = POS(linha_atual, coluna_atual);
        if (tabuleiro_resultante[pos_atual] != lado_jogador)
            return 0;
        pos_nova = POS(linha_nova, coluna_nova);
        if (tabuleiro_resultante[pos_nova] != '-')
            return 0;
        tabuleiro_resultante[pos_atual] = '-';
        tabuleiro_resultante[pos_nova] = lado_jogador;
    } else { // tipo s
        linha_atual = linhas_mov[0];
        coluna_atual = colunas_mov[0];
        pos_atual = POS(linha_atual, coluna_atual);
        if ((lado_jogador != LADO_ONCA) || (tabuleiro_resultante[pos_atual] != LADO_ONCA))
            return 0;
        for (i = 1; i <= num_mov; i++) {
            linha_nova = linhas_mov[i];
            coluna_nova = colunas_mov[i];
            if (!eh_movimento_possivel(TIPO_SEQUENCIA, linha_atual, coluna_atual, linha_nova, coluna_nova))
                return 0;
            tabuleiro_resultante[pos_atual] = '-';
            pos_nova = POS(linha_nova,coluna_nova);
            if (tabuleiro_resultante[pos_nova] != '-')
                return 0;
            linha_atual = (linha_atual + linha_nova) / 2; // posicao do cachorro que sera saltado
            coluna_atual = (coluna_atual + coluna_nova) / 2;
            pos_atual = POS(linha_atual, coluna_atual);
            if (tabuleiro_resultante[pos_atual] != LADO_CACHORROS)
                return 0;
            tabuleiro_resultante[pos_atual] = '-';
            tabuleiro_resultante[pos_nova] = LADO_ONCA;
            linha_atual = linha_nova;
            coluna_atual = coluna_nova;
            pos_atual = pos_nova;
        }
    }
  
    return 1;
}

int eh_vencedor(char lado_jogador, char *tabuleiro) {
    int linha, coluna, num_cachorros, i, j;

    if (lado_jogador == LADO_ONCA) {
        num_cachorros = contar_cachorros(tabuleiro);
        if (num_cachorros > NUM_CACHORROS_VITORIA_ONCA)
            return 0;
        return 1;
    }

    for (linha = LINHA_INICIAL_TABULEIRO; linha <= LINHA_FINAL_TABULEIRO; linha++)
        for (coluna = COLUNA_INICIAL_TABULEIRO; coluna <= COLUNA_FINAL_TABULEIRO; coluna++)
            if (tabuleiro[POS(linha, coluna)] == LADO_ONCA) {
                for (i = -1; i <= 1; i++)
                    for (j = -1; j <= 1; j++)
                        if ((eh_movimento_possivel(TIPO_MOVIMENTO_UNICO, linha, coluna, linha + i, coluna + j) && 
                            (tabuleiro[POS(linha + i, coluna + j)] == '-')) || 
                            (eh_movimento_possivel(TIPO_SEQUENCIA, linha, coluna, linha + 2 * i, coluna + 2 * j) && 
                            (tabuleiro[POS(linha + i, coluna + j)] == LADO_CACHORROS) && 
                            (tabuleiro[POS(linha + 2 * i, coluna + 2 * j)] == '-'))) {
                            
                            return 0;
                        }
                return 1;
            }
    return 0;
}

/* ------------------ Utilitários ------------------ */
int contar_cachorros(char *tabuleiro) {
    int num_cachorros = 0;
    for (int linha = LINHA_INICIAL_TABULEIRO; linha <= LINHA_FINAL_TABULEIRO; linha++) {
        for (int coluna = COLUNA_INICIAL_TABULEIRO; coluna <= COLUNA_FINAL_TABULEIRO; coluna++) {
            if (tabuleiro[POS(linha, coluna)] == LADO_CACHORROS) {
                num_cachorros++;
            }
        }
    }
}

void imprimir_tabuleiro(char *tabuleiro) {
    printf("%s\n", tabuleiro);
}