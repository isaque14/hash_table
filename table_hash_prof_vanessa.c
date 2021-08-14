#include <stdio.h>


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct aluno {
    int matricula;
    float cr;
} TAluno;

typedef TAluno *Hash; //Hash é um vetor que será alocado dinamicamente

TAluno *aloca(int mat, float cr) {
    TAluno *novo = (TAluno *) malloc(sizeof(TAluno));
    novo->matricula = mat;
    novo->cr = cr;
    return novo;
}

void inicializa(Hash *tab, int m) {
    int i;
    for (i = 0; i < m; i++) {
        tab[i] = NULL;
    }
}

int hash(int mat, int m) {
    return mat % m;
}

int hash_linear(int mat, int m, int k) {
    return (hash(mat, m) + k) % m;
}

void libera(Hash *tab, int m) {
    int i;
    for (i = 0; i < m; i++) {
        if (tab[i]) {
            free(tab[i]);
        }
    }
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
int busca(Hash *tab, int m, int mat, int *achou) {
    *achou = 0;
    int end = -1;
    int pos_livre = -1;
    int k = 0;
    while (k < m) {
        end = hash_linear(mat, m, k);
        if (tab[end] != NULL && tab[end]->matricula == mat) {
            //encontrou chave
            *achou = 1;
            k = m; //força saída do loop
        }
        else {
            if (tab[end] == NULL) {
                //encontrou endereco livre
                //registra caso já não tenha sido encontrado algum anteriormente
                if (pos_livre == -1) {
                    pos_livre = end;
                }
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



/*
 * Função insere assume que end é o endereço onde será efetuada a inserção.
 *
 */
void insere(Hash *tab, int m, int mat, float cr) {
    int achou;
    int end = busca(tab, m, mat, &achou);
    if (!achou) {
        if (end != -1) {
            //Não encontrou a chave, mas encontrou posição livre
            //Inserção será realizada nessa posição livre encontrada pela busca
            tab[end] = aloca(mat, cr);
        } else {
            //Não foi encontrada posição livre durante a busca: overflow
            printf("A tabela esta cheia. Ocorreu overflow. Insercao nao realizada!\n");
        }
    } else {
        printf("Matricula ja existe. Insercao invalida! \n");
    }
}

void exclui(Hash *tab, int m, int mat) {
    int achou;
    int end = busca(tab, m, mat, &achou);
    if (achou) {
        //remove
        free(tab[end]);
        tab[end] = NULL;
    } else {
        printf("Matricula nao encontrada. Remocao nao realizada!");
    }
}

void imprime(Hash *tab, int m) {
    int i, h;
    char s[10];
    for (i = 0; i < m; i++) {
        h = i;
        printf("%d:\n", h);
        if (tab[h] != NULL) {
            TAluno *p = tab[h];
            printf("\tmat: %d\tcr: %.2f\n", p->matricula, p->cr);
        }
    }
}

int main() {
    int n, m;
    printf("Digite m (tamanho da tabela hash)... ");
    scanf("%d", &m);
    printf("Digite n (quantidade de matriculas)... ");
    scanf("%d", &n);
    
    if ((n <= 1) || (m <= 1)) return 0;
    
    Hash *tab[m];
    inicializa(tab, m);
    imprime(tab, m);

    int i, mat;
    float cr;
    for (i = 0; i < n; i++) {
        printf("Digite a matricula e o cr...\n");
        scanf("%d", &mat);
        scanf("%f", &cr);
        insere(tab, m, mat, cr);
        imprime(tab, m);
    }


    char resp;
    do {
        printf("Digite a matricula a ser removida... ");
        scanf("%d", &mat);
        exclui(tab, m, mat);
        imprime(tab, m);
        printf("Quer continuar? ");
        scanf(" %c", &resp);
    } while ((resp != 'N') && (resp != 'n'));

    do {
        printf("Digite a matricula a ser procurada... ");
        scanf("%d", &mat);
        int achou;
        int end = busca(tab, m, mat, &achou);
        if (achou == 0) printf("Elemento nao encontrado!\n");
        else {
            TAluno *p = tab[end];
            printf("%d\t%f\n", p->matricula, p->cr);
        }
        printf("Quer continuar? ");
        scanf(" %c", &resp);
    } while ((resp != 'N') && (resp != 'n'));
    imprime(tab, m);

    for (i = 0; i < n / 2; i++) {
        printf("Digite a matricula e o cr...\n");
        scanf("%d", &mat);
        scanf("%f", &cr);
        insere(tab, m, mat, cr);
        imprime(tab, m);
    }

    libera(tab, m);
    return 0;
}