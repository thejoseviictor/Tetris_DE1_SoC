#include "stdint.h" // Para Inteiros de 8 e 16 bits
#include <stdio.h>  // Entrada e Saída, como "printf"
#include <stdlib.h>
#include "fcntl.h"    // Para Manipulação de Arquivos
#include "unistd.h"   // Para Fechar Arquivos
#include "time.h"     // Função "sleep"
#include "sys/mman.h" // Para Mapear Endereço Físico em Endereço Virtual
#include <time.h>

#include "return_decomposto.h"
#include "exibir_pontos.h"

int exibir_pontos(int pontos)
{
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    size_t GPIO0_SPAN = 4096;
    off_t GPIO0_OFST = 0xff200000;
    void *GPIO_VIRTUAL = mmap(NULL, GPIO0_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO0_OFST);

    volatile unsigned int *KEYS_BASE = GPIO_VIRTUAL + 0x0; // botoes

    // Digitos do display
    volatile unsigned int *HEX5_BASE = GPIO_VIRTUAL + 0x10; // HEX5:Digito apagado

    volatile unsigned int *HEX4_BASE = GPIO_VIRTUAL + 0x20; // HEX4: Digito da dezena
    volatile unsigned int *HEX3_BASE = GPIO_VIRTUAL + 0x30; // HEX3: Digito da unidade

    volatile unsigned int *HEX2_BASE = GPIO_VIRTUAL + 0x40; // HEX2: Digito apagado
    volatile unsigned int *HEX1_BASE = GPIO_VIRTUAL + 0x50; // HEX1: Digito apagado
    volatile unsigned int *HEX0_BASE = GPIO_VIRTUAL + 0x60; // HEX0: Digito apagado

    // Digitos que não seram utilizados são apagados
    *HEX5_BASE = 0xff;

    *HEX2_BASE = 0xff;

    *HEX1_BASE = 0xff;

    *HEX0_BASE = 0xff;

    int *decomposto = return_decomposto(pontos);

    // Definindo a unidade
    if (decomposto[1] == 0)
    { // Definindo unidade zerada

        *HEX3_BASE = 0xc0;
        // sleep(1);
    }
    else if (decomposto[1] == 1)
    { // Definindo unidade 1

        *HEX3_BASE = 0xf9;
        // sleep(1);
    }
    else if (decomposto[1] == 2)
    { // Definindo unidade 2

        *HEX3_BASE = 0x1a4;
        // sleep(1);
    }
    else if (decomposto[1] == 3)
    { // Definindo unidade 3

        *HEX3_BASE = 0xb0;
        // sleep(1);
    }
    else if (decomposto[1] == 4)
    { // Definindo unidade 4

        *HEX3_BASE = 0x99;
        // sleep(1);
    }
    else if (decomposto[1] == 5)
    { // Definindo unidade 5

        *HEX3_BASE = 0x92;
        // sleep(1);
    }
    else if (decomposto[1] == 6)
    { // Definindo unidade 6

        *HEX3_BASE = 0x82;
        // sleep(1);
    }
    else if (decomposto[1] == 7)
    { // Definindo unidade 7

        *HEX3_BASE = 0xf8;
        // sleep(1);
    }
    else if (decomposto[1] == 8)
    { // Definindo unidade 8

        *HEX3_BASE = 0x80;
        // sleep(1);
    }
    else if (decomposto[1] == 9)
    { // Definindo unidade 9

        *HEX3_BASE = 0x90;
        // sleep(1);
    }
    else
    {

        *HEX3_BASE = 0xc0;
        // sleep(1);
    }

    /*--------------------------------------------------------------------------------------------------------------------*/

    // Definindo a dezena:

    if (decomposto[0] == 0)
    { // Definindo dezena zerada

        *HEX4_BASE = 0xc0;
        // sleep(1);
    }
    else if (decomposto[0] == 1)
    { // Definindo dezena 10

        *HEX4_BASE = 0xf9;
        // sleep(1);
    }
    else if (decomposto[0] == 2)
    { // Definindo dezena 20

        *HEX4_BASE = 0x1a4;
        // sleep(1);
    }
    else if (decomposto[0] == 3)
    { // Definindo dezena 30

        *HEX4_BASE = 0xb0;
        // sleep(1);
    }
    else if (decomposto[0] == 4)
    { // Definindo dezena 40

        *HEX4_BASE = 0x99;
        // sleep(1);
    }
    else if (decomposto[0] == 5)
    { // Definindo dezena 50

        *HEX4_BASE = 0x92;
        // sleep(1);
    }
    else if (decomposto[0] == 6)
    { // Definindo dezena 60

        *HEX4_BASE = 0x82;
        // sleep(1);
    }
    else if (decomposto[0] == 7)
    { // Definindo dezena 70

        *HEX4_BASE = 0xf8;
        // sleep(1);
    }
    else if (decomposto[0] == 8)
    { // Definindo dezena 80

        *HEX4_BASE = 0x80;
        // sleep(1);
    }
    else if (decomposto[0] == 9)
    { // Definindo dezena 90

        *HEX4_BASE = 0x90;
        // sleep(1);
    }
    else
    {

        *HEX4_BASE = 0xc0;
        // sleep(1);
    }

    /*--------------------------------------------------------------------------------------------------------------------*/

    return 0;
}
