# Algorithm Visualizer

Visualizador de algoritmos de ordenação feito em C com [raylib](https://www.raylib.com/), que mostra visualmente, passo a passo, como o **Bubble Sort** e o **Selection Sort** ordenam um conjunto de dados gerado aleatoriamente.

## Sobre o projeto

O programa gera uma lista de valores aleatórios e desenha cada valor como uma barra na tela. A cada passo do algoritmo escolhido, uma nova "operação" (comparação/troca) é executada e a tela é atualizada, deixando visível:

- 🟩 **Verde**: elementos já ordenados (ou envolvidos em uma comparação sem troca)
- 🟥 **Vermelho**: elementos que acabaram de ser trocados de posição
- ⬜ **Branco**: elementos ainda não processados

O objetivo é puramente didático: entender de forma visual e intuitiva como cada algoritmo de ordenação se comporta, além de aprender enquanto desenvolvo.

## Como funciona

O núcleo do projeto é baseado em uma técnica de **execução em passos (step-by-step)**: em vez de rodar o algoritmo de ordenação inteiro de uma vez, cada algoritmo é implementado como uma função que, a cada chamada, executa **apenas uma operação** (uma comparação, ou uma comparação + troca) e retorna o estado atual dessa operação. Isso é o que permite desenhar a tela entre uma operação e outra, criando a animação.

Esse estado da operação é representado pela struct `algorithmpack`:

```c
typedef struct
{
    int x, y, changed, sorted;
} algorithmpack;
```

- `x`, `y`: os dois índices envolvidos na operação atual (para saber quais barras destacar)
- `changed`: indica se houve troca entre `x` e `y`
- `sorted`: indica se o algoritmo já terminou de ordenar tudo

Cada algoritmo mantém seu **próprio estado interno** entre chamadas (através de uma struct `static`), guardando coisas como a rodada atual e a posição atual da varredura. Por isso as funções recebem um parâmetro `reset`: quando `reset == 1`, o algoritmo reinicia seu estado do zero (usado toda vez que uma nova execução começa).

## Estrutura dos arquivos

```
.
├── main.c                  # Loop principal, menu, tela de configurações e visualizador
├── gerar-dados.c/.h        # Gera os dados aleatórios que serão ordenados
├── algoritimos/
│   ├── algorithmpack.h     # Struct compartilhada que representa o estado de uma operação
│   ├── bubblesort.c/.h     # Implementação passo a passo do Bubble Sort
│   └── selectionsort.c/.h  # Implementação passo a passo do Selection Sort
```

> **Nota:** o `main.c` importa os algoritmos de dentro de uma pasta `algoritimos/`. Se for compilar, garanta que `algorithmpack.h`, `bubblesort.c/.h` e `selectionsort.c/.h` estejam dentro dessa pasta (ou ajuste os `#include` conforme sua organização).

## Telas do programa

1. **Menu principal**: escolha entre Bubble Sort e Selection Sort, acesse as configurações ou aperte "INICIAR" para começar a visualização (os dados são gerados aleatoriamente nesse momento).
2. **Configurações**: permite ajustar:
   - **Velocidade**: define **a cada quanto tempo (em segundos) uma operação é executada**. Por exemplo, `0.15` significa que uma nova comparação/troca acontece a cada 0,15 segundos. Quanto menor o valor, mais rápida a visualização (mínimo permitido: `0.01`).
   - **Quantidade**: quantos elementos serão gerados e ordenados (entre 3 e 95).
3. **Visualizador**: mostra a animação da ordenação em tempo real, com um botão "VOLTAR" para retornar ao menu.

## Dependências

- [raylib](https://www.raylib.com/) — biblioteca gráfica usada para criar a janela, desenhar e capturar entrada do usuário/mouse.
- Compilador C (GCC/Clang/MinGW etc.)

## Como compilar

Exemplo usando `gcc` (ajuste os caminhos da raylib conforme sua instalação):

```bash
gcc main.c gerar-dados.c algoritimos/bubblesort.c algoritimos/selectionsort.c -o visualizador -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

No Windows (MinGW), os flags de link costumam ser:

```bash
gcc main.c gerar-dados.c algoritimos/bubblesort.c algoritimos/selectionsort.c -o visualizador.exe -lraylib -lopengl32 -lgdi32 -lwinmm
```

## Como usar

1. Compile o projeto.
2. Execute o binário gerado.
3. No menu, escolha o algoritmo desejado.
4. (Opcional) Vá em "Configuracoes" para ajustar velocidade e quantidade de dados.
5. Clique em "INICIAR" para ver a ordenação acontecendo.

## Visão do projeto / Próximos passos

Essa v1 é o MVP do projeto (mais detalhes na seção de requisitos abaixo), mas a ideia é que ele continue crescendo. Alguns pontos que pretendo abordar no futuro:

- Adicionar novos algoritmos de ordenação (Insertion Sort, Quick Sort, Merge Sort etc.)
- Trazer explicações teóricas sobre cada algoritmo (como funciona, complexidade de tempo/espaço) direto na interface ou como material de apoio, para reforçar o caráter didático do projeto
- Contador de comparações/trocas na tela
- Modo de comparação lado a lado entre dois algoritmos

Essas são intenções de direção, não um compromisso fechado de entrega — o escopo pode mudar conforme o projeto evolui.

## Requisitos

O escopo dessa v1 (MVP) foi definido a partir de um levantamento de requisitos feito com a técnica **MoSCoW** (Must have, Should have, Could have, Won't have), bastante usada no mercado para priorizar o que entra em uma primeira entrega e o que fica para depois.

> O documento original desse levantamento foi perdido, então a lista abaixo foi reconstruída com base no que efetivamente foi implementado nesta versão.

**Must have (essencial para o MVP)**
- Gerar um conjunto de dados aleatórios para ordenar
- Implementar pelo menos dois algoritmos de ordenação (Bubble Sort e Selection Sort)
- Executar a ordenação passo a passo, exibindo visualmente cada operação (comparação/troca)
- Diferenciar visualmente elementos trocados, elementos comparados/ordenados e elementos ainda não processados
- Permitir iniciar uma nova execução com dados novos

**Should have (importante, mas não bloqueia o MVP)**
- Tela de configurações para ajustar a velocidade da visualização
- Tela de configurações para ajustar a quantidade de dados gerados
- Navegação simples entre menu, configurações e visualizador (botões de voltar)

**Could have (desejável, se sobrar tempo)**
- Feedback visual ao passar o mouse sobre os botões (hover)
- Validação/ajuste automático de valores inválidos nos campos de configuração

**Won't have (fora do escopo desta versão)**
- Outros algoritmos além de Bubble Sort e Selection Sort
- Explicações teóricas dos algoritmos na interface
- Contadores de métricas (número de comparações, trocas, tempo total)
- Comparação simultânea entre dois algoritmos
