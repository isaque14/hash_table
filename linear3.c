#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define tam 100000 
char arquivo[] = "tablin3.bin";
int hash(int chave, int m) {
    return chave % m;
}

double fatorCarga(){
    FILE *arq = fopen(arquivo,"rb");
    int aux;
    double nRegis = 0;
    for(int i = 0; i < tam; i++){
        fseek(arq, i*sizeof(int), SEEK_SET);
        fread(&aux, sizeof(int), 1, arq);
        if(aux != -1)
            nRegis++;
    }
    fclose(arq);
    return nRegis/tam;
}

void criaTabela(){
    FILE *arq = fopen(arquivo,"wb");
    int aux = -1;
    for(int i=0;i<tam;i++){
        fseek(arq,i*sizeof(int),SEEK_SET);
        fwrite(&aux,sizeof(int),1,arq);
    }
    fclose(arq);
}

int buscaChave(int chave) {
    
    FILE *arq = fopen(arquivo,"rb");
    int aux;
    int init = hash(chave,tam);
    for(int i=init;i<init+100;i++){
        fseek(arq,i*sizeof(int),SEEK_SET);
        fread(&aux,sizeof(int),1,arq);
        if(aux == chave){
            fclose(arq);
            return i;
        }
    }
    fclose(arq);
    return -1;
}

void insereChave(int chave) {
    int aux = buscaChave(chave);
    if(aux == -1){
        FILE *arq = fopen(arquivo,"r+b");
        int init = hash(chave,tam);
        for(int i=init;i<init+100&&i<tam;i++){
            fseek(arq,i*sizeof(int),SEEK_SET);
            fread(&aux,sizeof(int),1,arq);
            if(aux == -1){
                fseek(arq,i*sizeof(int),SEEK_SET);
                fwrite(&chave,sizeof(int),1,arq);
                printf("inserindo a chave %d\n",chave);
                break;
            }
        }
        fclose(arq);
    }
    else{
        printf("sem espaço livre para a chave %d\n",chave);
        }
}

void removeChave(int chave){
    int aux = buscaChave(chave);
    int vazio = -1;
    if(aux==-1){
        printf("chave nao encontrada\n");
    }
    else{
        FILE *arq = fopen(arquivo,"r+b");
        fseek(arq,aux*sizeof(int),SEEK_SET);
        fwrite(&vazio,sizeof(int),1,arq);
        fclose(arq);
        printf("chave %d removida\n",chave);
    }
}

void imprimeTabela() {
    FILE *arq = fopen(arquivo,"rb");
    int aux = 0;
    
    for(int i = 0; i < tam; i++){
        fseek(arq, i*sizeof(int), SEEK_SET);
        fread(&aux, sizeof(int), 1, arq);
        if(aux == -1){
            printf("* ");
        }
        else{
            printf("%d ", aux);
        }
    }
    printf("\n"); 
    fclose(arq);
}

int main(void) {
    
    criaTabela();
    imprimeTabela();
    srand(time(NULL));

    while(fatorCarga()<=0.9){
        insereChave(rand()%tam*100);
    }  
    while(fatorCarga()>=0.8){
        removeChave(rand()%tam*100);
    }

    imprimeTabela();

    return 0;
}