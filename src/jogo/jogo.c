#include "jogo.h"

int interpretar_jogada(char *entrada, char *lado_jogador, char *tipo_jogada, int *num_mov, int *linhas_mov, int *colunas_mov)
{
    int i, p;
    char *s;

    if (!(s = strtok(entrada, " \n")) || sscanf(s, "%c", lado_jogador) != 1)
        return 0;
    if ((*lado_jogador != LADO_CACHORROS) && (*lado_jogador != LADO_ONCA))
        return 0;
    if (!(s = strtok(NULL, " \n")) || sscanf(s, "%c", tipo_jogada) != 1)
        return 0;
    if (*tipo_jogada == 'n')
        return 1;
    if (*tipo_jogada != TIPO_MOVIMENTO_UNICO && *tipo_jogada != TIPO_SEQUENCIA)
        return 0;

    if (*tipo_jogada == TIPO_MOVIMENTO_UNICO)
    {
        *num_mov = 1;
        for (i = 0; i <= *num_mov; i++)
        {
            if (!(s = strtok(NULL, " \n")) || sscanf(s, "%d", &(linhas_mov[i])) != 1)
                return 0;
            if (!(s = strtok(NULL, " \n")) || sscanf(s, "%d", &(colunas_mov[i])) != 1)
                return 0;
        }
    }
    else
    {
        if (*lado_jogador == LADO_CACHORROS)
            return 0;
        if (!(s = strtok(NULL, " \n")) || sscanf(s, "%d", num_mov) != 1)
            return 0;
        if (*num_mov < 1)
            return 0;
        for (i = 0; i <= *num_mov; i++)
        {
            if (!(s = strtok(NULL, " \n")) || sscanf(s, "%d", &(linhas_mov[i])) != 1)
                return 0;
            if (!(s = strtok(NULL, " \n")) || sscanf(s, "%d", &(colunas_mov[i])) != 1)
                return 0;
        }
    }

    p = 0;
    p += sprintf(&(entrada[p]), "%c %c", *lado_jogador, *tipo_jogada);

    if (*tipo_jogada == TIPO_SEQUENCIA)
        p += sprintf(&(entrada[p]), " %d", *num_mov);

    for (i = 0; i <= *num_mov; i++)
        p += sprintf(&(entrada[p]), " %d %d", linhas_mov[i], colunas_mov[i]);

    return 1;
}

int eh_posicao_valida(int linha, int coluna)
{
    if (linha < LINHA_INICIAL_TABULEIRO || linha > LINHA_FINAL_TABULEIRO || coluna < COLUNA_INICIAL_TABULEIRO || coluna > COLUNA_FINAL_TABULEIRO)
        return 0;

    if (linha == 6 && (coluna == 1 || coluna == 5))
        return 0;

    if (linha == 7 && (coluna == 2 || coluna == 4))
        return 0;

    return 1;
}

int eh_movimento_possivel(char tipo_jogada, int linha_origem, int coluna_origem, int linha_destino, int coluna_destino)
{
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

    if (tipo_jogada == TIPO_MOVIMENTO_UNICO)
    {
        if ((linha_origem == 7) && (dist_linha == 0))
        {
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

        if ((linha_origem == 6) && ((coluna_origem % 2) == 0))
        {
            if ((linha_destino == 5) && (coluna_destino != 3))
                return 0;
            if ((linha_destino == 7) && (coluna_destino == 3))
                return 0;
        }

        return 1;
    }
    else if (tipo_jogada == TIPO_SEQUENCIA)
    {
        if ((linha_origem == 7) && (dist_linha == 0))
        {
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

int aplicar_jogada(char *tabuleiro_resultante, char *tabuleiro_inicial, char lado_jogador, char tipo_jogada, int num_mov, int *linhas_mov, int *colunas_mov)
{
    int i, linha_atual, coluna_atual, pos_atual, linha_nova, coluna_nova, pos_nova;

    strcpy(tabuleiro_resultante, tabuleiro_inicial);
    if (tipo_jogada == 'n')
        return 1;
    if (tipo_jogada == TIPO_MOVIMENTO_UNICO)
    {
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
    }
    else
    { // tipo s
        linha_atual = linhas_mov[0];
        coluna_atual = colunas_mov[0];
        pos_atual = POS(linha_atual, coluna_atual);
        if ((lado_jogador != LADO_ONCA) || (tabuleiro_resultante[pos_atual] != LADO_ONCA))
            return 0;
        for (i = 1; i <= num_mov; i++)
        {
            linha_nova = linhas_mov[i];
            coluna_nova = colunas_mov[i];
            if (!eh_movimento_possivel(TIPO_SEQUENCIA, linha_atual, coluna_atual, linha_nova, coluna_nova))
                return 0;
            tabuleiro_resultante[pos_atual] = '-';
            pos_nova = POS(linha_nova, coluna_nova);
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

int eh_vencedor(char lado_jogador, char *tabuleiro)
{
    int linha, coluna, num_cachorros, i, j;

    if (lado_jogador == LADO_ONCA)
    {
        num_cachorros = 0;
        for (linha = 1; linha < 8; linha++)
            for (coluna = 1; coluna < 6; coluna++)
                if (tabuleiro[POS(linha, coluna)] == LADO_CACHORROS)
                    num_cachorros++;
        if (num_cachorros > 9)
            return 0;
        return 1;
    }

    for (linha = 1; linha < 8; linha++)
        for (coluna = 1; coluna < 6; coluna++)
            if (tabuleiro[POS(linha, coluna)] == LADO_ONCA)
            {
                for (i = -1; i <= 1; i++)
                    for (j = -1; j <= 1; j++)
                        if ((eh_movimento_possivel(TIPO_MOVIMENTO_UNICO, linha, coluna, linha + i, coluna + j) &&
                             (tabuleiro[POS(linha + i, coluna + j)] == '-')) ||
                            (eh_movimento_possivel(TIPO_SEQUENCIA, linha, coluna, linha + 2 * i, coluna + 2 * j) &&
                             (tabuleiro[POS(linha + i, coluna + j)] == LADO_CACHORROS) &&
                             (tabuleiro[POS(linha + 2 * i, coluna + 2 * j)] == '-')))
                        {

                            return 0;
                        }
                return 1;
            }
    return 0;
}

void ler_mensagem(char *buf, char *lado_meu, char *lado_adv, char *tabuleiro)
{
    char *token;
    char tipo_mov_adv;
    int num_mov_adv, i;

    token = strtok(buf, " \n");
    if (!token)
        return;
    sscanf(token, "%c", lado_meu);

    token = strtok(NULL, " \n");
    if (!token)
        return;
    sscanf(token, "%c", lado_adv);

    token = strtok(NULL, " \n");
    if (!token)
        return;
    tipo_mov_adv = token[0];

    if (tipo_mov_adv == TIPO_MOVIMENTO_UNICO)
    {
        strtok(NULL, " \n");
        strtok(NULL, " \n");
        strtok(NULL, " \n");
        strtok(NULL, " \n");
    }
    else if (tipo_mov_adv == TIPO_SEQUENCIA)
    {
        token = strtok(NULL, " \n");
        if (!token)
            return;
        sscanf(token, "%d", &num_mov_adv);
        for (i = 0; i <= num_mov_adv; i++)
        {
            if (!strtok(NULL, " \n"))
                return;
            if (!strtok(NULL, " \n"))
                return;
        }
    }

    token = strtok(NULL, ".");
    if (!token)
    {
        // mensagem malformada
        tabuleiro[0] = '\0';
        return;
    }

    // copia com segurança e garante '\0'
    strncpy(tabuleiro, token, TAMANHO_BUFFER_TABULEIRO - 1);
    tabuleiro[TAMANHO_BUFFER_TABULEIRO - 1] = '\0';
}

void formatar_jogada(char *buf_envio, char lado, jogada_t jogada)
{
    int pos = 0;

    pos += sprintf(buf_envio, "%c %c", lado, jogada.tipo);

    if (jogada.tipo == TIPO_SEQUENCIA)
    {
        pos += sprintf(buf_envio + pos, " %d", jogada.num_mov);
    }

    for (int i = 0; i <= jogada.num_mov; i++)
    {
        pos += sprintf(buf_envio + pos, " %d %d", jogada.linhas[i], jogada.colunas[i]);
    }

    strcat(buf_envio, "\n");
}

void buscar_saltos_dfs(char *tabuleiro, jogada_t jogada_atual, jogada_t *lista_jogadas, int *num_jogadas)
{
    int i, j;
    int linha_atual = jogada_atual.linhas[jogada_atual.num_mov];
    int coluna_atual = jogada_atual.colunas[jogada_atual.num_mov];

    if (jogada_atual.num_mov >= MAXINT)
        return;

    for (i = -2; i <= 2; i += 2)
    {
        for (j = -2; j <= 2; j += 2)
        {
            if (i == 0 && j == 0)
                continue;

            int l2 = linha_atual + i;
            int c2 = coluna_atual + j;
            int l_meio = (linha_atual + l2) / 2;
            int c_meio = (coluna_atual + c2) / 2;

            if (!eh_posicao_valida(l2, c2))
                continue;
            if (!eh_posicao_valida(l_meio, c_meio))
                continue;

            if (tabuleiro[POS(l_meio, c_meio)] != LADO_CACHORROS)
                continue;
            if (tabuleiro[POS(l2, c2)] != '-')
                continue;

            if (!eh_movimento_possivel(TIPO_SEQUENCIA, linha_atual, coluna_atual, l2, c2))
                continue;

            char backup_origem = tabuleiro[POS(linha_atual, coluna_atual)];
            char backup_meio = tabuleiro[POS(l_meio, c_meio)];
            char backup_dest = tabuleiro[POS(l2, c2)];

            // marca no tabuleiro
            tabuleiro[POS(linha_atual, coluna_atual)] = '-';
            tabuleiro[POS(l_meio, c_meio)] = '-';       // come o cachorro
            tabuleiro[POS(l2, c2)] = LADO_ONCA; // onça pousa

            jogada_t nova_jogada = jogada_atual;
            nova_jogada.num_mov++;
            nova_jogada.linhas[nova_jogada.num_mov] = l2;
            nova_jogada.colunas[nova_jogada.num_mov] = c2;

            if (*num_jogadas < MAX_JOGADAS_BUFFER)
            {
                lista_jogadas[*num_jogadas] = nova_jogada;
                (*num_jogadas)++;
            }
            else
            {
                // Lista cheia: desfazemos a alteração e retornamos para evitar o crash
                tabuleiro[POS(linha_atual, coluna_atual)] = backup_origem;
                tabuleiro[POS(l_meio, c_meio)] = backup_meio;
                tabuleiro[POS(l2, c2)] = backup_dest;
                return;
            }

            buscar_saltos_dfs(tabuleiro, nova_jogada, lista_jogadas, num_jogadas);

            // restaura o tabuleiro
            tabuleiro[POS(linha_atual, coluna_atual)] = backup_origem;
            tabuleiro[POS(l_meio, c_meio)] = backup_meio;
            tabuleiro[POS(l2, c2)] = backup_dest;
        }
    }
}

int gerar_movimentos(char *tabuleiro, char meu_lado, jogada_t lista_jogadas[])
{
    int linha, coluna, i, j;
    int num_jogadas = 0;

    for (linha = LINHA_INICIAL_TABULEIRO; linha <= LINHA_FINAL_TABULEIRO; linha++)
    {
        for (coluna = COLUNA_INICIAL_TABULEIRO; coluna <= COLUNA_FINAL_TABULEIRO; coluna++)
        {

            int pos = POS(linha, coluna);
            if (tabuleiro[pos] != meu_lado)
                continue;

            // Movimentos simples
            for (i = -1; i <= 1; i++)
            {
                for (j = -1; j <= 1; j++)
                {
                    if (i == 0 && j == 0)
                    {
                        continue;
                    }

                    int l2 = linha + i;
                    int c2 = coluna + j;

                    if (!eh_posicao_valida(l2, c2))
                        continue;

                    if (tabuleiro[POS(l2, c2)] != '-')
                        continue;

                    if (!eh_movimento_possivel(TIPO_MOVIMENTO_UNICO, linha, coluna, l2, c2))
                        continue;

                    if (num_jogadas >= MAX_JOGADAS_BUFFER)
                    {
                        break; // Sai do loop imediatamente
                    }

                    jogada_t nova;
                    nova.tipo = TIPO_MOVIMENTO_UNICO;
                    nova.num_mov = 1;
                    nova.linhas[0] = linha;
                    nova.colunas[0] = coluna;
                    nova.linhas[1] = l2;
                    nova.colunas[1] = c2;
                    nova.valor = 0;

                    lista_jogadas[num_jogadas++] = nova;
                }
            }

            // Saltos da onça
            char tabuleiro_rascunho[TAMANHO_BUFFER_TABULEIRO];
            strncpy(tabuleiro_rascunho, tabuleiro, TAMANHO_BUFFER_TABULEIRO - 1);
            tabuleiro_rascunho[TAMANHO_BUFFER_TABULEIRO - 1] = '\0';
            if (meu_lado == LADO_ONCA)
            {
                jogada_t base;
                base.tipo = TIPO_SEQUENCIA;
                base.num_mov = 0;
                base.linhas[0] = linha;
                base.colunas[0] = coluna;
                base.valor = 0;

                strcpy(tabuleiro_rascunho, tabuleiro);

                // faz a busca dos saltos
                buscar_saltos_dfs(tabuleiro_rascunho, base, lista_jogadas, &num_jogadas);
            }
        }
    }

    return num_jogadas;
}