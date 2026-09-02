#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "gerar-dados.h"
#include "algoritimos/algorithmpack.h"
#include "algoritimos/bubblesort.h"
#include "algoritimos/selectionsort.h"

int main(){

    int largura = 8, i;
    int dados[100], qtd_dados = 95, reset = 1;
    int alt_janela = 450, lar_janela = 900;
    int alt_base = 400, dis_base = (lar_janela - ((largura + 1) * qtd_dados)) / 2;

    float switch_speed = 0.15;
    double t_exec = 0;

    algorithmpack pack = {0};

    int menu = 1;
    int algoritmo_selecionado = 0;

    char velocidade_input[20] = "0.15";
    char quantidade_input[20] = "95";

    int campo_ativo = 0;

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

            Rectangle botao_bubble = {300, 130, 300, 50};
            Rectangle botao_selection = {300, 190, 300, 50};
            Rectangle botao_config = {300, 250, 300, 50};
            Rectangle botao_iniciar = {300, 330, 300, 60};

            Vector2 mouse = GetMousePosition();

            //bubble sort
            DrawRectangleRec(botao_bubble, RAYWHITE);
            DrawText("Bubble Sort", 390, 145, 20, BLACK);

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
            DrawText("Selection Sort", 380, 205, 20, BLACK);

            if (CheckCollisionPointRec(mouse, botao_selection))
            {
                DrawRectangleLinesEx(botao_selection, 3, GRAY);

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    algoritmo_selecionado = 1;
                }
            }

            //configuracoes
            DrawRectangleRec(botao_config, RAYWHITE);
            DrawText("Configuracoes", 380, 265, 20, BLACK);

            if (CheckCollisionPointRec(mouse, botao_config))
            {
                DrawRectangleLinesEx(botao_config, 3, GRAY);

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    menu = 2;
                    campo_ativo = 0;
                }
            }

            //oniciar
            DrawRectangleRec(botao_iniciar, RAYWHITE);
            DrawText("INICIAR", 410, 350, 20, BLACK);

            if (CheckCollisionPointRec(mouse, botao_iniciar))
            {
                DrawRectangleLinesEx(botao_iniciar, 3, GRAY);

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    //gera os dados na hora que clicar no iniciar
                    GerarDados(dados, 10, 350, qtd_dados);

                    reset = 1;
                    t_exec = GetTime();
                    pack = (algorithmpack){0};

                    menu = 0;
                }
            }

            EndDrawing();
        }

        else if (menu == 2)
        {
            Rectangle campo_velocidade = {400, 140, 200, 45};
            Rectangle campo_quantidade = {400, 210, 200, 45};
            Rectangle botao_aplicar = {300, 285, 300, 50};
            Rectangle botao_voltar = {300, 355, 300, 50};

            Vector2 mouse = GetMousePosition();

            //define qual campo ta clicado
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (CheckCollisionPointRec(mouse, campo_velocidade))
                {
                    campo_ativo = 1;
                }
                else if (CheckCollisionPointRec(mouse, campo_quantidade))
                {
                    campo_ativo = 2;
                }
                else
                {
                    campo_ativo = 0;
                }
            }

            //campo velocidade
            if (campo_ativo == 1)
            {
                int caractere = GetCharPressed();

                while (caractere > 0)
                {
                    //esse if e so um filtro ja que e uma string, e a mesma coisa e repitida para o campo 2
                    if ((caractere >= '0' && caractere <= '9') || caractere == '.')
                    {
                        int tamanho = 0;

                        while (velocidade_input[tamanho] != '\0')
                        {
                            tamanho++;
                        }

                        if (tamanho < 19)
                        {
                            velocidade_input[tamanho] = (char)caractere;
                            velocidade_input[tamanho + 1] = '\0';
                        }
                    }

                    caractere = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    int tamanho = 0;

                    while (velocidade_input[tamanho] != '\0')
                    {
                        tamanho++;
                    }

                    if (tamanho > 0)
                    {
                        velocidade_input[tamanho - 1] = '\0';
                    }
                }
            }

            //campo quantidade
            if (campo_ativo == 2)
            {
                int caractere = GetCharPressed();

                while (caractere > 0)
                {
                    if (caractere >= '0' && caractere <= '9')
                    {
                        int tamanho = 0;

                        while (quantidade_input[tamanho] != '\0')
                        {
                            tamanho++;
                        }

                        if (tamanho < 19)
                        {
                            quantidade_input[tamanho] = (char)caractere;
                            quantidade_input[tamanho + 1] = '\0';
                        }
                    }

                    caractere = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    int tamanho = 0;

                    while (quantidade_input[tamanho] != '\0')
                    {
                        tamanho++;
                    }

                    if (tamanho > 0)
                    {
                        quantidade_input[tamanho - 1] = '\0';
                    }
                }
            }

            //mostra tudo na tela
            BeginDrawing();

            ClearBackground(BLACK);

            DrawText("CONFIGURACOES", 310, 60, 30, RAYWHITE);

            DrawText("Velocidade:", 280, 152, 20, RAYWHITE);
            DrawText("Quantidade:", 280, 222, 20, RAYWHITE);

            DrawRectangleRec(campo_velocidade, RAYWHITE);
            DrawText(velocidade_input, 415, 152, 20, BLACK);

            DrawRectangleRec(campo_quantidade, RAYWHITE);
            DrawText(quantidade_input, 415, 222, 20, BLACK);

            //mostrar qual campo ta ativo
            if (campo_ativo == 1)
            {
                DrawRectangleLinesEx(campo_velocidade, 3, GRAY);
            }
            else
            {
                DrawRectangleLinesEx(campo_velocidade, 1, GRAY);
            }

            if (campo_ativo == 2)
            {
                DrawRectangleLinesEx(campo_quantidade, 3, GRAY);
            }
            else
            {
                DrawRectangleLinesEx(campo_quantidade, 1, GRAY);
            }

            //botao aplicar
            DrawRectangleRec(botao_aplicar, RAYWHITE);
            DrawText("APLICAR", 405, 300, 20, BLACK);

            if (CheckCollisionPointRec(mouse, botao_aplicar))
            {
                DrawRectangleLinesEx(botao_aplicar, 3, GRAY);

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    switch_speed = atof(velocidade_input);
                    qtd_dados = atoi(quantidade_input);

                    if (switch_speed < 0.01)
                    {
                        strcpy(velocidade_input, "0.01");
                        switch_speed = 0.01;
                    }
                    
                    if (qtd_dados < 3)
                    {
                        strcpy(quantidade_input, "3");
                        qtd_dados = 3;
                    }
                    else if (qtd_dados > 95)
                    {
                        strcpy(quantidade_input, "95");
                        qtd_dados = 95;
                    }

                    largura = (lar_janela - (qtd_dados - 1)) / qtd_dados;
                    dis_base = (lar_janela - ((largura + 1) * qtd_dados)) / 2;
                }
            }

            //voltar pro menu principal
            DrawRectangleRec(botao_voltar, RAYWHITE);
            DrawText("VOLTAR", 405, 370, 20, BLACK);

            if (CheckCollisionPointRec(mouse, botao_voltar))
            {
                DrawRectangleLinesEx(botao_voltar, 3, GRAY);

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    campo_ativo = 0;
                    menu = 1;
                }
            }

            EndDrawing();
        }

        //visualizador
        else
        {
            BeginDrawing();

            ClearBackground(BLACK);

            Rectangle botao_voltar = {10, 10, 90, 30};
            Vector2 mouse = GetMousePosition();

            DrawRectangleRec(botao_voltar, RAYWHITE);
            DrawText("VOLTAR", 13, 15, 20, BLACK);

            //voltar pro menu
            if (CheckCollisionPointRec(mouse, botao_voltar))
            {
                DrawRectangleLinesEx(botao_voltar, 3, GRAY);
            
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    menu = 1;
                }
            }

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
                switch (algoritmo_selecionado)
                {
                    case 0:
                        pack = BubbleSort(dados, qtd_dados, reset);
                        break;

                    case 1:
                        pack = SelectionSort(dados, qtd_dados, reset);
                        break;
                }

                reset = 0;
                t_exec = GetTime();
            }
        }
    }

    CloseWindow();

    return 0;
}