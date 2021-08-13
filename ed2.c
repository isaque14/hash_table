#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define tam 100

void criaDados(int m){
    FILE *arq = fopen("entradas.bin","wb");
    int aux;
    for(int i=0;i<tam;i++){
        aux = rand() % m;
        fwrite(&aux,sizeof(int),sizeof(int),arq);
    }
    fclose(arq);
}

int hash(int chave, int m) {
    return chave % m;
}

void insere(int chave){
    FILE *arq = fopen("tabela.bin","wb");
    fseek(arq,hash(chave,tam),SEEK_SET);
    fwrite(&chave,sizeof(int),sizeof(int),arq);
    fclose(arq);
}

int busca(int chave){
    FILE *arq = fopen("tabela.bin","rb");
    int aux;
    fseek(arq,hash(chave,tam),SEEK_SET);
    fread(&aux,sizeof(int),sizeof(int),arq);

    if(aux == chave){
        return 1;
    }
    else{
        for(int i = hash(chave,tam);i<100;i++){
            fseek(arq,i,SEEK_SET);
            fread(&aux,sizeof(int),sizeof(int),arq);
            if(aux == chave){
                return 1;
            }
        }
    }
    fclose(arq);
    return 0;
}


int main(void) {

    criaDados(100);
    insere(101);
    printf("%d\n",busca(9999));

    return EXIT_SUCCESS;
}

