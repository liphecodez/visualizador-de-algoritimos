# Algorithm Visualizer

Visualizador de ordenação em **C + Raylib**, com execução incremental compartilhada entre desktop e navegador via **WebAssembly**. Preserva a organização original: `main.c`, geração de dados e um módulo por algoritmo em `algoritimos/`.

## Funcionalidades

- Bubble Sort e Selection Sort preservados e corrigidos; Insertion Sort, Merge Sort e Quick Sort adicionados.
- Seleção de algoritmo, pausa/continuação, passo único e reinício com a mesma entrada. Trocar de algoritmo também preserva a entrada para facilitar comparações.
- Novos dados aleatórios, quantidade de 3 a 95 elementos e intervalo de 0,01 a 1 segundo por passo. O limite real também depende da taxa de quadros; cada passo permanece visível por pelo menos um quadro.
- Contadores, descrição e complexidades na interface. Branco: barras comuns; amarelo: comparação; coral: escrita; verde: execução concluída.
- Atalhos com foco no canvas: **Espaço** pausa/continua, **→** executa um passo e **R** reinicia.

## Métricas e complexidade

**Comparação** é um teste entre valores dos elementos (inclui a chave do Insertion e o pivô do Quick). Testes de índices e controle de laços não entram no contador.

**Movimentação** é cada escrita de um elemento no vetor principal ou auxiliar. Uma troca entre posições diferentes conta **2**; cópias para variáveis locais não contam. O Merge contabiliza tanto a escrita no auxiliar quanto a cópia de volta. O Insertion contabiliza deslocamentos e a escrita final da chave, mesmo que ela permaneça na mesma posição. Escritas do Merge no auxiliar destacam os índices de origem/destino sobre o gráfico principal; o auxiliar não é desenhado separadamente. Geração de dados e reinício não entram nas métricas.

| Algoritmo | Melhor tempo | Tempo médio | Pior tempo | Espaço auxiliar |
|---|---|---|---|---|
| Bubble Sort | O(n) | O(n²) | O(n²) | O(1) |
| Selection Sort | O(n²) | O(n²) | O(n²) | O(1) |
| Insertion Sort | O(n) | O(n²) | O(n²) | O(1) |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) |
| Quick Sort | O(n log n) | O(n log n) | O(n²) | O(n) |

As complexidades descrevem o crescimento do algoritmo com n, apesar da capacidade fixa do visualizador. O Merge é iterativo, de baixo para cima. O Quick usa partição de Lomuto com o último valor como pivô e pilha explícita de faixas (armazenamento reservado linear). Entradas ordenadas ou com muitos valores iguais podem atingir seu pior caso. Não há ordenação em JavaScript.

## Compilar e testar

Requisitos: C99, CMake ≥ 3.20, Ninja e um compilador C. O CMake obtém **Raylib 5.5** na primeira configuração; também aceita `-DFETCHCONTENT_SOURCE_DIR_RAYLIB=/caminho/raylib` para usar uma cópia local. A versão desktop usa OpenGL; a web usa WebGL 1 / OpenGL ES 2.

```sh
cmake -S . -B build/desktop -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/desktop
ctest --test-dir build/desktop --output-on-failure
```

Execute `build/desktop/visualizador.exe` no Windows ou `build/desktop/visualizador` no Linux/macOS. No Linux, instale também as dependências de desenvolvimento do ambiente gráfico exigidas pela Raylib.

Os testes não precisam de Raylib nem janela:

```sh
cmake -S . -B build/tests -G Ninja -DBUILD_VISUALIZER=OFF
cmake --build build/tests
ctest --test-dir build/tests --output-on-failure
```

## Exportação web

A exportação gerada fica em **`dist/web/`**: `index.html`, `index.js`, `index.wasm`, `.nojekyll` e a licença da Raylib. Os builds locais são preservados, mas ficam fora do Git; em um checkout novo, compile ou baixe o artefato web de uma execução bem-sucedida do GitHub Actions. Publique a pasta inteira; todos os caminhos são relativos e não há CDN, fontes ou recursos externos em tempo de execução.

Para recompilar, instale e ative o [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html). A entrega foi compilada com **Emscripten 3.1.64 e Raylib 5.5**. Em um terminal com o ambiente do SDK carregado:

```sh
emcmake cmake -S . -B build/web -G Ninja -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
cmake --build build/web
```

No PowerShell, também é possível usar `./scripts/build-web.ps1 -SdkPath C:/caminho/emsdk` (com CMake e Ninja no PATH). O parâmetro opcional `-RaylibSource` evita novo download da biblioteca.

Para validar a exportação com Python e Node.js no PATH, execute `python scripts/validate-web.py dist/web`. A verificação valida os arquivos, os caminhos relativos, as dimensões do canvas, a sintaxe JavaScript, o binário WASM e as respostas HTTP de uma hospedagem estática temporária.

O desktop mantém o `while (!WindowShouldClose())`; a web registra a mesma função de quadro com `emscripten_set_main_loop`, devolvendo o controle ao navegador entre quadros. Cada chamada dos algoritmos faz no máximo uma comparação e duas escritas, sem executar a ordenação inteira de uma vez.

Para conferir localmente:

```sh
python -m http.server 8000 --directory dist/web
```

Acesse `http://localhost:8000`. HTTP serve apenas os arquivos estáticos; não existe backend. Abrir `index.html` com `file://` pode bloquear o carregamento do WASM. O navegador precisa oferecer WebAssembly e WebGL.

### GitHub Pages e iframe

Copie **o conteúdo** de `dist/web/` para a raiz da branch usada pelo Pages, ou para `/docs` caso essa seja a pasta configurada em Settings → Pages. Alternativamente, envie `dist/web` como artefato de um workflow do Pages. O endereço final será semelhante a `https://USUARIO.github.io/REPOSITORIO/`.

```html
<iframe src="https://USUARIO.github.io/REPOSITORIO/"
        title="Algorithm Visualizer"
        style="width:100%;max-width:1124px;height:820px;border:0"
        loading="lazy"></iframe>
```

O canvas tem resolução lógica de 1100 × 740 e escala proporcional por CSS; os eventos de mouse são escalados pelo backend web. Em telas estreitas, textos e botões ficam menores; recomenda-se usar uma área ampla. Em iframe com `sandbox`, permita scripts e mesma origem (`allow-scripts allow-same-origin`) para carregar o WASM e usar o canvas normalmente.

## Estrutura e contrato dos algoritmos

- `main.c`: interface Raylib, controles e callback de quadro comum às plataformas.
- `gerar-dados.c/.h`: geração da entrada.
- `algoritimos/`: cinco máquinas de estado e `algorithmpack` (índices destacados, escrita, conclusão e métricas acumuladas).
- `tests/test_sort.c`: validação independente da interface.
- `web/shell.html`: invólucro HTML responsivo do executável.
- `CMakeLists.txt` e `scripts/build-web.ps1`: builds reproduzíveis.
- `dist/web/`: arquivos finais para hospedagem.

A API original `Algoritmo(vetor, tamanho, reset)` permanece: inicie cada execução com `reset=1` e continue com `reset=0`, mantendo vetor e tamanho até concluir ou reiniciar. Tamanhos válidos: 0 a `SORT_CAPACITY` (95); a interface utiliza 3 a 95. Cada algoritmo mantém um único estado estático, portanto não é reentrante. Depois de concluir, chamadas sem reset não modificam vetor nem métricas.

## Validação desta entrega

- 2.890 casos aprovados em C: todos os tamanhos de 0 a 95, entradas ordenadas, invertidas, iguais, duplicadas, aleatórias e extremos inteiros.
- Verificação contra `qsort`, preservação dos valores, sentinelas de limites, reinício após execução interrompida, conclusão persistente, contadores monotônicos e limite de trabalho por chamada.
- Compilação desktop Windows/MinGW e CTest aprovados. Fontes da aplicação verificadas com `-Wall -Wextra -Wpedantic -Werror`.
- Build Emscripten concluído com a Raylib compilada para web. A inspeção visual interativa no desktop/navegador não foi concluída: a ferramenta de controle local encerrou por timeout.
- Os mesmos 2.890 casos passaram em WebAssembly/Node com `ASSERTIONS=2` e `SAFE_HEAP=1`. O JavaScript gerado passou na verificação sintática, o binário passou em `WebAssembly.validate`, e HTML/JS/WASM responderam HTTP 200 com os tipos MIME esperados em hospedagem estática local.

Para repetir os testes do núcleo em WebAssembly, com o SDK ativado e Node no PATH:

```sh
emcc -std=c99 -O2 -I. tests/test_sort.c algoritimos/bubblesort.c algoritimos/selectionsort.c algoritimos/insertionsort.c algoritimos/mergesort.c algoritimos/quicksort.c -sENVIRONMENT=node -sASSERTIONS=2 -sSAFE_HEAP=1 -o build/test_sort.js
node build/test_sort.js
```

No ambiente restrito desta entrega, o LLVM não conseguiu expandir o caminho longo original. Um alias temporário de unidade com `subst` para o próprio workspace resolveu a inicialização; nenhum código do compilador ou configuração de segurança foi alterado. Em instalações normais do SDK isso não deve ser necessário.

## Desenvolvimento e integração contínua

Este repositório usa **GitHub Flow**. A `main` representa a versão estável; desenvolvimento acontece em `feature/<nome>`, correções em `fix/<nome>` e infraestrutura em `chore/<nome>`. Atualize a referência da `main` com `git fetch origin` antes de criar uma nova branch a partir de `origin/main`. Preserve alterações locais existentes e não reescreva o histórico.

Depois de implementar, execute as verificações locais, revise o diff, crie commits [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) em inglês, envie a branch e abra um Pull Request para `main`. O responsável pelo repositório revisa e faz o merge **manualmente** depois dos checks aprovados. Não use push direto na `main`, force push ou auto-merge. Não é exigida aprovação de outro colaborador.

O workflow `.github/workflows/ci.yml` roda no Ubuntu 24.04 em pushes para `main`, `feature/**`, `fix/**` e `chore/**`, e em PRs destinados à `main`. Ele possui dois checks obrigatórios:

- **Desktop build and tests**: instala dependências gráficas, compila C + Raylib com GCC 13 e executa os testes de ordenação.
- **WebAssembly build and tests**: compila C + Raylib para web com Emscripten 3.1.64, executa os mesmos testes em WASM/Node com verificações de memória e valida a exportação estática.

Os warnings `-Wall -Wextra -Wpedantic` são erros nas fontes da aplicação e dos testes. As dependências externas mantêm suas próprias opções de compilação. A Raylib 5.5 tem revisão imutável e SHA-256 verificado; as Actions são fixadas por SHA completo; CMake 3.31.6 e Ninja 1.11.1.3 têm versões explícitas. O runner e os pacotes Ubuntu recebem atualizações de segurança, portanto não se promete identidade binária entre execuções. O SDK é armazenado em cache; execuções obsoletas são canceladas. O token do workflow possui somente leitura do conteúdo.

Em **Actions → CI → execução aprovada → Artifacts**, baixe `web-wasm-<commit>` para integrar ao portfólio posteriormente ou `desktop-linux-<commit>` para obter o executável Linux (requer ambiente gráfico e bibliotecas de sistema compatíveis). Os artefatos ficam disponíveis por 14 dias; reexecute o workflow para gerar uma nova cópia. Após extrair o binário desktop, pode ser necessário executar `chmod +x visualizador`.

O CI não publica sites, não altera o repositório do portfólio e não realiza merge. Não existe workflow de deploy para este projeto. A proteção da `main` exige PR, os dois checks aprovados, branch atualizada e conversas resolvidas; bloqueia force push e exclusão e se aplica também aos administradores. Se o CI falhar, corrija a causa na própria branch do PR.
