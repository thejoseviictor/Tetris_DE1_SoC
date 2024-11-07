#include <stdio.h>
#include <stdlib.h>

#include "return_decomposto.h"
#include "exibir_pontos.h"


int* return_decomposto(int pontos) //Função de retorno da pontuação decomposta. A pontuação máxima foi definida como 99;
{
    static int tamanho = 2;
    
    int* dVetor = (int*)malloc(tamanho * sizeof(int)); //Define o vetor que irá guardar os valores decompostos dos pontos
    
   if(pontos>=0 && pontos < 10){
       
        dVetor[0] = 0; 
        dVetor[1] = pontos; // Definindo a unidade;
       
   }
   else if(pontos >= 10 && pontos <= 99){

        //Preenchendo o vetor "decomposto" com dezena e unidade;
        dVetor[1] = pontos % 10; // Definindo a unidade;   
        dVetor[0] = pontos/10; // Definindo a dezena;
   
   }

   else{
        dVetor[0] = 0;
        dVetor[1] = 0; 
   }
   
   /*
   
    //Lógica descartada: Se o máximo for 999 pontos;
   
    //Definindo uniddde
    int cdT = pontos / 10; //Salva a parte inteira da divisão, excluindo o decimal (unidade do número);
    int u = pontos - (uT * 10); //Define a unidade original

    //Definindo dezena
    int cT = ((pontos - u) / 100); //Salva a parte inteira da divisão, excluindo o decimal (dezena e unidade do número);
    int d = (pontos - u) - (cT * 100); //Define a dezena original
    
    // Definindo a centena
    int c = (pontos - (u + d)); // Define a centena original
    
    vetor[0] = c;
    vetor[1] = d; 
    vetor[2] = u; 
    
    */
    
    return dVetor;
} 