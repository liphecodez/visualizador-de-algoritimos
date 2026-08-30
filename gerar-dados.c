#include <stdlib.h>

int GerarDados(int arr[], int valor_min, int valor_max, int qtd_min, int qtd_max){

    int i, qtd_dados = qtd_min + (rand() % ((qtd_max + 1) - qtd_min));

    for (i = 0; i < qtd_dados; i++)
    {
        arr[i] = valor_min + (rand() % ((valor_max + 1) - valor_min));
    }
    
    return qtd_dados;
}
