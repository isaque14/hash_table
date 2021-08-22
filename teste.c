#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{

    float tempo = 0;
    time_t t_ini, t_fim;
    clock_t tempoIni;

    t_ini = time(NULL);
    tempoIni = clock();
    for (int i = 0; i < 50000; i++){

        for (int  j = 0; j < 50000; j++)
        {

        }
        
    }
    t_fim = time(NULL);
    tempoIni = clock() - tempoIni;

    tempo = difftime(t_fim, t_ini);

    printf("Tempo em segundos %.2f\n", tempo);
    printf("Tempo em milessegundos %.4f\n", ((float)tempoIni)/CLOCKS_PER_SEC);
    return 0;
}
