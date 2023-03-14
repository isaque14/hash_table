#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define tam 100 

// Alternar entre tipos de hash na função busca linha 109

int colisao = 0;

int criaChave()
{
    return rand() % 100000;
}

int num_primo(int num)
{
    int cont = 0;
    do
    {
        for (int i = 1; i <= num; i++)
            if (num % i == 0)
                cont++;

        if (cont != 2)
            num--, cont = 0;

    } while (cont != 2 && num > 0);

    if (cont == 2)
        return num;
    else
        return -1;
}

int hash(int chave, int m)
{
    return chave % m;
}

int hash_linear(int chave, int m, int k)
{
    return (hash(chave, m) + k) % m;
}

int hash_quadratico(int chave, int m, int k)
{
    return (hash(chave, m) + k * k) % m;
}

// Auxiliar do hash duplo
int hash_ii(int k, int chave)
{
    return (k - (chave % k));
}

int hash_duplo(int chave, int m, int k)
{
    int q = num_primo(m - 1);
    if (q != -1)
        return (hash(chave, m) + k * hash_ii(q, chave)) % m;
}

void criaTabela()
{
    FILE *arq = fopen("tabela.bin", "wb");
    int aux = -1;

    for (int i = 0; i < tam; i++)
    {
        fseek(arq, i * sizeof(int), SEEK_SET);
        fwrite(&aux, sizeof(int), 1, arq);
    }
    fclose(arq);
}

double fatorCarga()
{
    FILE *arq = fopen("tabela.bin", "rb");
    int aux;
    double nRegis = 0;
    for (int i = 0; i < tam; i++)
    {
        fseek(arq, i * sizeof(int), SEEK_SET);
        fread(&aux, sizeof(int), 1, arq);
        if (aux != -1)
            nRegis++;
    }
    fclose(arq);
    return nRegis / tam;
}

int busca(int chave, int *achou)
{
    FILE *arq = fopen("tabela.bin", "rb");

    *achou = 0;
    int end = -1;
    int pos_livre = -1;
    int k = 0;
    int aux = 0;

    while (k < tam)
    {
        end = hash_linear(chave, tam, k);

        fseek(arq, end * sizeof(int), SEEK_SET);
        fread(&aux, sizeof(int), 1, arq);

        if (aux == chave)
        {
            *achou = 1;
            k = tam; 
        }
        else
        {
            if (aux == -1)
            {
                //encontrou endereco livre
                //registra caso já não tenha sido encontrado algum anteriormente
                if (pos_livre == -1)
                {
                    pos_livre = end;
                    k = tam;
                }
            }
            else
            {
                colisao++;
            }

            k = k + 1; 
        }
    }

    if (*achou)
    {
        return end;
    }
    else
    {
        return pos_livre;
    }

    fclose(arq);
}

void insere(int chave)
{
    int achou;
    int end = busca(chave, &achou);

    if (!achou)
    {
        if (end != -1)
        {
            //Não encontrou a chave, mas encontrou posição livre
            //Inserção será realizada nessa posição livre encontrada pela busca
            FILE *arq = fopen("tabela.bin", "r+b");
            fseek(arq, end * sizeof(int), SEEK_SET);
            fwrite(&chave, sizeof(int), 1, arq);
            fclose(arq);
        }
        else
        {
            printf("A tabela esta cheia. Ocorreu overflow. Insercao nao realizada!\n");
        }
    }
    else
    {
        printf("Chave ja existe. Insercao invalida! \n");
    }
}

void removeChave(int chave)
{
    int achou;
    int end = busca(chave, &achou);
    if (achou)
    {
        chave = -1;
        FILE *arq = fopen("tabela.bin", "r+b");
        fseek(arq, end * sizeof(int), SEEK_SET);
        fwrite(&chave, sizeof(int), 1, arq);
        fclose(arq);
        printf("chave removida\n");
    }
    else
    {
        printf("Chave nao encontrada. Remocao nao realizada!\n");
    }
}

void imprimeVetor(int *vet)
{
    printf("[ ");

    for (int i = 0; i < tam; i++)
    {
        printf("%d ", vet[i]);
    }

    printf("]\n\n*****\n");
}

void imprime()
{
    FILE *arq = fopen("tabela.bin", "rb");

    printf("[ ");
    int aux = 0;

    for (int i = 0; i < tam; i++)
    {
        fseek(arq, i * sizeof(int), SEEK_SET);
        fread(&aux, sizeof(int), 1, arq);
        if (aux == -1)
            printf("* ");
        else
            printf("%d ", aux);
    }

    printf("]\n");
    fclose(arq);
}

int main(void)
{
    printf("Gerando Dados...\n\n");

    criaTabela();

    // CÓDIGO INSERÇÃO/REMOÇÃO ALTOMÁTICA COM FATOR DE CARGA

    int chave = 0;
    int end = 0;
    float carga = 0;

    for (int i = 0; i < tam; i++)
    {

        chave = criaChave();
        carga = fatorCarga();

        if (carga < 0.9)
            insere(chave);

        if (carga > 0.9)
        {
            end = rand() % tam;
            printf("\nIncersao nao realizada, Fator de Carga > 0.9\n");
            printf("Endereco sorteado para remocao -> %d\n", end);

            FILE *arq = fopen("tabela.bin", "rb");
            fseek(arq, end * sizeof(int), SEEK_SET);
            fread(&chave, sizeof(int), 1, arq);
            fclose(arq);

            removeChave(chave);

            i = tam;
        }
    }

    printf("\nTabela Final\n");
    imprime();
    printf("\n\nfator de Carga final -> %.3f\n", fatorCarga());
    printf("Numero de Colisoes -> %d\n\n", colisao);

    clock_t tempoTotal;
    chave = -1;
    int achou;
    end = rand() % tam;
    FILE *arq = fopen("tabela.bin", "rb");

    while (chave == -1)
    {
        fseek(arq, end * sizeof(int), SEEK_SET);
        fread(&chave, sizeof(int), 1, arq);
    }
    fclose(arq);

    tempoTotal = clock();
    busca(chave, &achou);
    tempoTotal = clock() - tempoTotal;

    printf("\n\nTempo gasto para buscar a chave %d no arquivo:  %.5f milessegundos\n",
           chave, ((float)tempoTotal) / CLOCKS_PER_SEC);

    printf("FIM!\n");
    return 0;
}
