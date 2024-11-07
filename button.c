#include "stdint.h" // Para Inteiros de 8 e 16 bits
#include <stdio.h>  // Entrada e Saída, como "printf"
#include <stdlib.h>
#include "fcntl.h"    // Para Manipulação de Arquivos
#include "unistd.h"   // Para Fechar Arquivos
#include "time.h"     // Função "sleep"
#include "sys/mman.h" // Para Mapear Endereço Físico em Endereço Virtual
#include <time.h>

int button()
{
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    size_t GPIO0_SPAN = 4096;
    off_t GPIO0_OFST = 0xff200000;
    void *GPIO_VIRTUAL = mmap(NULL, GPIO0_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO0_OFST);

    volatile unsigned int *KEYS_BASE = GPIO_VIRTUAL + 0x0; // botoes

    int botao;

    while (1)
    {

        if (*KEYS_BASE == 0x0)
        {

            print("Botões desligados");
            botao = 0;
        }
        else if (*KEYS_BASE == 0x1)
        {

            print("Botão 0 ligado!");
            botao = 1;
        }

        else if (*KEYS_BASE == 0x2)
        {

            print("Botão 1 ligado!");
            botao = 1;
        }

        else if (*KEYS_BASE == 0x4)
        {

            print("Botão 2 ligado!");
            botao = 1;
        }

        else if (*KEYS_BASE == 0x8)
        {

            print("Botão 3 ligado!");
            botao = 1;
        }

        else
        {

            print("Botões desligados");
            botao = 0;
        }
    }

    return botao;
}