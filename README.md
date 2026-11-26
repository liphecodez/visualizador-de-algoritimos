# Algorithm Visualizer

Visualizador de ordenação em **C + Raylib**, com execução incremental compartilhada entre desktop e navegador via **WebAssembly**. Preserva a organização original: `main.c`, geração de dados e um módulo por algoritmo em `algoritimos/`.

## Funcionalidades

- Doze algoritmos de ordenação, agrupados por categoria na barra lateral.
- Seleção de algoritmo, pausa/continuação, passo único e reinício com a mesma entrada. Trocar de algoritmo também preserva a entrada para facilitar comparações.
- Novos dados aleatórios, quantidade de 3 a 95 elementos e intervalo de 0,01 a 1 segundo por passo. O limite real também depende da taxa de quadros; cada passo permanece visível por pelo menos um quadro.
- Contadores, descrição e complexidades na interface. Branco: barras comuns; amarelo: comparação; coral: escrita; verde: execução concluída.
- Os botões **−** e **+** ajustam a quantidade ou o intervalo em passos de 1 elemento ou 0,01 s. Um toque ou clique aplica o primeiro ajuste imediatamente. Ao segurar, a repetição começa após cerca de 350 ms, com intervalos que diminuem de 100 ms até 25 ms. Soltar, sair do botão ou perder o foco cancela a repetição. Alterar a quantidade gera uma nova entrada, zera as métricas e deixa a execução pausada. Um ajuste no limite mantém a entrada atual. Alterar a velocidade mantém o estado da ordenação.
- Atalhos com foco no canvas: **Espaço** pausa/continua, **→** executa um passo e **R** reinicia.

## Métricas e complexidade

**Comparação** é um teste entre valores dos elementos (inclui a chave do Insertion e o pivô do Quick). Testes de índices e controle de laços não entram no contador.

**Movimentação** é cada escrita de um elemento no vetor principal ou auxiliar. Uma troca entre posições diferentes conta **2**; cópias para variáveis locais não contam. O Merge contabiliza tanto a escrita no auxiliar quanto a cópia de volta. O Insertion contabiliza deslocamentos e a escrita final da chave, mesmo que ela permaneça na mesma posição. Escritas do Merge no auxiliar destacam os índices de origem/destino sobre o gráfico principal; o auxiliar não é desenhado separadamente. Geração de dados e reinício não entram nas métricas.

| Algoritmo | Melhor tempo | Tempo médio | Pior tempo | Espaço auxiliar | Característica |
|---|---|---|---|---|---|
| Bubble Sort | O(n) | O(n²) | O(n²) | O(1) | Trocas adjacentes e parada antecipada |
| Selection Sort | O(n²) | O(n²) | O(n²) | O(1) | Seleciona o mínimo de cada faixa |
| Insertion Sort | O(n) | O(n²) | O(n²) | O(1) | Insere a chave no prefixo ordenado |
| Cocktail Shaker Sort | O(n) | O(n²) | O(n²) | O(1) | Passadas adjacentes nas duas direções |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) | Intercala faixas de baixo para cima |
| Quick Sort | O(n log n) | O(n log n) | O(n²) | O(n) | Partição de Lomuto e pilha explícita |
| Heap Sort | O(n) | O(n log n) | O(n log n) | O(1) | Constrói um heap máximo e extrai o topo |
| Counting Sort | O(n + k) | O(n + k) | O(n + k) | O(k) | Reconstrói os valores a partir das frequências |
| Radix Sort | O(d(n + b)) | O(d(n + b)) | O(d(n + b)) | O(n + b) | Distribuição estável do dígito menos ao mais significativo |
| Gnome Sort | O(n) | O(n²) | O(n²) | O(1) | Troca vizinhos e recua ao encontrar inversões |
| Shell Sort | O(n log n) | — | O(n²) | O(1) | Inserção por trocas com lacunas reduzidas à metade |
| Comb Sort | O(n log n) | — | O(n²) | O(1) | Reduz a lacuna pelo fator 1,3 e termina com passadas adjacentes |

A tabela descreve as variantes implementadas. O travessão indica que não foi adotada uma estimativa de caso médio para a sequência de lacunas do Shell ou para o Comb. O melhor caso do Heap pode ocorrer com chaves iguais. O Quick usa o último elemento como pivô e reserva espaço linear para as faixas pendentes. Entradas ordenadas ou com muitos valores iguais podem atingir seu pior caso.

O Counting aceita inteiros de **0 a 350**, com **k = 351** posições no histograma. Fora desse intervalo, informa `invalid` antes de modificar o vetor. O Radix usa **b = 256** e percorre **d** dígitos da representação sem sinal, com ajuste para cobrir todo o intervalo de `int`, inclusive negativos. As comparações entre elementos ficam em **zero** nesses dois algoritmos. Leituras de chaves aparecem em amarelo. Atualizações de frequências e deslocamentos de baldes são metadados, então ficam fora das métricas. O Counting conta uma escrita por elemento reconstruído. O Radix conta as escritas para o auxiliar e de volta ao vetor principal em cada dígito.

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

Para recompilar, instale e ative o [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html). O CI usa **Emscripten 3.1.64 e Raylib 5.5**. Em um terminal com o ambiente do SDK carregado:

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
- `algoritimos/`: doze máquinas de estado e `algorithmpack` (índices destacados, escrita, conclusão e métricas acumuladas).
- `tests/test_sort.c`: validação independente da interface.
- `web/shell.html`: invólucro HTML responsivo do executável.
- `CMakeLists.txt` e `scripts/build-web.ps1`: builds reproduzíveis.
- `dist/web/`: arquivos finais para hospedagem.

A API original `Algoritmo(vetor, tamanho, reset)` permanece: inicie cada execução com `reset=1` e continue com `reset=0`, mantendo vetor e tamanho até concluir ou reiniciar. Tamanhos válidos: 0 a `SORT_CAPACITY` (95); a interface utiliza 3 a 95. Cada algoritmo mantém um único estado estático, portanto não é reentrante. Depois de concluir, chamadas sem reset não modificam vetor nem métricas.

## Validação automatizada

A suíte versionada cobre Bubble, Selection, Insertion, Merge e Quick. Ela compara a saída com `qsort` em todos os tamanhos de 0 a 95 e verifica limites, reinício, contadores e trabalho por passo.

Para executar a mesma suíte em WebAssembly com Node.js e verificações de memória, ative o SDK e rode:

```sh
emcmake cmake -S . -B build/wasm-tests -G Ninja -DBUILD_VISUALIZER=OFF -DBUILD_TESTING=ON
cmake --build build/wasm-tests
ctest --test-dir build/wasm-tests --output-on-failure
```

## Desenvolvimento e integração contínua

Este repositório usa **GitHub Flow**. A `main` representa a versão estável; desenvolvimento acontece em `feature/<nome>`, correções em `fix/<nome>` e infraestrutura em `chore/<nome>`. Atualize a referência da `main` com `git fetch origin` antes de criar uma nova branch a partir de `origin/main`. Preserve alterações locais existentes e não reescreva o histórico.

Depois de implementar, execute as verificações locais, revise o diff, crie commits [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) em inglês, envie a branch e abra um Pull Request para `main`. O responsável pelo repositório revisa e faz o merge **manualmente** depois dos checks aprovados. Não use push direto na `main`, force push ou auto-merge. Não é exigida aprovação de outro colaborador.

O workflow `.github/workflows/ci.yml` roda no Ubuntu 24.04 em pushes para `main`, `feature/**`, `fix/**` e `chore/**`, e em PRs destinados à `main`. Ele possui dois checks obrigatórios:

- **Desktop build and tests**: instala dependências gráficas, compila C + Raylib com GCC 13 e executa os testes de ordenação.
- **WebAssembly build and tests**: compila C + Raylib para web com Emscripten 3.1.64, executa os mesmos testes em WASM/Node com verificações de memória e valida a exportação estática.

Os warnings `-Wall -Wextra -Wpedantic` são erros nas fontes da aplicação e dos testes. As dependências externas mantêm suas próprias opções de compilação. A Raylib 5.5 tem revisão imutável e SHA-256 verificado; as Actions são fixadas por SHA completo; CMake 3.31.6 e Ninja 1.11.1.3 têm versões explícitas. O runner e os pacotes Ubuntu recebem atualizações de segurança, portanto não se promete identidade binária entre execuções. O SDK é armazenado em cache; execuções obsoletas são canceladas. O token do workflow possui somente leitura do conteúdo.

Em **Actions → CI → execução aprovada → Artifacts**, baixe `web-wasm-<commit>` para integrar ao portfólio posteriormente ou `desktop-linux-<commit>` para obter o executável Linux (requer ambiente gráfico e bibliotecas de sistema compatíveis). Os artefatos ficam disponíveis por 14 dias; reexecute o workflow para gerar uma nova cópia. Após extrair o binário desktop, pode ser necessário executar `chmod +x visualizador`.

O CI não publica sites, não altera o repositório do portfólio e não realiza merge. Não existe workflow de deploy para este projeto. A proteção da `main` exige PR, os dois checks aprovados, branch atualizada e conversas resolvidas; bloqueia force push e exclusão e se aplica também aos administradores. Se o CI falhar, corrija a causa na própria branch do PR.
