#ifndef TABELAHASH_H
#define TABELAHASH_H

int numPrimo(int num);
int hash(int chave,int m);
double fatorCarga(char* tabela,int tam);
void criaTabela(char* tabela,int tam);
void imprimeTabela(char* tabela,int tam);

int buscaChaveLinear(int chave,char* tabela,int tam);
int insereChaveLinear(int chave,char* tabela,int tam);
void removeChaveLinear(int chave,char* tabela,int tam);

int buscaChaveQuadratico(int chave,char* tabela,int tam);
int insereChaveQuadratico(int chave,char* tabela,int tam);
void removeChaveQuadratico(int chave,char* tabela,int tam);


#endif