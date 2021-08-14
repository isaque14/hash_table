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

void criaTabela(){
    FILE *arq = fopen("tabela.bin","wb");
    int aux = -1;
    for(int i=0;i<tam;i++){
        fwrite(&aux,sizeof(int),sizeof(int),arq);
        fseek(arq,i,SEEK_SET);
    }
}

float fatorCarga(){
    FILE *arq = fopen("tabela.bin","rb");
    int aux;
    int vazio = 0;
    for(int i=0;i<tam;i++){
        fseek(arq,i,SEEK_SET);
        fread(&aux,sizeof(int),sizeof(int),arq);
        if(aux == -1){
            vazio++;
        }
    }
    fclose(arq);
    return (tam-vazio)/vazio;
}

int busca(int chave){
    FILE *arq = fopen("tabela.bin","rb");
    int aux;
    
    for(int i = hash(chave,tam);i<tam;i++){
        fseek(arq,i,SEEK_SET);
        fread(&aux,sizeof(int),sizeof(int),arq);
        if(aux == chave){
            return -2;
        }
    }
    for(int i=hash(chave,tam);i<tam;i++){
        fseek(arq,i,SEEK_SET);
        fread(&aux,sizeof(int),sizeof(int),arq);
        if(aux == -1){
            return i;
        }
    }

    fclose(arq);
    return -1;
}

void insere(int chave){
    FILE *arq = fopen("tabela.bin","wb");
    int aux = busca(chave);

    if(aux == -2){
        printf("Chave já inserida\n");
    }
    if(aux == -1){
        printf("Não há espaço disponível\n");
    }
    if(aux >= 0){
        fseek(arq,aux,SEEK_SET);
        fwrite(&chave,sizeof(int),sizeof(int),arq);
    }

    fclose(arq);
}

void removeChave(int chave){
    int aux = busca(chave);
    if(aux == -2){
        FILE *arq = fopen("tabela.bin","wb");
        for(int i = hash(chave,tam);i<tam;i++){
            fseek(arq,i,SEEK_SET);
            fread(&aux,sizeof(int),sizeof(int),arq);
            if(aux == chave){
                aux = -1;
                fwrite(&aux,sizeof(int),sizeof(int),arq);
            }
        }
    }
    printf("Chave não encontrada\n");
}


int main(void) {
    criaTabela();
    criaDados(100);
    insere(1);

    return EXIT_SUCCESS;
}

