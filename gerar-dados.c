#include <stdlib.h>

void GerarDados(int arr[], int valor_min, int valor_max, int qtd_dados){

    int i;

    for (i = 0; i < qtd_dados; i++)
    {
        arr[i] = valor_min + (rand() % ((valor_max + 1) - valor_min));
    }
    
}
