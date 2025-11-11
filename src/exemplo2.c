#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "tabuleiro/tabuleiro.h"

#define MAXSTR 512

int main(int argc, char **argv) {
  char buf[MAXSTR];
  char *linha;
  
  conectar_tabuleiro(argc, argv);

  while(1) {
    receber_tabuleiro(buf);
    printf("%s", buf);
    linha = readline(NULL);
    if(linha[0] == '0')
      break;
    sprintf(buf, "%s\n", linha);
    free(linha);
    enviar_tabuleiro(buf);
  }
}
