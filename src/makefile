# Para o Jogo "Tetris" no DE1-SoC

all: tetris

tetris: tetris.o
	gcc -o tetris tetris.o -std=c99 -lintelfpgaup

tetris.o: tetris.c
	gcc -c tetris.c