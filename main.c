#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "gerar-dados.h"
#include "algoritimos/bubblesort.h"

int main(){
    
    int dados[300], qtd_dados, alt_base = 400, dis_base = 20, i;
    double t_exec = 0;

    //Geração de dados
    qtd_dados = GerarDados(dados,10,350,1,100);

    //Inicialização da janela
    InitWindow(900, 450, "Visualizador de algoritimos");

        while (!WindowShouldClose())
        {
            BeginDrawing();

            ClearBackground(BLACK);

            //Exibição das colunas
            for (i = 0; i < qtd_dados; i++)
            {
                int dis_real = dis_base + (i*4), alt_real = alt_base - dados[i];

                DrawRectangle(dis_real, alt_real, 3, dados[i], RAYWHITE);
            }
            
            EndDrawing();

            //Utilização do algoritimo pelo loop da janela
            if (GetTime() - t_exec > 0.01)
            {
                BubbleSort(dados, qtd_dados);
                t_exec = GetTime();
            }
        }

    CloseWindow();

    return 0;
}