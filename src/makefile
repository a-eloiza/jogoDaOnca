CC = gcc
CFLAGS = -g -Wall -Wno-unused-result -O3 -I.
LDLIBS = -l hiredis -l readline

all: controlador exemplo1 exemplo2 jogador

jogador: jogador.c minimax/minimax.c heuristica/heuristica.c jogo/jogo.c tabuleiro/tabuleiro.c
	$(CC) $(CFLAGS) jogador.c minimax/minimax.c heuristica/heuristica.c jogo/jogo.c tabuleiro/tabuleiro.c -o jogador $(LDLIBS)

controlador: jogo/jogo.c controlador.c  
	$(CC) $(CFLAGS)  controlador.c jogo/jogo.c -o controlador $(LDLIBS)

exemplo1: tabuleiro/tabuleiro.c jogo/jogo.c exemplo1.c

exemplo2: tabuleiro/tabuleiro.c exemplo2.c

clean:
	rm -f controlador exemplo1 exemplo2 jogador
