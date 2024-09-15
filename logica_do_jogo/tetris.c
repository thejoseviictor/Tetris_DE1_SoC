#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <intelfpgaup/video.h> 

#define ALTURA 20
#define LARGURA 10
#define TEMPO_QUEDA 400
#define TEMPO_RESPOSTA 40
#define TAMANHO_DO_BLOCO 20

int pontuacao = 0;
int posicao_da_peca_no_tabuleiroX=3;
int posicao_da_peca_no_tabuleiroY=0;
int indice_peca = 0;
int borda[ALTURA][LARGURA]= {0};


unsigned int video_color[] = {video_WHITE, video_YELLOW, video_RED,
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

//Desenho do bloco no vga, no qual recebe o eixo x e o eixo y para saber exatamente onde colocar o bloco
void desenharBloco(int x, int y, unsigned *cor){

    int x1= x * TAMANHO_DO_BLOCO;
    int y1= y * TAMANHO_DO_BLOCO;
    int x2= x1 + TAMANHO_DO_BLOCO;
    int y2= y1 + TAMANHO_DO_BLOCO;
    video_box(x1,y1,x2,y2,cor);
}

//Desenho do campo de jogo
void desenharCampo(){
    video_clear();


    //loop para desenhar cada bloco da borda
    for(int y=0; y <= ALTURA; y++){
        desenharBloco(0,y,video_ORANGE); //LADO ESQUERDO
        desenharBloco(LARGURA+1,y,video_ORANGE); //LADO DIREITO
    }   //obs.: pode ser largura -1 

    //fazer a base
    for(int x=0;x<=LARGURA+1; x++){
        desenharBloco(x,ALTURA,video_ORANGE);
    }

    //fazer a peça que está fixa
    for (int y=0; y< ALTURA; y++){
        for (int x=0; x<LARGURA; x++){
            if(borda[y][x]){
                desenharBloco(x+1,y,video_GREY);
            }
        }
    }

    //fazer a peça que está caindo
    for(int i=0; i<4;i++){
        for(int j=0; j<4;j++){
            if(pecas[indice_peca][i][j]){
                desenharBloco(posicao_da_peca_no_tabuleiroX + j +1, posicao_da_peca_no_tabuleiroY + i, video_color[indice_peca]);
            }
        }
    }

    video_show();

}

void descida() {
    posicao_da_peca_no_tabuleiroY++; // Move o bloco para baixo
}


int main(){
    video_open();
    srand(time(0));
    indice_peca= rand() % 6;

    while (1){

        desenharCampo();
        usleep(TEMPO_QUEDA * 1000);
        descida();
        
        if (posicao_da_peca_no_tabuleiroY >= ALTURA - 4) {
            indice_peca = rand() % 6;
            posicao_da_peca_no_tabuleiroX = 3;
            posicao_da_peca_no_tabuleiroY = 0;
        }
    }

video_close();
return 0;

}

