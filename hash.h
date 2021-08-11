#ifndef HASH_H
#define HASH_H

void criaDados(int m, int tam);
void inicializa(int *tab, int m);
void imprime(int *vet, int m);
int hash(int mat, int m);
int hash_linear(int mat, int m, int k); 
int hash_quadratico(int chave, int m, int k);
int hash_ii(int k, int chave);
int hash_duplo(int chave, int m, int k);
int busca(int *tab, int m, int chave, int *achou, int *colisao);
void insere(int *tab, int m, int chave);
void exclui(int *tab, int m, int chave);
int indice (int *vet, int m, int chave);
int num_primo (int num);

#endif