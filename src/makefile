CFLAGS = -g -Wall -Wno-unused-result -O3
LDLIBS = -l hiredis -l readline

all: controlador exemplo1 exemplo2

controlador: jogo/jogo.h jogo/jogo.c controlador.c  

exemplo1: tabuleiro/tabuleiro.h tabuleiro/tabuleiro.c jogo/jogo.c exemplo1.c

exemplo2: tabuleiro/tabuleiro.h tabuleiro/tabuleiro.c exemplo2.c

clean:
	rm -f controlador exemplo1 exemplo2
