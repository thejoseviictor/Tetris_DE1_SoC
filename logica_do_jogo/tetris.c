#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <intelfpgaup/video.h> 
#include <intelfpgaup/KEY.h>

//Tamanho do jogo 20x10
#define LINHA 20
#define COLUNA 10
#define TEMPO_QUEDA 100
#define TAMANHO_DO_BLOCO 10

int pontuacao = 0; //
int posicaoPeca_coluna=4; //Coluna no tabuleiro, posição horizontal
int posicaoPeca_linha=0; // linha no tabuleiro, posição vertical
int indice_peca = 0;
int borda[LINHA][COLUNA]= {0};  //Matriz que representa o espaço do jogo. O zero significa espaço em branco, matriz toda de zeros.

//Todos os tipos de cores
short int video_color[] = {video_WHITE, video_YELLOW, video_RED,
video_GREEN, video_BLUE, video_CYAN, video_MAGENTA, video_GREY,
video_PINK, video_ORANGE};


//Definção de 6 peças 
int pecas[6][4][4] = {
    { // formato - barra
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    },
    { // formato [] quadrado
        {0,0,0,0},
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0}
    },
    { // formato de t invertido 
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    { // formato de L ao contrário
        {0,0,0,1},
        {0,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    },
    { // formato de z 
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    { // formato de z ao contrário
        {0,0,1,1},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
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
                desenharBloco(posicaoPeca_coluna+ j +1, posicaoPeca_linha + i, video_color[indice_peca]);
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
int colisao(int x, int y, int indice_peca_atual){
    for(int i =0; i<4; i ++){
        for (int j =0; j <4; j++){  //Pecorre todos os blocos da peça
            //Se encontrar qualquer colisão, ou na base ou em algum bloco vai retornar 1
            if(pecas[indice_peca_atual][i][j]&& (y+i >= LINHA || x+j >= COLUNA || x +j <0 || borda[y+1][x+j])){
                return 1;
            }
        }
    }
    return 0;
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

int main(){
    video_open();
    video_erase();
    //key_open();
    srand(time(0));
    indice_peca= rand() % 6; //Escolhendo uma peça aleatória

    while (1){

        /*int botao = key_read();
        
        //Verificando se algum botão foi pressionado
        if(botao & 0x1){
            mover(-1);
        } else if(botao & 0x2){
            mover(1);
        }
        */

        if(descida()){
            indice_peca = rand() % 6; //Pegando o indice da peça
            posicaoPeca_coluna= 4; 
            posicaoPeca_linha = 0;

            //Verifica se ouve colisão 
            if(colisao(posicaoPeca_coluna,posicaoPeca_linha,indice_peca)){
                printf("Game Over!");
                break;
            }
        }

        desenharCampo();
        //Talvez usleep não pegue
        usleep(TEMPO_QUEDA * 1000); // Controla a velocidade da queda
    }

video_close();
//key_close();
return 0;

}

