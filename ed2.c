#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define tam 10000 //2m e 5s sem printf

int num_primo (int num){
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

int hash_linear(int chave, int m, int k) {
    return (hash(chave, m) + k) % m;
}

int hash_quadratico(int chave, int m, int k){
    return (hash(chave, m) + k*k) % m;
}

int hash_ii(int k, int chave){
    return (k - (chave % k));
}

int hash_duplo(int chave, int m, int k){
    int q = num_primo(m-1);
    if (q != -1)
        return (hash(chave, m) + k * hash_ii(q, chave)) % m;
}

double fatorCarga(){
    FILE *arq = fopen("tabela.bin","rb");
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
    FILE *arq = fopen("tabela.bin","wb");
    int aux = -1;
    for(int i=0;i<tam;i++){
        fseek(arq,i*sizeof(int),SEEK_SET);
        fwrite(&aux,sizeof(int),1,arq);
    }
    fclose(arq);
}

int buscaChave(int chave) {
    
    FILE *arq = fopen("tabela.bin","rb");
    int aux;
    for(int i=hash(chave,tam);i<tam;i++){
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
        FILE *arq = fopen("tabela.bin","r+b");
        for(int i=hash(chave,tam);i<tam;i++){
            fseek(arq,i*sizeof(int),SEEK_SET);
            fread(&aux,sizeof(int),1,arq);
            if(aux == -1){
                fseek(arq,i*sizeof(int),SEEK_SET);
                fwrite(&chave,sizeof(int),1,arq);
                //printf("inserindo a chave %d\n",chave);
                break;
            }
        }
        fclose(arq);
    }
    //else{
        //printf("sem espaço livre para a chave %d\n",chave);
        //}
}

void removeChave(int chave){
    int aux = buscaChave(chave);
    int vazio = -1;
    if(aux==-1){
        printf("chave nao encontrada\n");
    }
    else{
        FILE *arq = fopen("tabela.bin","r+b");
        fseek(arq,aux*sizeof(int),SEEK_SET);
        fwrite(&vazio,sizeof(int),1,arq);
        fclose(arq);
        printf("chave %d removida\n",chave);
    }
}

void imprimeTabela() {
    FILE *arq = fopen("tabela.bin","rb");
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

    while(fatorCarga() < 0.7){
        insereChave(rand()%tam*2);
    }   
    
    imprimeTabela();

    return 0;
}

