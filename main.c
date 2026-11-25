#include "algoritimos/bubblesort.h"
#include "algoritimos/insertionsort.h"
#include "algoritimos/mergesort.h"
#include "algoritimos/quicksort.h"
#include "algoritimos/selectionsort.h"
#include "algoritimos/cocktailsort.h"
#include "algoritimos/shellsort.h"
#include "algoritimos/heapsort.h"
#include "algoritimos/countingsort.h"
#include "algoritimos/radixsort.h"
#include "algoritimos/gnomesort.h"
#include "algoritimos/combsort.h"
#include "gerar-dados.h"
#include "repeat-control.h"
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
typedef struct {
    StepFunction step;
    const char *name, *category, *best, *average, *worst, *space, *description;
    int key_reads;
} Algorithm;

static const Algorithm algorithms[] = {
    {BubbleSort, "Bubble Sort", "ELEMENTARES", "O(n)", "O(n^2)", "O(n^2)", "O(1)",
     "Compara vizinhos e troca os que estao fora de ordem.", 0},
    {SelectionSort, "Selection Sort", NULL, "O(n^2)", "O(n^2)", "O(n^2)", "O(1)",
     "Encontra o menor valor e o posiciona no inicio da faixa.", 0},
    {InsertionSort, "Insertion Sort", NULL, "O(n)", "O(n^2)", "O(n^2)", "O(1)",
     "Insere cada valor na posicao correta do prefixo ordenado.", 0},
    {CocktailSort, "Cocktail Shaker Sort", NULL, "O(n)", "O(n^2)", "O(n^2)", "O(1)",
     "Troca vizinhos em passadas alternadas nas duas direcoes.", 0},
    {MergeSort, "Merge Sort", "DIVISAO E CONQUISTA", "O(n log n)", "O(n log n)", "O(n log n)",
     "O(n)", "Intercala faixas ordenadas usando um vetor auxiliar.", 0},
    {QuickSort, "Quick Sort", NULL, "O(n log n)", "O(n log n)", "O(n^2)", "O(n)",
     "Particiona os valores ao redor do ultimo elemento (pivo).", 0},
    {HeapSort, "Heap Sort", "ESTRUTURA DE DADOS", "O(n)", "O(n log n)", "O(n log n)", "O(1)",
     "Monta um heap maximo e extrai o maior valor a cada rodada.", 0},
    {CountingSort, "Counting Sort", "NAO COMPARATIVOS", "O(n + k)", "O(n + k)", "O(n + k)",
     "O(k)", "Reconstrui o vetor pelo histograma. Inteiros de 0 a 350 (k = 351).", 1},
    {RadixSort, "Radix Sort", NULL, "O(d(n + b))", "O(d(n + b))", "O(d(n + b))", "O(n + b)",
     "Distribui por digito do menos ao mais significativo (base b = 256).", 1},
    {GnomeSort, "Gnome Sort", "OUTROS", "O(n)", "O(n^2)", "O(n^2)", "O(1)",
     "Recua apos trocar vizinhos e avanca quando o par esta em ordem.", 0},
    {ShellSort, "Shell Sort", NULL, "O(n log n)", "-", "O(n^2)", "O(1)",
     "Insere por trocas em grupos separados por lacunas n/2, n/4, ...", 0},
    {CombSort, "Comb Sort", NULL, "O(n log n)", "-", "O(n^2)", "O(1)",
     "Compara pares separados por uma lacuna reduzida pelo fator 1,3.", 0},
};
static int data[SORT_CAPACITY], initial[SORT_CAPACITY], count = 50, selected = 0, needs_reset = 1,
                                                        running = 0;
enum { MIN_COUNT = 3, MIN_INTERVAL = 1, MAX_INTERVAL = 100, MAX_VALUE = COUNTING_MAX };
static int interval_cs = 5;
static float elapsed;
static Vector2 pointer;
static int pointer_pressed, pointer_held, pointer_focused, previous_touch = -1;
static double input_time;
static RepeatControl count_down, count_up, interval_down, interval_up;
static algorithmpack pack = {.x = -1, .y = -1};

#ifdef __EMSCRIPTEN__
/* Browser focus and pointer events can arrive while animation frames are suspended. */
EMSCRIPTEN_KEEPALIVE void CancelControlRepeat(void) {
    count_down.active = count_up.active = interval_down.active = interval_up.active = 0;
}
#endif
static void restart(void) {
    memcpy(data, initial, sizeof(data));
    pack = (algorithmpack){.x = -1, .y = -1};
    needs_reset = 1;
    running = 0;
    elapsed = 0;
}
static void generate(void) {
    GerarDados(initial, 10, MAX_VALUE, count);
    restart();
}
static void step(void) {
    if (!pack.sorted && !pack.invalid) {
        pack = algorithms[selected].step(data, count, needs_reset);
        needs_reset = 0;
    }
    if (pack.sorted || pack.invalid)
        running = 0;
}
static void read_pointer(void) {
    int touch = GetTouchPointCount() > 0 ? GetTouchPointId(0) : -1;
    if (touch >= 0) {
        pointer = GetTouchPosition(0);
        pointer_pressed = previous_touch == -1;
        pointer_held = previous_touch == -1 || touch == previous_touch;
    } else {
        pointer = GetMousePosition();
        pointer_pressed = previous_touch == -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        pointer_held = previous_touch == -1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    }
    /* Wait for all contacts to leave before accepting a different primary finger. */
    if (previous_touch == -1 || touch == -1)
        previous_touch = touch;
    pointer_focused = IsWindowFocused();
    input_time = GetTime();
}

static int button_hover(Rectangle r, const char *label, int active, int font_size) {
    int hover = CheckCollisionPointRec(pointer, r);
    DrawRectangleRounded(r, 0.15f, 4, active ? ACCENT : (hover ? LINE : PANEL));
    DrawRectangleRoundedLinesEx(r, 0.15f, 4, 1, active ? ACCENT : LINE);
    int width = MeasureText(label, font_size);
    DrawText(label, (int)(r.x + (r.width - width) / 2), (int)(r.y + (r.height - font_size) / 2),
             font_size, active ? BG : TEXT);
    if (hover)
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    return hover && pointer_focused;
}

static int button(Rectangle r, const char *label, int active, int font_size) {
    return button_hover(r, label, active, font_size) && pointer_pressed;
}

static int repeat_button(Rectangle r, const char *label, RepeatControl *repeat) {
    int hover = button_hover(r, label, 0, 18);
    return RepeatChanges(repeat, input_time, pointer_pressed && hover,
                         hover && (pointer_held || pointer_pressed));
}

static int clamp(int value, int min, int max) {
    return value < min ? min : (value > max ? max : value);
}
static void metric(int x, const char *label, const char *value) {
    DrawText(label, x, 126, 15, MUTED);
    DrawText(value, x, 150, 28, TEXT);
}
static void frame(void) {
    int was_sorted = pack.sorted;
    read_pointer();
    BeginDrawing();
    ClearBackground(BG);
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    DrawText("ALGORITHM", 24, 24, 23, TEXT);
    DrawText("VISUALIZER", 24, 51, 23, ACCENT);
    DrawText("C + RAYLIB / SORTING LAB", 270, 27, 14, MUTED);
    DrawLine(242, 24, 242, 710, LINE);
    int action = 0;
    int y = 104;
    for (int i = 0; i < (int)(sizeof(algorithms) / sizeof(algorithms[0])); i++) {
        if (algorithms[i].category) {
            DrawText(algorithms[i].category, 24, y + 3, 13, MUTED);
            y += 20;
        }
        if (button((Rectangle){24, y, 194, 25}, algorithms[i].name, selected == i, 16)) {
            selected = i;
            restart();
            action = 1;
        }
        y += 29;
    }
    DrawText("ELEMENTOS (3 A 95)", 24, 562, 14, MUTED);
    int fewer = repeat_button((Rectangle){24, 581, 44, 31}, "-", &count_down);
    int more = repeat_button((Rectangle){174, 581, 44, 31}, "+", &count_up);
    int new_count = clamp(count + more - fewer, MIN_COUNT, SORT_CAPACITY);
    if (new_count != count) {
        count = new_count;
        generate();
        action = 1;
    }
    DrawText(TextFormat("%d", count), 99, 587, 20, TEXT);

    DrawText("INTERVALO / PASSO", 24, 624, 14, MUTED);
    int faster = repeat_button((Rectangle){24, 643, 44, 31}, "-", &interval_down);
    int slower = repeat_button((Rectangle){174, 643, 44, 31}, "+", &interval_up);
    interval_cs = clamp(interval_cs + slower - faster, MIN_INTERVAL, MAX_INTERVAL);
    DrawText(TextFormat("%.2f s", interval_cs / 100.0), 84, 649, 19, TEXT);
    DrawText("Menor intervalo = mais rapido", 24, 682, 12, MUTED);
    DrawText("Espaco: pausar / continuar", 24, 700, 13, MUTED);
    DrawText("Seta direita: passo   R: reiniciar", 24, 717, 12, MUTED);
    /* Keep every visible step on screen for at least one frame. */
    if (button((Rectangle){270, 210, 170, 40},
               pack.sorted ? "Concluido"
               : running   ? "Pausar"
                           : "Continuar",
               !running && !pack.sorted, 18) &&
        !pack.sorted) {
        running = !running;
        elapsed = 0;
        action = 1;
    }
    if (button((Rectangle){452, 210, 140, 40}, "Um passo", 0, 18)) {
        running = 0;
        elapsed = 0;
        step();
        action = 1;
    }
    if (button((Rectangle){604, 210, 166, 40}, "Reiniciar", 0, 18)) {
        restart();
        action = 1;
    }
    if (button((Rectangle){782, 210, 170, 40}, "Novos dados", 0, 18)) {
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
        if (elapsed >= interval_cs / 100.0f) {
            elapsed = 0;
            step();
        }
    }
    DrawText(algorithms[selected].name, 270, 54, 32, TEXT);
    DrawText(pack.sorted ? "CONCLUIDO"
             : running   ? "EM EXECUCAO"
                         : "PAUSADO",
             900, 62, 17, pack.sorted ? ACCENT : COMPARE);
    DrawRectangleRounded((Rectangle){270, 111, 806, 82}, 0.12f, 4, PANEL);
    metric(290, "COMPARACOES", TextFormat("%llu", (unsigned long long)pack.comparisons));
    metric(570, "MOVIMENTACOES", TextFormat("%llu", (unsigned long long)pack.movements));
    metric(876, "ELEMENTOS", TextFormat("%d", count));
    DrawRectangleRounded((Rectangle){270, 267, 806, 284}, 0.025f, 4, PANEL);
    for (int y = 0; y < 3; y++)
        DrawLine(286, 295 + y * 112, 1060, 295 + y * 112, LINE);
    float barWidth = 774.0f / count;
    int just_finished = !was_sorted && pack.sorted;
    for (int i = 0; i < count; i++) {
        Color color = pack.sorted ? ACCENT : TEXT;
        if ((!pack.sorted || just_finished) && (pack.x == i || pack.y == i))
            color = pack.changed ? WRITE : COMPARE;
        float h = data[i] / (float)MAX_VALUE * 240;
        DrawRectangleRec(
            (Rectangle){286 + i * barWidth, 535 - h, barWidth > 4 ? barWidth - 2 : barWidth - 1, h},
            color);
    }
    DrawText(algorithms[selected].key_reads ? "Lendo digito / chave" : "Comparando", 292, 564, 14, COMPARE);
    DrawText("Escrevendo", 430, 564, 14, WRITE);
    DrawText("Ordenacao concluida", 568, 564, 14, ACCENT);
    DrawText(algorithms[selected].description, 270, 596, 16, TEXT);
    DrawText(TextFormat("TEMPO   Melhor: %s    Medio: %s    Pior: %s", algorithms[selected].best,
                        algorithms[selected].average, algorithms[selected].worst),
             270, 627, 16, MUTED);
    DrawText(TextFormat("ESPACO AUXILIAR   %s", algorithms[selected].space), 270, 651, 16, MUTED);
    DrawText("Comparacao = teste entre valores. Movimento = escrita em vetor "
             "(inclui auxiliar).",
             270, 686, 14, MUTED);
    DrawText(algorithms[selected].key_reads
                 ? "Leituras de chaves e atualizacoes de frequencias nao contam como comparacoes."
                 : "Uma troca conta 2 movimentos. Reiniciar preserva a entrada e zera os contadores.",
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
