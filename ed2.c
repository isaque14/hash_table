#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define tam 11

/*
// Gera chaves aleatórias não repetidas
void geraAleatorio(int *vet){
    int i = 0, j, igual;
    srand(time(NULL));


    do{ // faça
        int chave = rand() % tam*2; // sorteia uma chave
        printf("Chave sorteada -> %d\n", chave);
        insere(vet, tam, chave);
        igual = 0;
        for(j = 0; j < i; j++){ // percorre a parte do vetor já preenchida
            if(vet[j] == vet[i])
                igual = 1; // chave repetida
        }

        if(igual == 0) // significa que a chave não se repetiu
            i++;
    }while(i < tam); // enquanto não for sorteado m chaves diferentes
    printf("\n\n");
}

*/

void criaDados(int m){
    FILE *arq = fopen("entradas.bin","wb"); 
    int aux;
    for(int i=0;i<tam;i++){
        aux = rand() % m;
        fwrite(&aux, sizeof(int), 1, arq);
    }
    fclose(arq);
}

int hash(int chave, int m) {
    return chave % m;
}

int hash_linear(int chave, int m, int k) {
    return (hash(chave, m) + k) % m;
}

void criaTabela(){
    FILE *arq = fopen("tabela.bin","wb");
    int aux = -1;

    for(int i = 0; i < 11; i++){
        fseek(arq, i*sizeof(int), SEEK_SET);
        fwrite(&aux, sizeof(int), 1, arq);
    }
    fclose(arq);
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

int busca(int chave, int *achou) {
    
    FILE *arq = fopen("tabela.bin","rb");
    
    printf("\nBuscando...\n");
    *achou = 0;
    int end = -1;
    int pos_livre = -1;
    int k = 0;
    int aux = 0;

    while (k < tam) {
        end = hash_linear(chave, tam, k);
        //printf("End = %d\n", end);
       
        fseek(arq, end*sizeof(int), SEEK_SET);
        fread(&aux, sizeof(int), 1, arq);

        if (aux == chave) {
            //encontrou chave
            *achou = 1;
            k = tam; //força saída do loop
        }
        else {
            if (aux == -1) {
                //encontrou endereco livre
                //registra caso já não tenha sido encontrado algum anteriormente
                if (pos_livre == -1) 
                    pos_livre = end;
            }
            k = k + 1; //continua procurando
        }
    }
    if (*achou) {
        return end;
    }
    else {
        return pos_livre;
    }

    fclose(arq);
}

void insere(int chave) {
    int achou;
    int end = busca(chave, &achou);
    

    if (!achou) {
        if (end != -1) {
                //Não encontrou a chave, mas encontrou posição livre
                //Inserção será realizada nessa posição livre encontrada pela busca
            FILE *arq = fopen("tabela.bin","rb+");
            fseek(arq, end*sizeof(int), SEEK_SET);   
            fwrite(&chave, sizeof(int), 1, arq);
            fclose(arq);
            printf("chave incerida");
        } 
        else {
            //Não foi encontrada posição livre durante a busca: overflow
            printf("A tabela esta cheia. Ocorreu overflow. Insercao nao realizada!\n");
        }
    } else {
        printf("Chave ja existe. Insercao invalida! \n");
    }
}


void removeChave(int chave) {
    int achou;
    int end = busca(chave, &achou);
    if (achou) {
        //remove
        chave = -1;
        FILE *arq = fopen("tabela.bin","rb+");
        fseek(arq, end*sizeof(int), SEEK_SET);   
        fwrite(&chave, sizeof(int), 1, arq);
        fclose(arq);
        printf("chave removida\n");
    } else {
        printf("Matricula nao encontrada. Remocao nao realizada!\n");
    }
}

void imprime() {
    FILE *arq = fopen("tabela.bin","rb");
    
    printf("[ ");
    int aux = 0;
    
    for(int i = 0; i < tam; i++){
        fseek(arq, i*sizeof(int), SEEK_SET);
        fread(&aux, sizeof(int), 1, arq);
        if (aux == -1)
            printf("* ");
        else
            printf("%d ", aux);
    }

    printf("]\n"); 
    fclose(arq);
}

int main(void) {
    criaTabela();
    //criaDados(100);

    printf("Tabela Inicial\n");
    imprime();
    
    
    // CÓDIGO PARA INSERÇÃO
    int chave = 0;
    for (int i = 0; i < 4; i++){
        printf("Digite a chave a ser inserida -> ");
        scanf("%d", &chave);
        insere(chave);
        printf("\nTabela Atual:\n");
    imprime();
    }

     //CÓDIGO PARA REMOÇÃO
    printf("Digite a chave a ser removida -> ");
    scanf("%d", &chave);
    removeChave(chave);

    printf("\nTabela Final:\n");
    imprime();    
    
    /*
    for (int i = 0; i < 3; i++){
        printf("\n\n");
        //CÓDIGO PARA BUSCA
        int chave;
        printf("Digite a chave a ser procurada... ");
        scanf("%d", &chave);
        int achou;
        int end = busca(chave,&achou);
        if (achou == 0) printf("Chave nao encontrada!\n");
        else {
            printf("Chave %d encontrada, posicao -> %d \n", chave, end);
        }
    }
    */
    

    printf("fim!\n");                           
    return 0;
}

