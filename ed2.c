#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <hash.h>

int colisao = 0;

int main() {
    int m = 12, chave = 0, result;
    int tab[m];
    int *vet = tab;
    printf("***COMECO***\n");

   
    printf("\nInicializado\n");
    inicializa(vet, m);
    imprime(vet,m);
    
    /*
    geraAleatorio(vet, m); 
    printf("Vetor Incerido\n");
    imprime(vet, m);
    
    printf("Numero de Colisoes = %d\n\n", colisao);

    //CÓDIGO PARA BUSCA
    printf("Digite a chave a ser procurada... ");
    scanf("%d", &chave);
    int achou;
    int end = busca(vet, m, chave, &achou, &colisao);
    if (achou == 0) printf("Chave nao encontrada!\n");
    else {
        int p = vet[end];
        printf("Chave %d encontrada -> posicao = %d\n", p, indice(vet, m, p));
    }
    
    */

     // CÓDIGO PARA INSERÇÃO
    for (int i = 0; i < 6; i++){
        printf("Digite a chave a ser inserida -> ");
        scanf("%d", &chave);
        insere(vet, m, chave);
        printf("\nTabela Final:\n");
        imprime(vet, m);
        printf("Numero de Colisoes = %d\n\n", colisao);
    }
    /* CÓDIGO PARA REMOÇÃO
    printf("Digite a chave a ser removida -> ");
    scanf("%d", &chave);
    exclui(vet, tamVet, chave);
    printf("\nTabela Final:\n");
    imprime(vet, tamVet);
    */

    printf("FIM...");
    return 0;
}

