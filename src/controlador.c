#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>

#include "jogo/jogo.h"

#define MAXSTR 512
#define MAXINT 16 

int main(int argc, char **argv) {
    redisContext *contexto_redis;
    redisReply *resposta_redis;
    char mensagem_envio[MAXSTR];
    char jogada_str[MAXSTR];
    char key[16];
    char tempo_espera[16];
    char jogador_atual;
    char vencedor;
    char lado_jogada;
    char tipo_mov;
    int num_mov;
    int linhas_mov[MAXINT];
    int colunas_mov[MAXINT];
    int jogadas_restantes;
    int ok;
    char tabuleiro_atual[MAXSTR] = TABULEIRO_INICIAL;
    
    contexto_redis = iniciar(argc, argv, &jogador_atual, &jogadas_restantes, tempo_espera);
  
    vencedor = ' ';
  
    printf("%d:\n", jogadas_restantes);
    imprimir_tabuleiro(tabuleiro_atual);

    sprintf(mensagem_envio, "%c\n%c n\n%s", jogador_atual, OUTRO(jogador_atual), tabuleiro_atual);

    while(jogadas_restantes) {
        sprintf(key, "tabuleiro_%c", jogador_atual);
        resposta_redis = redisCommand(contexto_redis, "LTRIM %s 1 0", key);
        freeReplyObject(resposta_redis);
        resposta_redis = redisCommand(contexto_redis, "RPUSH %s %s", key, mensagem_envio);
        freeReplyObject(resposta_redis);
    
        ok = 0;
        sprintf(key, "jogada_%c", jogador_atual);
        resposta_redis = redisCommand(contexto_redis, "BLPOP %s %s", key, tempo_espera);
        if(resposta_redis->type != REDIS_REPLY_NIL) {
            strcpy(jogada_str, resposta_redis->element[1]->str);
            if (interpretar_jogada(jogada_str, &lado_jogada, &tipo_mov, &num_mov, linhas_mov, colunas_mov) &&
                jogador_atual == lado_jogada &&
                aplicar_jogada(mensagem_envio, tabuleiro_atual, lado_jogada, tipo_mov, num_mov, linhas_mov, colunas_mov)) {
                    strcpy(tabuleiro_atual, mensagem_envio);
                    ok = 1;
            }
        }
        freeReplyObject(resposta_redis);
        if(!ok)
            sprintf(jogada_str, "%c n", jogador_atual);

        printf("%d: %s\n", jogadas_restantes, jogada_str);
        imprimir_tabuleiro(tabuleiro_atual);

        if (eh_vencedor(jogador_atual, tabuleiro_atual)) {
            printf("%d: vitória de %c\n", jogadas_restantes, jogador_atual);
            vencedor = jogador_atual;
            break;
        }
    
        jogador_atual = OUTRO(jogador_atual);
        sprintf(mensagem_envio, "%c\n%s\n%s", jogador_atual, jogada_str, tabuleiro_atual);
        jogadas_restantes--;
    }

    sprintf(mensagem_envio, "o\nc n\n%s", tabuleiro_atual);
    resposta_redis = redisCommand(contexto_redis, "RPUSH tabuleiro_o %s", mensagem_envio);
    freeReplyObject(resposta_redis);

    sprintf(mensagem_envio, "c\no n\n%s", tabuleiro_atual);
    resposta_redis = redisCommand(contexto_redis, "RPUSH tabuleiro_c %s", mensagem_envio);
    freeReplyObject(resposta_redis);
  
    if(jogadas_restantes == 0)
        printf("empate\n");
    else    
        printf("vencedor: %c\n", vencedor);

    return 0;
}
