#include "hash.h"
#include<stdlib.h>

// FUNCTIONS

// Gera chaves aleatórias não repetidas
void geraAleatorio(int *vet, int m){
    int i = 0, j, igual;
    srand(time(NULL));

    
    do{ // faça
        int chave = rand() % m*2; // sorteia uma chave
        printf("Chave sorteada -> %d\n", chave);
        insere(vet, m, chave);
        igual = 0;
        for(j = 0; j < i; j++){ // percorre a parte do vetor já preenchida
            if(vet[j] == vet[i])
                igual = 1; // chave repetida
        }

        if(igual == 0) // significa que a chave não se repetiu
            i++;
    }while(i < m); // enquanto não for sorteado m chaves diferentes
    printf("\n\n");
}

// Inicializa o Vetor de chaves com posições vazias (considera -1 como posições vazias)
void inicializa(int *tab, int m) {
    int i;
    for (i = 0; i < m; i++) {
        tab[i] = -1;
    }
}

void imprime(int *vet, int m) {
    printf("[ ");
    for (int i = 0; i < m; i++){
        if (vet[i] != -1)
            printf("%d ", vet[i]);
        else
            printf("* ");
    }
    printf("]");  
    printf("\n");
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

// Auxiliar do hash duplo
int hash_ii(int k, int chave){
    return (k - (chave % k));
}

int hash_duplo(int chave, int m, int k){
    int q = num_primo(m-1);
    printf("Primo antecessor de m -> %d\n", q);
    if (q != -1)
        return (hash(chave, m) + k * hash_ii(q, chave)) % m;
}

/*
 * Função busca

   RETORNO:
     Se chave mat for encontrada, achou = 1,
     função retorna endereço onde mat foi encontrada

     Se chave mat não for encontrada, achou = 0, e há duas
     possibilidades para valor retornado pela função:
	   endereço de algum compartimento livre encontrado durante a busca
	   -1 se não for encontrado endereço livre (tabela foi percorrida até o final)
 */
int busca(int *tab, int m, int chave, int *achou, int *colisao) {
    printf("\nBuscando...\n");
    *achou = 0;
    int end = -1;
    int pos_livre = -1;
    int k = 0;

    while (k < m) {
        end = hash_duplo(chave, m, k);
        //printf("End = %d\n", end);
        if (tab[end] != -1 && tab[end] == chave) {
            //encontrou chave
            *achou = 1;
            k = m; //força saída do loop
        }
        else {
            if (tab[end] == -1) {
                //encontrou endereco livre
                //registra caso já não tenha sido encontrado algum anteriormente
                if (pos_livre == -1) 
                    pos_livre = end;
                else
                    colisao = colisao +1;
                    //printf("Colisao %d\n", colisao);
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
}

void insere(int *tab, int m, int chave) {
    int achou;
    int end = busca(tab, m, chave, &achou, 0);
    if (!achou) {
        if (end != -1) {
            //Não encontrou a chave, mas encontrou posição livre
            //Inserção será realizada nessa posição livre encontrada pela busca
            tab[end] = chave;
        } else {
            //Não foi encontrada posição livre durante a busca: overflow
            printf("A tabela esta cheia. Ocorreu overflow. Insercao nao realizada!\n");
        }
    } else {
        printf("Chave ja existe. Insercao invalida! \n");
    }
}

void exclui(int *tab, int m, int chave) {
    int achou;
    int end = busca(tab, m, chave, &achou, 0);
    if (achou) {
        //remove
        tab[end] = -1;
    } else {
        printf("Matricula nao encontrada. Remocao nao realizada!");
    }
}

// Retorna o indice da chave no vetor
int indice (int *vet, int m, int chave){
    for (int i = 0; i < m; i++){
        if (vet[i] == chave)
            return i;
    }
}

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