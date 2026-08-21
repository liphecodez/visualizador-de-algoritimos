#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

int main(){
    int qtd_min = 1, qtd_max = 100, qtd_dados = qtd_min + (rand() % ((qtd_max + 1) - qtd_min));
    int v_min = 1, v_max = 350, dados[300];
    int alt_base = 400, dis_base = 20, i;

    for (i = 0; i < qtd_dados; i++)
    {
        dados[i] = v_min + (rand() % ((v_max + 1) - v_min));
    }
    printf("%d", qtd_dados);

    InitWindow(900, 450, "Visualizador de algoritimos");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        for (i = 0; i < qtd_dados; i++)
        {
            int dis_real = dis_base + (i*4), alt_real = alt_base - dados[i];

            DrawRectangle(dis_real, alt_real, 3, dados[i], RAYWHITE);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}