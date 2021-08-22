#include<stdio.h>
#include<stdlib.h>
#include"tabelahash.h"

int numPrimo (int num){
    int cont = 0;
    do{
        for (int i = 1; i <= num; i++) 
            if (num % i == 0)
                cont++;

       if (cont != 2)
            num--, cont = 0; 

    }while (cont != 2 && num > 0);    

    if (cont == 2)
        return num;  
    else
        return -1;  
}

int hash(int chave, int m) {
    return chave % m;
}

double fatorCarga(char* tabela,int tam){
    FILE *arq = fopen(tabela,"rb");
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

void criaTabela(char* tabela,int tam){
    FILE *arq = fopen(tabela,"wb");
    int aux = -1;
    for(int i=0;i<tam;i++){
        fseek(arq,i*sizeof(int),SEEK_SET);
        fwrite(&aux,sizeof(int),1,arq);
    }
    fclose(arq);
}

void imprimeTabela(char* tabela,int tam){
    FILE *arq = fopen(tabela,"rb");
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

int buscaChaveQuadratico(int chave,char* tabela,int tam){
    
    FILE *arq = fopen(tabela,"rb");
    int aux;
    int k=0;
    int init = hash(chave,tam);
    for(int i=init;i<tam;k++){
        i += (k*k);
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

int insereChaveQuadratico(int chave,char* tabela,int tam) {
    int aux = buscaChaveQuadratico(chave,tabela,tam);
    int colisao = 0;
    if(aux == -1){
        FILE *arq = fopen(tabela,"r+b");
        int init = hash(chave,tam);
        int k=0;
        for(int i=init;i<tam;k++){
            i += k*k;
            fseek(arq,i*sizeof(int),SEEK_SET);
            fread(&aux,sizeof(int),1,arq);
            if(aux == -1){
                fseek(arq,i*sizeof(int),SEEK_SET);
                fwrite(&chave,sizeof(int),1,arq);
                printf("inserindo a chave %d\n",chave);
                break;
            }
            else{
                colisao++;
            }
        }
        fclose(arq);
    }
    return colisao;
}

void removeChaveQuadratico(int chave,char* tabela,int tam){
    int aux = buscaChaveQuadratico(chave,tabela,tam);
    int vazio = -1;
    if(aux==-1){
        printf("chave nao encontrada\n");
    }
    else{
        FILE *arq = fopen(tabela,"r+b");
        fseek(arq,aux*sizeof(int),SEEK_SET);
        fwrite(&vazio,sizeof(int),1,arq);
        fclose(arq);
        printf("chave %d removida\n",chave);
    }
}

int buscaChaveLinear(int chave,char* tabela,int tam) {
    
    FILE *arq = fopen(tabela,"rb");
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

int insereChaveLinear(int chave,char* tabela,int tam){
    int aux = buscaChaveLinear(chave,tabela,tam);
    int colisao = 0;
    if(aux == -1){
        FILE *arq = fopen(tabela,"r+b");
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
            else{
                colisao++;
            }
        }
        fclose(arq);
    }
    else{
        printf("sem espaço livre para a chave %d\n",chave);
    }
    return colisao;
}

void removeChaveLinear(int chave,char* tabela,int tam){
    int aux = buscaChaveLinear(chave,tabela,tam);
    int vazio = -1;
    if(aux==-1){
        printf("chave nao encontrada\n");
    }
    else{
        FILE *arq = fopen(tabela,"r+b");
        fseek(arq,aux*sizeof(int),SEEK_SET);
        fwrite(&vazio,sizeof(int),1,arq);
        fclose(arq);
        printf("chave %d removida\n",chave);
    }
}