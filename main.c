#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "algoritimos/algorithmpack.h"
#include "gerar-dados.h"
#include "algoritimos/bubblesort.h"

int main(){
    
    int dados[100], qtd_dados = 95, alt_base = 400, dis_base = 20, largura = 8, i;
    float switch_speed = 0.15;
    double t_exec = 0;
    algorithmpack pack;

    //Geração de dados
    GerarDados(dados,10,350, qtd_dados);

    //Inicialização da janela
    InitWindow(900, 450, "Visualizador de algoritimos");

    if (!IsWindowReady()) {
        return 1;
    }

        while (!WindowShouldClose())
        {
            BeginDrawing();

            ClearBackground(BLACK);

            //Exibição das colunas
            for (i = 0; i < qtd_dados; i++)
            {
                int dis_real = dis_base + (i*(largura + 1)), alt_real = alt_base - dados[i];

                if (pack.x == i || pack.y == i)
                {
                    if (pack.changed == 1)
                    {
                        DrawRectangle(dis_real, alt_real, largura, dados[i], RED);
                    }
                    else
                    {
                        DrawRectangle(dis_real, alt_real, largura, dados[i], GREEN);
                    }
                }
                else
                {
                    if (pack.sorted == 1)
                    {
                        DrawRectangle(dis_real, alt_real, largura, dados[i], GREEN);
                    }
                    else
                    {
                        DrawRectangle(dis_real, alt_real, largura, dados[i], RAYWHITE);
                    }
                }
            }
            
            EndDrawing();

            //Utilização do algoritimo pelo loop da janela
            if (GetTime() - t_exec > switch_speed)
            {
                if (switch_speed > 0.03)
                {
                    switch_speed -= 0.01;
                }
                pack = BubbleSort(dados, qtd_dados);
                t_exec = GetTime();
            }
        }

    CloseWindow();

    return 0;
}