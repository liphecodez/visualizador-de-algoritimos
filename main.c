#include "algoritimos/bubblesort.h"
#include "algoritimos/insertionsort.h"
#include "algoritimos/mergesort.h"
#include "algoritimos/quicksort.h"
#include "algoritimos/selectionsort.h"
#include "gerar-dados.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif
#define WIDTH 1100
#define HEIGHT 740
static const Color BG = {15, 20, 29, 255}, PANEL = {23, 31, 43, 255}, LINE = {43, 56, 73, 255};
static const Color TEXT = {231, 238, 247, 255}, MUTED = {151, 169, 191, 255},
                   ACCENT = {80, 214, 182, 255};
static const Color COMPARE = {247, 202, 107, 255}, WRITE = {255, 129, 123, 255};
typedef algorithmpack (*StepFunction)(int *, int, int);
static const StepFunction functions[] = {BubbleSort, SelectionSort, InsertionSort, MergeSort,
                                         QuickSort};
static const char *names[] = {"Bubble Sort", "Selection Sort", "Insertion Sort", "Merge Sort",
                              "Quick Sort"};
static const char *best[] = {"O(n)", "O(n^2)", "O(n)", "O(n log n)", "O(n log n)"};
static const char *average[] = {"O(n^2)", "O(n^2)", "O(n^2)", "O(n log n)", "O(n log n)"};
static const char *worst[] = {"O(n^2)", "O(n^2)", "O(n^2)", "O(n log n)", "O(n^2)"};
static const char *space[] = {"O(1)", "O(1)", "O(1)", "O(n)", "O(n)"};
static const char *description[] = {"Compara vizinhos e troca os que estao fora de ordem.",
                                    "Encontra o menor valor e o posiciona no inicio da faixa.",
                                    "Insere cada valor na posicao correta do prefixo ordenado.",
                                    "Intercala faixas ordenadas usando um vetor auxiliar.",
                                    "Particiona os valores ao redor do ultimo elemento (pivo)."};
static int data[SORT_CAPACITY], initial[SORT_CAPACITY], count = 50, selected = 0, needs_reset = 1,
                                                        running = 0;
static float interval = 0.05f, elapsed = 0;
static algorithmpack pack = {.x = -1, .y = -1};
static void restart(void) {
    memcpy(data, initial, sizeof(data));
    pack = (algorithmpack){.x = -1, .y = -1};
    needs_reset = 1;
    running = 0;
    elapsed = 0;
}
static void generate(void) {
    GerarDados(initial, 10, 350, count);
    restart();
}
static void step(void) {
    if (!pack.sorted) {
        pack = functions[selected](data, count, needs_reset);
        needs_reset = 0;
    }
    if (pack.sorted)
        running = 0;
}
static int button(Rectangle r, const char *label, int active) {
    int hover = CheckCollisionPointRec(GetMousePosition(), r);
    DrawRectangleRounded(r, 0.15f, 4, active ? ACCENT : (hover ? LINE : PANEL));
    DrawRectangleRoundedLinesEx(r, 0.15f, 4, 1, active ? ACCENT : LINE);
    int width = MeasureText(label, 18);
    DrawText(label, (int)(r.x + (r.width - width) / 2), (int)(r.y + (r.height - 18) / 2), 18,
             active ? BG : TEXT);
    if (hover)
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    return hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}
static void metric(int x, const char *label, const char *value) {
    DrawText(label, x, 126, 15, MUTED);
    DrawText(value, x, 150, 28, TEXT);
}
static void frame(void) {
    BeginDrawing();
    ClearBackground(BG);
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    DrawText("ALGORITHM", 24, 24, 23, TEXT);
    DrawText("VISUALIZER", 24, 51, 23, ACCENT);
    DrawText("C + RAYLIB / SORTING LAB", 270, 27, 14, MUTED);
    DrawText(names[selected], 270, 54, 32, TEXT);
    DrawText(pack.sorted ? "CONCLUIDO"
             : running   ? "EM EXECUCAO"
                         : "PAUSADO",
             900, 62, 17, pack.sorted ? ACCENT : COMPARE);
    DrawLine(242, 24, 242, 710, LINE);
    DrawText("ALGORITMOS", 24, 112, 15, MUTED);
    int action = 0;
    for (int i = 0; i < 5; i++)
        if (button((Rectangle){24, 143 + i * 48, 194, 38}, names[i], selected == i)) {
            selected = i;
            restart();
            action = 1;
        }
    DrawText("ELEMENTOS", 24, 413, 15, MUTED);
    if (button((Rectangle){24, 440, 44, 34}, "-", 0)) {
        count = count > 3 ? count - 1 : 3;
        generate();
        action = 1;
    }
    DrawText(TextFormat("%d", count), 99, 448, 20, TEXT);
    if (button((Rectangle){174, 440, 44, 34}, "+", 0)) {
        count = count < SORT_CAPACITY ? count + 1 : SORT_CAPACITY;
        generate();
        action = 1;
    }
    DrawText("INTERVALO / PASSO", 24, 505, 15, MUTED);
    if (button((Rectangle){24, 532, 44, 34}, "-", 0)) {
        interval -= 0.01f;
        if (interval < 0.01f)
            interval = 0.01f;
        elapsed = 0;
        action = 1;
    }
    DrawText(TextFormat("%.2f s", interval), 84, 540, 19, TEXT);
    if (button((Rectangle){174, 532, 44, 34}, "+", 0)) {
        interval += 0.01f;
        if (interval > 1)
            interval = 1;
        elapsed = 0;
        action = 1;
    }
    DrawText("Menor intervalo = mais rapido", 24, 579, 12, MUTED);
    DrawText("3 a 95 elementos", 24, 603, 14, MUTED);
    DrawText("Espaco  pausar / continuar", 24, 661, 13, MUTED);
    DrawText("Seta direita  um passo", 24, 683, 13, MUTED);
    DrawRectangleRounded((Rectangle){270, 111, 806, 82}, 0.12f, 4, PANEL);
    metric(290, "COMPARACOES", TextFormat("%llu", (unsigned long long)pack.comparisons));
    metric(570, "MOVIMENTACOES", TextFormat("%llu", (unsigned long long)pack.movements));
    metric(876, "ELEMENTOS", TextFormat("%d", count));
    /* Keep every visible step on screen for at least one frame. */
    if (button((Rectangle){270, 210, 170, 40},
               pack.sorted ? "Concluido"
               : running   ? "Pausar"
                           : "Continuar",
               !running && !pack.sorted) &&
        !pack.sorted) {
        running = !running;
        elapsed = 0;
        action = 1;
    }
    if (button((Rectangle){452, 210, 140, 40}, "Um passo", 0)) {
        running = 0;
        elapsed = 0;
        step();
        action = 1;
    }
    if (button((Rectangle){604, 210, 166, 40}, "Reiniciar", 0)) {
        restart();
        action = 1;
    }
    if (button((Rectangle){782, 210, 170, 40}, "Novos dados", 0)) {
        generate();
        action = 1;
    }
    if (IsKeyPressed(KEY_SPACE) && !pack.sorted) {
        running = !running;
        elapsed = 0;
        action = 1;
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        running = 0;
        elapsed = 0;
        step();
        action = 1;
    }
    if (IsKeyPressed(KEY_R)) {
        restart();
        action = 1;
    }
    if (!action && running) {
        elapsed += GetFrameTime();
        if (elapsed >= interval) {
            elapsed = 0;
            step();
        }
    }
    DrawRectangleRounded((Rectangle){270, 267, 806, 284}, 0.025f, 4, PANEL);
    for (int y = 0; y < 3; y++)
        DrawLine(286, 295 + y * 112, 1060, 295 + y * 112, LINE);
    float barWidth = 774.0f / count;
    for (int i = 0; i < count; i++) {
        Color color = pack.sorted ? ACCENT : TEXT;
        if (!pack.sorted && (pack.x == i || pack.y == i))
            color = pack.changed ? WRITE : COMPARE;
        float h = data[i] / 350.0f * 240;
        DrawRectangleRec(
            (Rectangle){286 + i * barWidth, 535 - h, barWidth > 4 ? barWidth - 2 : barWidth - 1, h},
            color);
    }
    DrawText("Comparando", 292, 564, 14, COMPARE);
    DrawText("Escrevendo", 430, 564, 14, WRITE);
    DrawText("Ordenacao concluida", 568, 564, 14, ACCENT);
    DrawText(description[selected], 270, 596, 16, TEXT);
    DrawText(TextFormat("TEMPO   Melhor: %s    Medio: %s    Pior: %s", best[selected],
                        average[selected], worst[selected]),
             270, 627, 16, MUTED);
    DrawText(TextFormat("ESPACO AUXILIAR   %s", space[selected]), 270, 651, 16, MUTED);
    DrawText("Comparacao = teste entre valores. Movimento = escrita em vetor "
             "(inclui auxiliar).",
             270, 686, 14, MUTED);
    DrawText("Uma troca conta 2 movimentos. Reiniciar preserva a entrada e zera "
             "os contadores.",
             270, 707, 13, MUTED);
    EndDrawing();
}
int main(void) {
    srand((unsigned)time(NULL));
    InitWindow(WIDTH, HEIGHT, "Algorithm Visualizer | C + Raylib");
    if (!IsWindowReady())
        return 1;
    SetTargetFPS(60);
    generate();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(frame, 0, 1);
#else
    while (!WindowShouldClose())
        frame();
    CloseWindow();
#endif
    return 0;
}
