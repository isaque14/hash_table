#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define tam 100


// Gera chaves aleatórias não repetidas
void criaDados(){
    int i = 0, j, igual, aux;
    srand(time(NULL));

    do{ 
        int chave = rand() % tam*2; // sorteia uma chave
        //printf("Chave sorteada -> %d\n", chave);
        
        FILE *arq = fopen("entradas.bin","rb+");
        fseek(arq, i*sizeof(int), SEEK_SET);
        fwrite(&chave, sizeof(int), 1, arq);
        fclose(arq);

        igual = 0;
        for(j = 0; j < i; j++){ // percorre a parte do arquivo já preenchida
            FILE *arq = fopen("entradas.bin","rb");

            fseek(arq, j*sizeof(int), SEEK_SET);
            fread(&aux, sizeof(int), 1, arq);
            fclose(arq);
            
            if(chave == aux)
                igual = 1; // chave repetida
        }

        if(igual == 0) // significa que a chave não se repetiu
            i++;
    }while(i < tam); // enquanto não for sorteado tam chaves diferentes
    printf("\n\n");
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
    //printf("Primo antecessor de m -> %d\n", q);
    if (q != -1)
        return (hash(chave, m) + k * hash_ii(q, chave)) % m;
}

void criaTabela(){
    FILE *arq = fopen("tabela.bin","wb");
    int aux = -1;

    for(int i = 0; i < tam; i++){
        fseek(arq, i*sizeof(int), SEEK_SET);
        fwrite(&aux, sizeof(int), 1, arq);
    }
    fclose(arq);
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

int busca(int chave, int *achou) {
    
    FILE *arq = fopen("tabela.bin","rb");
    
    //printf("\nBuscando...\n");
    *achou = 0;
    int end = -1;
    int pos_livre = -1;
    int k = 0;
    int aux = 0;

    while (k < tam) {
        end = hash_duplo(chave, tam, k);
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
            //printf("chave incerida");
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
        printf("Chave nao encontrada. Remocao nao realizada!\n");
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
    printf("GERANDO DADOS ALEATORIOS...\n");
    criaTabela();
    criaDados();

    printf("Tabela Inicial\n");
    imprime();
    printf("\n");
    
    // CÓDIGO INCERÇÃO/REMOÇÃO ALTOMÁTICA COM FATOR DE CARGA
    FILE *imput = fopen("entradas.bin","rb");
    int chave = 0;
    int end = 0;
    float carga = 0;
    for (int i = 0; i < tam; i++){
           
        fseek(imput, i*sizeof(int), SEEK_SET);
        fread(&chave, sizeof(int), 1, imput);

        carga = fatorCarga();
        printf("Fator de Carga = %.3f\n", carga);
        
        if (carga < 0.9)
            insere(chave);
        
        if (carga > 0.9){
            end = rand() % tam;
            printf("Tabela Atual\n");
            imprime();
            printf("Incersao nao realizada, Fator de Carga > 0.9\n");
            printf("Endereco sorteado para remocao -> %d\n", end);
            
            FILE *arq = fopen("tabela.bin","rb");
            fseek(arq, end*sizeof(int), SEEK_SET);
            fread(&chave, sizeof(int), 1, arq);
            fclose(arq);

            removeChave(chave);
        }
    }
    
    printf("fator de Carga final -> %.3f\n", fatorCarga());
    printf("\nTabela Final\n");
    imprime();
    fclose(imput);
    

    // CÓDIGO PARA INSERÇÃO COM FATOR DE CARGA
    /*
    int chave = 0;
    float carga = 0;
    for (int i = 0; i < 11; i++){
        printf("Digite a chave a ser inserida -> ");
        scanf("%d", &chave);
        carga = fatorCarga();
        printf("Fator de Carga = %.3f\n", carga);

        if (carga < 0.9)
            insere(chave);

        if (carga > 0.9){
            chave = rand() % tam;
            printf("Incerso nao realizada, Fator de Carga > 0.9\n");
            printf("Chave sorteada para remocao -> %d\n", chave);
            removeChave(chave);
        }

        printf("\nTabela Atual:\n");
        imprime();
        printf("\n");
    }
    */
    
    //CÓDIGO PARA REMOÇÃO
    /*
    printf("Digite a chave a ser removida -> ");
    scanf("%d", &chave);
    removeChave(chave);

    printf("\nTabela Final:\n");
    imprime();    
    */

    //CÓDIGO PARA BUSCA
    /*
    for (int i = 0; i < 3; i++){
        printf("\n\n");
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
    
    printf("FIM!\n");                           
    return 0;
}

