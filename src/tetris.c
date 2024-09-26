#include "stdint.h" // Para Inteiros de 8 e 16 bits
#include <stdio.h> // Entrada e Saída, como "printf"
#include <stdlib.h>
#include <intelfpgaup/video.h> // Exibir no Monitor
#include "fcntl.h" // Para Manipulação de Arquivos
#include "unistd.h" // Para Fechar Arquivos
#include "time.h" // Função "sleep"
#include "sys/mman.h"// Para Mapear Endereço Físico em Endereço Virtual
#include <intelfpgaup/KEY.h>

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

//Tamanho do jogo 20x10
#define LINHA 20
#define COLUNA 10
#define TEMPO_QUEDA 300
#define TAMANHO_DO_BLOCO 8

int pontuacao = 0; //
int posicaoPeca_coluna=3; //Coluna no tabuleiro, posição horizontal
int posicaoPeca_linha=0; // linha no tabuleiro, posição vertical
int indice_peca = 0;
int borda[LINHA][COLUNA]= {0};  //Matriz que representa o espaço do jogo. O zero significa espaço em branco, matriz toda de zeros.
int pausa = 1;
//Todos os tipos de cores
short int video_color[] = {video_WHITE, video_YELLOW, video_RED,
video_GREEN, video_BLUE, video_CYAN, video_MAGENTA, video_GREY,
video_PINK, video_ORANGE};

//Definção de 6 peças 
int pecas[6][4][4] = {
    { // formato - barra
        {0,0,0,0},
        {1,1,1,1},
   
    },
    { // formato [] quadrado
        {0,1,1,0},
        {0,1,1,0},

    },
    { // formato de t invertido 
        {0,1,0,0},
        {1,1,1,0},

    },
    { // formato de L ao contrário
        {0,0,0,1},
        {0,1,1,1},

    },
    { // formato de z 
        {1,1,0},
        {0,1,1}
    },
    { // formato de N
        {0,1},
        {1,1},
        {1,0},
    }

};

// Função responsável por desenhar bloco x- coluna, y- linha 
void desenharBloco(int x, int y, short int cor){
    //parametro x e é  as coordenadas da posição do bloco no tabuleiro
    //Convertendo posição do tabuleiro em pixels 
    //Se o bloco tem 12 pixels de largura, a posição 2 no tabuleiro vira 24 pixels na tela (2* 12 =24)
    int x1= x * TAMANHO_DO_BLOCO;
    int y1= y * TAMANHO_DO_BLOCO;

    int x2= x1 + TAMANHO_DO_BLOCO; 
    int y2= y1 + TAMANHO_DO_BLOCO;
    video_box(x1,y1,x2,y2,cor);
}

void desenharBlocoTetris(int x, int y, short int cor){
    int x1= x * TAMANHO_DO_BLOCO + 1;
    int y1= y * TAMANHO_DO_BLOCO + 1;
    
    int x2= (x+1)* TAMANHO_DO_BLOCO - 1;
    int y2= (y+1)* TAMANHO_DO_BLOCO - 1;

    video_box(x1,y1,x2,y2,cor);
}
//Desenho do campo de jogo
void desenharCampo(){
    //Limpa a tela primeiro
    video_clear();

    
    //loop para  percorrer todas as linhas para desenhar as bordas laterais
    for(int y=0; y <= LINHA; y++){
        desenharBloco(0,y,video_ORANGE); //a borda esquerda
        desenharBloco(COLUNA+1,y,video_ORANGE); // a borda direita 
    }   

    //percorre todas as colunas para desenhar a borda base
    for(int x=0;x<=COLUNA+1; x++){
        desenharBloco(x,LINHA,video_ORANGE); //Lembrando que LINHA é a ultima linha 
    }

    //laço para percorrer toda a matriz para fazer os blocos fixo nela
    for (int y=0; y< LINHA; y++){
        for (int x=0; x<COLUNA; x++){
            if(borda[y][x]){  // No caso, se tiver um valor positivo, ou seja, se tiver um bloco 
                //Verificando se no ponto atual na matriz o valor é 1, ou seja, tem peça
                desenharBloco(x+1,y,video_GREY);
            }
        }
    }\

    //Desenha o bloco da peça na posição correta do tabuleiro, passando por toda a matriz do bloco
    for(int i=0; i<4;i++){
        for(int j=0; j<4;j++){
            if(pecas[indice_peca][i][j]){ //Se tiver um valor positivo, eu desenho
                //verificar que se na posição atual tem algum bloco, se sim ele precisa ser desenhado 
                desenharBlocoTetris(posicaoPeca_coluna+ j +1, posicaoPeca_linha + i, video_color[indice_peca]);
            }
        }
    
    
    }

    //Mostrando a potuação
    char texto_da_pontuacao[20];
    sprintf(texto_da_pontuacao,"Pontuacao %d", pontuacao); //Fanzedo a string da texto_da_pontuacao
    video_text(40, 2, texto_da_pontuacao);  // mostrando na tela
    video_show();

}

//Verifica se a posição (i, j) da peça está ocupada, x - coluna, y - linha
int colisao(int x, int y, int indice_peca_atual) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {  // Percorre todos os blocos da peça
            if (pecas[indice_peca_atual][i][j]) {
                // Verifica colisão com as bordas laterais e inferiores
                if (y + i >= LINHA || x + j >= COLUNA || x + j < 0 || borda[y + i][x + j]) {
                    return 1;  // Retorna 1 se houver colisão
                }
            }
        }
    }
    return 0;  // Sem colisão
}


// Deixar a peça no tabuleiro
void fixarPeca(){
    //Um laço que percorre as linhas da peça atual
    //Verifica se a posição atual da peça contém um bloco
    for(int i=0; i<4; i++){
        for(int j =0; j<4; j++){
            if(pecas[indice_peca][i][j]){
                // Aqui eu só coloco na matriz borda os valores positivos onde a peça estava, ou seja, valores fixos
                borda[posicaoPeca_linha + i][posicaoPeca_coluna+ j]=1; //atualizaçao da matriz borda, marcando a posição da peça no tabuleiro
            }
        }
    }
}


void checarLinhas(){
    //Percorre todas as linhas do tabuleiro, ver tá completa
    for(int y = 0; y < LINHA; y++){
        //verificando linhas
        int linha_completa = 1;
        for(int x =0; x< COLUNA; x++){
            // se eu  achar na coluna um só zero eu não preciso verificar mais as outras colunas 
            if(borda[y][x] == 0){
                linha_completa=0;
                break;
            }
        }

        //Se tiver alguma linha completa, a variavel linha_completa permanecerá com o valor 1
        if(linha_completa){
            //Se a linha está completa, remove ela e atualiza pontuação.
            pontuacao+=1;
            
        
            //Descendo as coisas que tão em cima
            for(int i = y; i > 0; i--){
                for(int j = 0; j < COLUNA; j++){
                    borda[i][j] = borda[i-1][j];     
                }
            }

            for ( int j = 0; j < COLUNA; j++){ //Removendo blocos da linha, coluna por coluna
                borda[0][j]=0;
            }
            y--; //Reajusta o valor de y(linha) para verificar novamente a linha atual, já que ela foi substituída.

        }
    }
}

//Controlar a queda
int descida() {
    if(!colisao(posicaoPeca_coluna,posicaoPeca_linha+1, indice_peca)){
        //Verifica se a peça pode descer uma linha sem colidir com outra peça ou a base
        posicaoPeca_linha++; // Move o bloco para baixo
        return 0; //peça não precisa ser fixada ainda
    }
    else{ //Se não eu fixo a peça lá mesmo e logo após eu verifico se completou a linha
        fixarPeca();
        checarLinhas();
        return 1;
    }
}

//Função que move a peça -1 esquerda,+1 direita
void mover(int lado){
    int x= posicaoPeca_coluna+ lado;

    //verificar colisão com a borda
    if( !colisao(x,posicaoPeca_linha,indice_peca)){
        posicaoPeca_coluna= x; //Se não tiver colisão com a borda lateral a peça move
    }

    //Se  tiver colisão com a borda lateral a peça fica no mesmo lugar
}

// A PARTIR DESSA PARTE, COMEÇAM AS FUNÇÕES DO ACELERÔMETRO: //

/*
Acessando a memória física do sistema
Retorna "-1", caso ocorra um erro na abertura do arquivo
*/
int open_memory(void)
{
    int fd = open("/dev/mem", O_RDWR | O_SYNC); // "fd": file descriptor;
    return fd;
}

/*
Usada para habilitar a comunicação do ADXL345 com o I2C0,
através do bloco de multiplexadores no módulo "sysmgr",
usando os pinos HPS
*/
void enable_I2C0_HPS(int fd)
{
    size_t virtual_address_size = 4096; // Tamanho do Endereço Virtual

    // Endereços dos Registradores do Bloco de Multiplexadores do Módulo "sysmgr":
    off_t SYSMGR = 0xFFD08000; // Endereço Físico Base do SYSMGR
    void * SYSMGR_VIRTUAL = mmap (NULL, virtual_address_size, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, SYSMGR); // Endereço Virtual Base do SYSMGR

    // Definindo os Endereços de Outros Registradores do Bloco de Multiplexadores, de Acordo com o Offset:
    volatile unsigned int * SYSMGR_GENERALIO7 = SYSMGR_VIRTUAL + 0x49C;
    volatile unsigned int * SYSMGR_GENERALIO8 = SYSMGR_VIRTUAL + 0x4A0;
    volatile unsigned int * SYSMGR_I2C0USEFPGA = SYSMGR_VIRTUAL + 0x704;

    // Selecionando os Valores das Seguintes Entradas:
    * SYSMGR_GENERALIO7 = 0x01;
    * SYSMGR_GENERALIO8 = 0x01;
    * SYSMGR_I2C0USEFPGA = 0x00;
}

void setting_I2C0_ADXL345(void * I2C0_BASE_VIRTUAL)
{
    // Endereços dos Registradores do Módulo "I2C0":
    volatile unsigned int * I2C0_CONTROL = I2C0_BASE_VIRTUAL + 0x00; // Para ativar o modo "Master"
    volatile unsigned int * I2C0_TARGET = I2C0_BASE_VIRTUAL + 0x04; // Para selecionar o "Slave"
    volatile unsigned int * I2C0_ENABLE = I2C0_BASE_VIRTUAL + 0x6C; // Para ativar e desativar o I2C0
    volatile unsigned int * I2C0_ENABLE_STATUS = I2C0_BASE_VIRTUAL + 0x9C; // Para verificar se há alguma transmissão ativa

    /*
    Encerrando transmissões ativas e desativando o I2C0,
    para permitir a modificação dos registradores "ic_con"
    e "ic_tar":
    */
    * I2C0_ENABLE = 0x02;

    /*
    Analisa o bit 0, "activity", do registrador "ic_status",
    para verificar se há alguma atividade no I2C0. Então, só
    irá sair do loop quando essa possível atividade terminar.
    */
    while (((* I2C0_ENABLE_STATUS) & 0x1) == 1){}

    /*
    Configuração do registrador "ic_con":
        Modo "Slave" Desligado;
        "Restart Master" Ligado;
        Endereço de 7-bits para o Master;
        Endereço de 7-bits para o Slave;
        Modo de Alta Velocidade;
        Modo "Master" Ligado.
    */
    * I2C0_CONTROL = 0x65;

    // Selecionando o endereço base do ADXL345 no registrador "ic_tar":
    * I2C0_TARGET = 0x53;

    // Ativando o módulo "I2C0":
    * I2C0_ENABLE = 0x01;

    // Verificando se o módulo "I2C0" foi ativado:
    while (((* I2C0_ENABLE_STATUS) & 0x1) == 0){}
}

/*
Configurações iniciais dos sensores do ADXL345:
No Registrador BW_RATE - 0x2C:
	Modo de Operação: Normal
	Rate: Taxa de Dados de Saída: 200 Hz | Largura de Banda: 100 Hz
	Configuração Resultante: 0x0B
No Registrador POWER_CTL - 0x2D:
	Medição: Habilitada
	Frequência de Leitura: 4Hz (4 vezes por segundo - 250ms)
	Configuração Resultante: 0x0D
No Registrador DATA_FORMAT - 0x31:
	Modo de Resolução: Alta
	Intervalo de Aceleração (G-Range): +-16g.
	Configuração Resultante: 0x07
*/
void setting_ADXL345(volatile unsigned int * I2C0_DATA)
{
    // Configurando o THRESH_ACT:
    * I2C0_DATA = THRESH_ACT + 0x400;
    * I2C0_DATA = 0x04;

    // Configurando o THRESH_INACT:
    * I2C0_DATA = THRESH_INACT + 0x400;
    * I2C0_DATA = 0x02;

    // Configurando o TIME_INACT:
    * I2C0_DATA = TIME_INACT + 0x400;
    * I2C0_DATA = 0x02;

    // Configurando o ACT_INACT_CTL:
    * I2C0_DATA = ACT_INACT_CTL + 0x400;
    * I2C0_DATA = 0xFF;

    // Configurando o INT_ENABLE:
    * I2C0_DATA = INT_ENABLE + 0x400;
    * I2C0_DATA = 0x04 | 0x02;

    // Configurando o BW_RATE:
    * I2C0_DATA = BW_RATE + 0x400;
    * I2C0_DATA = 0x0B;

    // Configurando o POWER_CTL:
    * I2C0_DATA = POWER_CTL + 0x400;
    * I2C0_DATA = 0x0D;

    // Configurando o DATA_FORMAT:
    * I2C0_DATA = DATA_FORMAT + 0x400;
    * I2C0_DATA = 0x07;
}

// Ler e retornar valores do Eixo X do ADXL345:
int16_t lerEixoX(volatile unsigned int * I2C0_DATA, volatile unsigned int * I2C0_RXFLR)
{
    // Lendo os dados do byte menos significativo do Eixo X:
    * I2C0_DATA = DATAX0 + 0x400;
    * I2C0_DATA = 0x100; // "0x100", envia "1" no bit "8", "cmd", do registrador, ativando a leitura.
    while (* I2C0_RXFLR == 0) {} // Mantém no loop, enquanto o buffer do registrador "ic_rxflr" estiver vazio.
    uint8_t LSB_EixoX = * I2C0_DATA;
    
    // Lendo os dados do byte mais significativo do Eixo X:
    * I2C0_DATA = DATAX1 + 0x400;
    * I2C0_DATA = 0x100;
    while (* I2C0_RXFLR == 0) {}
    uint8_t MSB_EixoX = * I2C0_DATA;
    
    // Armazenando os dois bytes do Eixo X em um inteiro de 16 bits
    // Primeiro o Byte Mais Signficativo e Depois o Byte Menos Significativo
    int16_t EixoX = (MSB_EixoX << 8) | LSB_EixoX;

    return EixoX; // Retornando o valor de leitura do Eixo X
}

// Função Principal:
int main(){
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
    indice_peca= rand() % 6; //Escolhendo uma peça aleatória

    int16_t eixo_x; // Variável de 16 bits (2 Bytes) para Armazenar os Valores do Eixo X

    while (1){

        int botao = KEY_read();

        // Verificando se KEY0 foi pressionado 
        if (botao & 0x1) {  
            pausa = !pausa;  // Alterna o valores a cada aperto
            //Se pausa for 0 significa que tá pausado, inicia com o valor de 1
        }
        // Para Fazer a Medição de Valores do Eixo X:
        eixo_x = lerEixoX(I2C0_DATA, I2C0_RXFLR);

        // Mover Peças para Esquerda ou Direita:
        if (eixo_x > 100 && pausa)
        {
            mover(1);
        }
        else if (eixo_x < -100 && pausa)
        {
            mover(-1);
        }

        if(descida()&& pausa){
            indice_peca = rand() % 6; //Pegando o indice da peça
            posicaoPeca_coluna= 4; 
            posicaoPeca_linha = 0;

            //Verifica se ouve colisão 
            if(colisao(posicaoPeca_coluna,posicaoPeca_linha,indice_peca)){

                char texto_do_gameover[20];
                video_erase();
                sprintf(texto_do_gameover,"GAME-OVER");
                video_text(40,20,texto_do_gameover);                
                break;
            }
        }

        desenharCampo();
        //Talvez usleep não pegue
        usleep(TEMPO_QUEDA * 1000); // Controla a velocidade da queda
    }

    video_close();
    close(fd); // Fechando o Arquivo de Memória
    return 0;
}
