#include "../minimax/minimax.h"
#include <readline/readline.h>
#include <readline/history.h>

#define DEBUG 1

int main(int argc, char **argv){
    char buf[TAMANHO_BUFFER_TABULEIRO];           
    char buffer_envio[TAMANHO_BUFFER_TABULEIRO];  
    char tabuleiro[TAMANHO_BUFFER_TABULEIRO];     
    char lado_meu;              
    char lado_adv;
    char tipo_mov_adv;
    int num_mov_adv;
    int i;

    tabuleiro_conecta(argc, argv);

    int venceu_onca, venceu_cao = 0;
    while(!venceu_onca && !venceu_cao) {
        tabuleiro_recebe(buf); 

        ler_mensagem(buf, &lado_meu, &lado_adv, tabuleiro);tqop

        strncpy(tabuleiro, strtok(NULL, "."), MAXSTR-1);

        venceu_onca = eh_vencedor(LADO_ONCA, tabuleiro);
        venceu_cao = eh_vencedor(LADO_CACHORROS, tabuleiro);

        jogada_t jogada = minimax(tabuleiro, lado_meu);
        
        formatar_jogada(buf_envio, lado_meu, jogada);

        #if DEBUG
            printf(" - jogador: %c --- tabuleiro: %s", lado_meu, buf_envio);
        #endif

        tabuleiro_envia(buf_envio); 
    }

    if (venceu_onca) printf("Fim de jogo: Vitoria da Onca!\n");
    if (venceu_cao) printf("Fim de jogo: Vitoria dos Caes!\n");
    return 0;
}