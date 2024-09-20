#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <intelfpgaup/video.h> 


//Tamanho do jogo 20x10
#define ALTURA 20
#define LARGURA 10
#define TEMPO_QUEDA 400
#define TEMPO_RESPOSTA 40
#define TAMANHO_DO_BLOCO 12

int pontuacao = 0;
int posicao_da_peca_no_tabuleiroX=3; //Coluna no tabuleiro, posição horizontal
int posicao_da_peca_no_tabuleiroY=0; // linha no tabuleiro, posição vertical
int indice_peca = 0;
int borda[ALTURA][LARGURA]= {0};  //Matriz que representa o espaço do jogo. O zero significa espaço em branco

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

// Função responsável por desenhar bloco
void desenharBloco(int x, int y, short int cor){
    //parametro x e é  as coordenadas da posição do bloco no tabuleiro
    //x é uma posição no tabuleiro multiplicando por tamanho do bloco para obter a posição na tela em pixels. 
    int x1= x * TAMANHO_DO_BLOCO;
    int y1= y * TAMANHO_DO_BLOCO;
    //A largura do bloco é somada à posição inicial (x1) para obter o limite direito do bloco.
    int x2= x1 + TAMANHO_DO_BLOCO;
    int y2= y1 + TAMANHO_DO_BLOCO;
    video_box(x1,y1,x2,y2,cor);
}

//Desenho do campo de jogo
void desenharCampo(){
    //Limpa a tela primeiro
    video_clear();


    //loop para  percorrer todas as linhas para desenhar as bordas laterais
    for(int y=0; y <= ALTURA; y++){
        desenharBloco(0,y,video_ORANGE); //a borda esquerda
        desenharBloco(LARGURA+1,y,video_ORANGE); // a borda direita 
    }   

    //percorre todas as colunas para desenhar a borda base
    for(int x=0;x<=LARGURA+1; x++){
        desenharBloco(x,ALTURA,video_ORANGE);
    }

    //laço para percorrer cada linha para ver se a bloco fixo nela
    for (int y=0; y< ALTURA; y++){
        for (int x=0; x<LARGURA; x++){
            if(borda[y][x]){ 
                //Verificando se no ponto atual na matriz o valor é 1, ou seja, tem peça
                desenharBloco(x+1,y,video_GREY);
            }
        }
    }

    //Desenha o bloco da peça na posição correta do tabuleiro, ajustando as coordenadas da peça 
    for(int i=0; i<4;i++){
        for(int j=0; j<4;j++){
            if(pecas[indice_peca][i][j]){
                //verificar que se na posição atual tem algum bloco, se sim ele precisa ser desenhado 
                desenharBloco(posicao_da_peca_no_tabuleiroX + j +1, posicao_da_peca_no_tabuleiroY + i, video_color[indice_peca]);
            }
        }
    }

    video_show();

}

int descida() {
    if(!colisao(posicao_da_peca_no_tabuleiroX,posicao_da_peca_no_tabuleiroY+1, indice_peca)){
        //Verifica se a peça pode descer uma linha sem colidir com outra peça ou a base
        posicao_da_peca_no_tabuleiroY++; // Move o bloco para baixo
        return 0; //peça não precisa ser fixada ainda
    }
    else{
        fixarPeca();
        checarLinhas();
        return 1;
    }
}


void fixarPeca(){
    //Um laço que percorre as linhas da peça atual
    //Verifica se a posição atual da peça contém um bloco
    for(int i=0; i<4; i++){
        for(int j =0; j<4; j++){
            if(pecas[indice_peca][i][j]){
                borda[posicao_da_peca_no_tabuleiroY + i][posicao_da_peca_no_tabuleiroX + j]=1; //atualizaçao da matriz borda, marcando a posição da peça no tabuleiro
            }
        }
    }
}
//Verifica se a posição (i, j) da peça está ocupada
int colisao(int x, int y, int indice_peca_atual){
    for(int i =0; i<4; i ++){
        for (int j =0; j <4; j++1){
            if(peca[indice_peca_atual][i][j]&& (y+i >= ALTURA || x+j >= LARGURA || x +j <0 || borda[y+1][x+j])){
                return 1;
            }
        }
    }
    return 0;
}


void checarLinhas(){
    //Percorre todas as linhas do tabuleiro, ver tá completa
    for(int y = 0; y < ALTURA; y++){
        int linha_completa = 1;
        for(int x =0;; x< LARGURA; x++){
            if(borda[y][x] == 0){
                linha_completa=0
                break
            }
        }

        if(linha_completa){
            //Se a linha está completa, continua com a remoção dela.
            pontuacao += 1;:
            pontuacao+=1;
            for(int i = y; i > 0; i--){
                for(int j = 0; j < LARGURA; j++){
                    borda[i][j] = borda[i-1][j];     
                }
            }

            for ( int j = 0; j < LARGURA; j++){
                borda[0][j]=0;
            }
            y--; //Reajusta o valor de y para verificar novamente a linha atual, já que ela foi substituída.

        }
    }
}


void mover(int lado){
    int x= x+ lado;

    if( !colisao(x,posicao_da_peca_no_tabuleiroY,indice_peca)){
        posicao_da_peca_no_tabuleiroX = x
    }
}

int main(){
    video_open();
    srand(time(0));
    indice_peca= rand() % 6;

    while (1){

        if(descida()){
            indice_peca = rand() % 6;
            posicao_da_peca_no_tabuleiroX = 3;
            posicao_da_peca_no_tabuleiroY = 0;

            if(colisao(posicao_da_peca_no_tabuleiroX,posicao_da_peca_no_tabuleiroY,indice_peca)){
                printf("Game Over!")
            }
        }

        desenharCampo()
    }

video_close();
return 0;

}

