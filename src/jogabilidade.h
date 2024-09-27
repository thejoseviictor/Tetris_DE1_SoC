// Funções de Jogabilidade do Tetris

#ifndef JOGABILIDADE_H
#define JOGABILIDADE_H

#include <stdio.h> // Entrada e Saída, como "printf"
#include <stdlib.h> // Funções "rand" e "srand"
#include <intelfpgaup/video.h> // Exibir no Monitor
#include "fcntl.h" // Para Manipulação de Arquivos
#include "unistd.h" // Para Fechar Arquivos
#include "time.h" // Função "sleep"

//Tamanho do jogo 20x10
#define LINHA 20
#define COLUNA 10
#define TEMPO_QUEDA 300
#define TAMANHO_DO_BLOCO 8

void desenharBloco(int, int, short int);
void desenharBlocoTetris(int, int, short int);
void desenharCampo(int, int, int);
int colisao(int, int, int);
void fixarPeca(int, int, int);
void checarLinhas();
int descida(int, int *, int);
void mover(int, int *, int, int);

#endif // JOGABILIDADE_H