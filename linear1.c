#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tabelahash.h"
#define tam 1000 
char arquivo[] = "tablin1.bin";

int main(void) {
    
    criaTabela(arquivo,tam);
    imprimeTabela(arquivo,tam);
    srand(time(NULL));
    int aux = 0;

    while(fatorCarga(arquivo,tam)<=0.9){
       aux+=insereChaveLinear(rand()%tam*3,arquivo,tam);
    }  
    while(fatorCarga(arquivo,tam)>=0.8){
        removeChaveLinear(rand()%tam*5,arquivo,tam);
    }

    imprimeTabela(arquivo,tam);
    printf("colisao = %d\n",aux);
    return 0;
}

