#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <intelfpgaup/video.h> 

// Tamanho do jogo 20x10
#define ALTURA 20
#define LARGURA 10
#define TEMPO_QUEDA 400
#define TEMPO_RESPOSTA 40
#define TAMANHO_DO_BLOCO 12

int pontuacao = 0;
int posicao_da_peca_no_tabuleiroX = 3; // Coluna no tabuleiro, posição horizontal
int posicao_da_peca_no_tabuleiroY = 0; // Linha no tabuleiro, posição vertical
int indice_peca = 0;
int proxima_peca; // Próxima peça aleatória
int borda[ALTURA][LARGURA] = {0};  // Matriz que representa o espaço do jogo

// Todos os tipos de cores
short int video_color[] = {video_WHITE, video_YELLOW, video_RED,
                            video_GREEN, video_BLUE, video_CYAN,
                            video_MAGENTA, video_GREY, video_PINK, video_ORANGE};

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

// Função para desenhar um bloco com margem
void desenharBloco(int x, int y, short int cor) {
    int margem = 2; 
    int x1 = x * TAMANHO_DO_BLOCO + margem; // Canto superior esquerdo x
    int y1 = y * TAMANHO_DO_BLOCO + margem; // Canto superior esquerdo y
    int x2 = (x + 1) * TAMANHO_DO_BLOCO - margem; // Canto inferior direito x
    int y2 = (y + 1) * TAMANHO_DO_BLOCO - margem; // Canto inferior direito y

    video_box(x1, y1, x2, y2, cor); // Desenha o bloco
}

// Desenho do campo de jogo com margem
void desenharCampo() {
    video_clear(); // Limpa a tela

    // Desenha as bordas laterais
    for (int y = 0; y <= ALTURA; y++) {
        desenharBloco(0, y, video_ORANGE); // Borda esquerda
        desenharBloco(LARGURA + 1, y, video_ORANGE); // Borda direita
    }

    // Desenha a borda inferior
    for (int x = 0; x <= LARGURA + 1; x++) {
        desenharBloco(x, ALTURA, video_ORANGE); // Borda inferior
    }

    // Desenha os blocos fixos no tabuleiro
    for (int y = 0; y < ALTURA; y++) {
        for (int x = 0; x < LARGURA; x++) {
            if (borda[y][x]) {
                desenharBloco(x + 1, y, video_GREY); // Ajusta as coordenadas do bloco fixo
            }
        }
    }

    // Desenha a peça atual na posição correta
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (pecas[indice_peca][i][j]) {
                desenharBloco(posicao_da_peca_no_tabuleiroX + j + 1, posicao_da_peca_no_tabuleiroY + i, video_color[indice_peca]);
            }
        }
    }

    video_show(); // Atualiza a tela
}

int descida() {
    if (!colisao(posicao_da_peca_no_tabuleiroX, posicao_da_peca_no_tabuleiroY + 1, indice_peca)) {
        // Verifica se a peça pode descer uma linha sem colidir
        posicao_da_peca_no_tabuleiroY++; // Move o bloco para baixo
        return 0; // Peça não precisa ser fixada ainda
    } else {
        fixarPeca(); // Fixar a peça no tabuleiro
        checarLinhas(); // Verificar linhas completas
        return 1; // Peça precisa ser fixada
    }
}

void fixarPeca() {
    // Um laço que percorre as linhas da peça atual
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (pecas[indice_peca][i][j]) {
                borda[posicao_da_peca_no_tabuleiroY + i][posicao_da_peca_no_tabuleiroX + j] = 1; // Atualiza a matriz borda
            }
        }
    }
}

// Verifica se a posição (x, y) da peça está ocupada
int colisao(int x, int y, int indice_peca_atual) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (pecas[indice_peca_atual][i][j] && (y + i >= ALTURA || x + j >= LARGURA || x + j < 0 || borda[y + i][x + j])) {
                return 1; // Retorna 1 se houver colisão
            }
        }
    }
    return 0; // Retorna 0 se não houver colisão
}

// Prévia da próxima peça
void desenharProximaPeca() {
    int x_offset = LARGURA + 2; // Posição x para exibir a próxima peça
    int y_offset = 3; // Posição y para exibir a próxima peça

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (pecas[proxima_peca][i][j]) {
                desenharBloco(x_offset + j, y_offset + i, video_color[proxima_peca]);
            }
        }
    }
}

void checarLinhas() {
    // Percorre todas as linhas do tabuleiro para verificar se estão completas
    for (int y = 0; y < ALTURA; y++) {
        int linha_completa = 1; // Assume que a linha está completa
        for (int x = 0; x < LARGURA; x++) {
            if (borda[y][x] == 0) {
                linha_completa = 0; // Se encontrar um espaço em branco, a linha não está completa
                break;
            }
        }

        if (linha_completa) {
            // Se a linha está completa, continua com a remoção dela.
            pontuacao += 1; // Incrementa a pontuação
            for (int i = y; i > 0; i--) {
                for (int j = 0; j < LARGURA; j++) {
                    borda[i][j] = borda[i - 1][j]; // Move as linhas para baixo
                }
            }

            for (int j = 0; j < LARGURA; j++) {
                borda[0][j] = 0; // Limpa a linha superior
            }
            y--; // Reajusta o valor de y para verificar novamente
        }
    }
}

void mover(int lado) {
    int x = posicao_da_peca_no_tabuleiroX + lado; // Calcula nova posição x

    if (!colisao(x, posicao_da_peca_no_tabuleiroY, indice_peca)) {
        posicao_da_peca_no_tabuleiroX = x; // Move a peça para a nova posição
    }
}

int main() {
    video_open(); // Abre a interface de vídeo
    srand(time(0)); // Inicializa o gerador de números aleatórios
    proxima_peca = rand() % 6; // Gera a primeira próxima peça

    while (1) {
        if (descida()) {
            indice_peca = proxima_peca; // A peça atual é a próxima
            proxima_peca = rand() % 6; // Gera uma nova próxima peça
            posicao_da_peca_no_tabuleiroX = 3; // Reinicia a posição
            posicao_da_peca_no_tabuleiroY = 0;

            if (colisao(posicao_da_peca_no_tabuleiroX, posicao_da_peca_no_tabuleiroY, indice_peca)) {
                printf("Game Over!\n");
                break; // Encerra o jogo se houver colisão
            }
        }

        desenharCampo(); // Desenha o campo de jogo
        desenharProximaPeca(); // Desenha a próxima peça
        usleep(TEMPO_QUEDA); // Espera pelo tempo de queda
    }

    video_close(); // Fecha a interface de vídeo
    return 0; // Retorna 0 ao finalizar
}
