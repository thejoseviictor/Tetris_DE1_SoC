// Funções de Jogabilidade do Tetris

#include "jogabilidade.h"

int pontuacao = 0;
int borda[LINHA][COLUNA]= {0}; // Matriz que representa o espaço do jogo. O zero significa espaço em branco, matriz toda de zeros.

//Todos os tipos de cores:
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
void desenharCampo(int posicaoPeca_coluna, int posicaoPeca_linha, int indice_peca){
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
void fixarPeca(int posicaoPeca_coluna, int posicaoPeca_linha, int indice_peca){
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

// Controlar a queda
// Passei os Parametros como Ponteiro para Permitir a Escrita
int descida(int posicaoPeca_coluna, int * posicaoPeca_linha, int indice_peca) {
    if(!colisao(posicaoPeca_coluna, (* posicaoPeca_linha) + 1, indice_peca)){
        //Verifica se a peça pode descer uma linha sem colidir com outra peça ou a base
        (* posicaoPeca_linha) ++; // Move o bloco para baixo
        return 0; //peça não precisa ser fixada ainda
    }
    else{ //Se não eu fixo a peça lá mesmo e logo após eu verifico se completou a linha
        fixarPeca(posicaoPeca_coluna, * posicaoPeca_linha, indice_peca);
        checarLinhas();
        return 1;
    }
}

// Função que move a peça -1 esquerda,+1 direita
// Passei os Parametros como Ponteiro para Permitir a Escrita
void mover(int lado, int * posicaoPeca_coluna, int posicaoPeca_linha, int indice_peca){
    int x = (* posicaoPeca_coluna) + lado;

    //verificar colisão com a borda
    if( !colisao(x, posicaoPeca_linha, indice_peca)){
        (* posicaoPeca_coluna) = x; //Se não tiver colisão com a borda lateral a peça move
    }

    //Se  tiver colisão com a borda lateral a peça fica no mesmo lugar
}