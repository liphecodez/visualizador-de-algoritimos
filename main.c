#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "gerar-dados.h"
#include "algoritimos/algorithmpack.h"
#include "algoritimos/bubblesort.h"
#include "algoritimos/selectionsort.h"

int main(){

    int largura = 8, i;
    int dados[100], qtd_dados = 95;
    int alt_janela = 450, lar_janela = 900;
    int alt_base = 400, dis_base = (lar_janela - ((largura + 1) * qtd_dados)) / 2;

    float switch_speed = 0.15;
    double t_exec = 0;

    algorithmpack pack = {0};

    int menu = 1;
    int algoritmo_selecionado = 0;

    InitWindow(lar_janela, alt_janela, "Visualizador de algoritimos");

    if (!IsWindowReady()) {
        return 1;
    }

    while (!WindowShouldClose())
    {
        // Inicia no menu (por isso == 1)
        if (menu == 1)
        {
            BeginDrawing();

            ClearBackground(BLACK);

            DrawText("ALGORITHM VISUALIZER", 260, 60, 30, RAYWHITE);

            Rectangle botao_bubble = {300, 150, 300, 50};
            Rectangle botao_selection = {300, 220, 300, 50};
            Rectangle botao_iniciar = {300, 310, 300, 60};

            Vector2 mouse = GetMousePosition();

            //bubble sort
            DrawRectangleRec(botao_bubble, RAYWHITE);
            DrawText("Bubble Sort", 390, 165, 20, BLACK);

            if (CheckCollisionPointRec(mouse, botao_bubble))
            {
                DrawRectangleLinesEx(botao_bubble, 3, GRAY);

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    algoritmo_selecionado = 0;
                }
            }

            //selection sort
            DrawRectangleRec(botao_selection, RAYWHITE);
            DrawText("Selection Sort", 380, 235, 20, BLACK);

            if (CheckCollisionPointRec(mouse, botao_selection))
            {
                DrawRectangleLinesEx(botao_selection, 3, GRAY);

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    algoritmo_selecionado = 1;
                }
            }

            //oniciar
            DrawRectangleRec(botao_iniciar, RAYWHITE);
            DrawText("INICIAR", 410, 330, 20, BLACK);

            if (CheckCollisionPointRec(mouse, botao_iniciar))
            {
                DrawRectangleLinesEx(botao_iniciar, 3, GRAY);

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    //gera os dados na hora que clicar no iniciar
                    GerarDados(dados, 10, 350, qtd_dados);

                    switch_speed = 0.15;
                    t_exec = GetTime();
                    pack = (algorithmpack){0};

                    menu = 0;
                }
            }

            EndDrawing();
        }

        //visualizador
        else
        {
            BeginDrawing();

            ClearBackground(BLACK);

            //mostrar as colunas
            for (i = 0; i < qtd_dados; i++)
            {
                int dis_real = dis_base + (i * (largura + 1));
                int alt_real = alt_base - dados[i];

                if (pack.x == i || pack.y == i)
                {
                    if (pack.changed == 1)
                    {
                        DrawRectangle(dis_real,alt_real,largura,dados[i],RED);
                    }
                    else
                    {
                        DrawRectangle(dis_real,alt_real,largura,dados[i],GREEN);
                    }
                }
                else
                {
                    if (pack.sorted == 1)
                    {
                        DrawRectangle(dis_real,alt_real,largura,dados[i],GREEN);
                    }
                    else
                    {
                        DrawRectangle(dis_real,alt_real,largura,dados[i],RAYWHITE);
                    }
                }
            }
            EndDrawing();
            
            //executar os algoritimos com delay definido pela diferenca do gettime
            if (GetTime() - t_exec > switch_speed)
            {
                if (switch_speed > 0.05)
                {
                    switch_speed -= 0.01;
                }

                switch (algoritmo_selecionado)
                {
                    case 0:
                        pack = BubbleSort(dados, qtd_dados);
                        break;

                    case 1:
                        pack = SelectionSort(dados, qtd_dados);
                        break;
                }

                t_exec = GetTime();
            }
        }
    }

    CloseWindow();

    return 0;
}