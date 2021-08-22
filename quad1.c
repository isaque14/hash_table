#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tabelahash.h"
#define tam 1000 
char arquivo[] = "tabquad1.bin";

int main(void) {
    
    int aux = 0;
    criaTabela(arquivo,tam);
    imprimeTabela(arquivo,tam);
    srand(time(NULL));

    while(fatorCarga(arquivo,tam)<=0.9){
        aux=aux+insereChaveQuadratico(rand()%tam*3,arquivo,tam);
    }  
    while(fatorCarga(arquivo,tam)>=0.8){
        removeChaveQuadratico(rand()%tam*3,arquivo,tam);
    }

    imprimeTabela(arquivo,tam);
    printf("colisao = %d",aux);

    return 0;
}