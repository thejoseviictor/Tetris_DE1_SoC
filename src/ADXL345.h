// Funções para Acessar e Manipular a Memória e o Acelerômetro ADXL345 na DE1-SoC

#ifndef ADXL345_H
#define ADXL345_H

#include "stdint.h" // Para Inteiros de 8 e 16 bits
#include <stdio.h> // Entrada e Saída, como "printf"
#include <stdlib.h>
#include "fcntl.h" // Para Manipulação de Arquivos
#include "unistd.h" // Para Fechar Arquivos
#include "time.h" // Função "sleep"
#include "sys/mman.h"// Para Mapear Endereço Físico em Endereço Virtual

// Registradores do ADXL345:
#define ADXL345_ADDRESS 0x53 // Endereço Base do Acelerômetro
#define THRESH_ACT 0x24
#define THRESH_INACT 0x25
#define TIME_INACT 0x26
#define ACT_INACT_CTL 0x27
#define BW_RATE 0x2C
#define POWER_CTL 0x2D
#define INT_ENABLE 0x2E
#define DATA_FORMAT 0x31
#define DATAX0 0x32 // Byte Menos Significativo de X
#define DATAX1 0x33 // Byte Mais Significativo de X

int open_memory(void);
void enable_I2C0_HPS(int);
void setting_I2C0_ADXL345(void *);
void setting_ADXL345(volatile unsigned int *);
int16_t lerEixoX(volatile unsigned int *, volatile unsigned int *);

#endif // ADXL345_H