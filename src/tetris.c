#include "stdint.h" // Para Inteiros de 8 e 16 bits
#include <stdio.h> // Entrada e Saída, como "printf"
#include <stdlib.h> // Funções "rand" e "srand"
#include <intelfpgaup/video.h> // Exibir no Monitor
#include "fcntl.h" // Para Manipulação de Arquivos
#include "unistd.h" // Para Fechar Arquivos
#include "time.h" // Função "sleep"
#include "sys/mman.h" // Para Mapear Endereço Físico em Endereço Virtual
#include <intelfpgaup/KEY.h> // Para Usar os Botões da Placa
#include "ADXL345.h" // Funções do Acelerômetro
#include "jogabilidade.h" // Funções da Jogabilidade

// Função Principal:
int main(){
    // Declarando as Variáveis:
    int pausa = 1;
    int posicaoPeca_coluna = 3; // Coluna no tabuleiro, posição horizontal
    int posicaoPeca_linha = 0; // linha no tabuleiro, posição vertical
    int indice_peca = 0;

    // Iniciando o Acelerômetro:
    int fd = open_memory(); // Abrindo o Arquivo de Memória
    enable_I2C0_HPS(fd); // Ativando as Entradas do Multiplexador Que Conectam o ADXL345 ao I2C0

    size_t virtual_address_size = 4096; // Tamanho dos Endereços Virtuais
    
    // Endereços dos Registradores do Módulo "I2C0":
    off_t I2C0_BASE = 0xFFC04000; // Endereço Físico Base do I2C0
    void * I2C0_BASE_VIRTUAL = mmap (NULL, virtual_address_size, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, I2C0_BASE); // Endereço Virtual Base do I2C0
    volatile unsigned int * I2C0_DATA = I2C0_BASE_VIRTUAL + 0x10; // Para Escrever ou Ler em um "Slave"
    volatile unsigned int * I2C0_RXFLR = I2C0_BASE_VIRTUAL + 0x78; // Para Verificar se Existem Dados a Receber

    setting_I2C0_ADXL345(I2C0_BASE_VIRTUAL); // Configurando o Barramento I2C0
    setting_ADXL345(I2C0_DATA); // Configurando os Sensores do Acelerômetro

    // Inciando o Vídeo e o Tetris:
    video_open();
    video_erase();
    KEY_open();

    //key_open();
    srand(time(0));
    indice_peca = rand() % 6; //Escolhendo uma peça aleatória

    int16_t eixo_x; // Variável de 16 bits (2 Bytes) para Armazenar os Valores do Eixo X

    while (1){
        int botao;
        
        KEY_read(&botao);

        // Verificando se KEY0 foi pressionado 
        if (botao & 0x1) {  
            pausa = !pausa;  // Alterna o valores a cada aperto
            //Se pausa for 0 significa que tá pausado, inicia com o valor de 1
        }
        // Para Fazer a Medição de Valores do Eixo X:
        eixo_x = lerEixoX(I2C0_DATA, I2C0_RXFLR);

        // Mover Peças para Esquerda ou Direita:
        if (eixo_x > 100)
        {
            mover(1, &posicaoPeca_coluna, posicaoPeca_linha, indice_peca);
        }
        else if (eixo_x < -100)
        {
            mover(-1, &posicaoPeca_coluna, posicaoPeca_linha, indice_peca);
        }

        if(descida(posicaoPeca_coluna, &posicaoPeca_linha, indice_peca) && pausa){
            indice_peca = rand() % 6; //Pegando o indice da peça
            posicaoPeca_coluna= 4; 
            posicaoPeca_linha = 0;

            //Verifica se ouve colisão 
            if(colisao(posicaoPeca_coluna, posicaoPeca_linha, indice_peca)){

                char texto_do_gameover[20];
                video_erase();
                sprintf(texto_do_gameover,"GAME-OVER");
                video_text(40,20,texto_do_gameover);                
                break;
            }
        }

        desenharCampo(posicaoPeca_coluna, posicaoPeca_linha, indice_peca);
        //Talvez usleep não pegue
        usleep(TEMPO_QUEDA * 1000); // Controla a velocidade da queda
    }

    video_close();
    close(fd); // Fechando o Arquivo de Memória
    return 0;
}
