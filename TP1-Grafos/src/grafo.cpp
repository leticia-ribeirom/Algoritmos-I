#include "grafo.hpp"

// Vetores que armazenam raio e coordenadas dos abrigos
int r[MAXN], x[MAXN], y[MAXN];

// Representação do grafo como matriz de adjacência e grau de cada nó
int grafo[MAXN][MAXN];                                              
int grau[MAXN];

/*------------------------------- Grid espacial simples para construção do Grafo ---------------------------------*/

const int OFFSET = 10000;                                           // Para lidar com coordenadas negativas
const int CELLSIZE = 2000;                                          // Tamanho de célula (ajustável)
const int GRIDMAX = 20;                                             // Tamanho do grid (20000 / CELLSIZE = 10 ~ 20)

// Estrutura de grid
int celula[GRIDMAX][GRIDMAX][MAXN];                                 // Armazena os índices dos abrigos
int qtdCelula[GRIDMAX][GRIDMAX];                                    // Quantidade em cada célula

// Função que verifica se dois círculos (abrigos) se interceptam
bool intersect(int r1, int x1, int y1, int r2, int x2, int y2) {
    int dx = x1 - x2;                                               // Diferença de coordenadas X
    int dy = y1 - y2;                                               // Diferença de coordenadas Y
    int distSq = dx * dx + dy * dy;                                 // Distância quadrada entre os centros
    int rSum = r1 + r2;                                             // Soma dos raios dos dois círculos
    return distSq <= rSum * rSum;                                   // Se a distância entre os centros for menor ou igual à soma dos raios, eles se interceptam
}

void construirGrafoComGrid(int N) {
    // Zera a grid
    for (int i = 0; i < GRIDMAX; i++) {
        for (int j = 0; j < GRIDMAX; j++) {
            qtdCelula[i][j] = 0;
        }
    }

    // Distribui os abrigos nas células
    for (int i = 0; i < N; i++) {
        int cx = (x[i] + OFFSET) / CELLSIZE;
        int cy = (y[i] + OFFSET) / CELLSIZE;
        celula[cx][cy][qtdCelula[cx][cy]++] = i;
    }

    // Constroi o grafo baseado na grid
    for (int i = 0; i < N; i++) {
        grau[i] = 0;
        int cx = (x[i] + OFFSET) / CELLSIZE;
        int cy = (y[i] + OFFSET) / CELLSIZE;

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int nx = cx + dx;
                int ny = cy + dy;
                if (nx < 0 || ny < 0 || nx >= GRIDMAX || ny >= GRIDMAX) continue;

                for (int k = 0; k < qtdCelula[nx][ny]; k++) {
                    int j = celula[nx][ny][k];
                    if (i != j && intersect(r[i], x[i], y[i], r[j], x[j], y[j])) {
                        grafo[i][grau[i]++] = j;
                    }
                }
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------*/

// Função que verifica se um ponto (px, py) está dentro do círculo de raio r e centro (x, y)
bool dentro(int r, int x, int y, int px, int py) {
    int dx = x - px;                                                // Diferença de coordenadas X
    int dy = y - py;                                                // Diferença de coordenadas Y
    return dx * dx + dy * dy <= r * r;                              // Verifica se o ponto está dentro ou na borda do círculo
}

// Busca em largura para encontrar o menor número de abrigos entre dois pontos
int bfsMinimo(int inicio, int fim) {
    bool visitado[MAXN] = {false};                                  // Vetor para marcar os abrigos visitados
    int fila[MAXN][2];                                              // Fila para armazenar os nós e suas distâncias
    int frente = 0, tras = 0;                                       // Posições da frente e do final da fila

    fila[tras][0] = inicio;                                         // Enfileira o nó inicial
    fila[tras][1] = 0;                                              // Distância inicial é 0
    tras++;
    visitado[inicio] = true;                                        // Marca o nó inicial como visitado

    // Enquanto houver elementos na fila
    while (frente < tras) {                                         
        int atual = fila[frente][0];                                // Pega o nó da frente da fila
        int dist = fila[frente][1];                                 // Pega a distância do nó atual
        frente++;                                                   // Move a frente da fila para o próximo nó

        // Se o nó atual for o destino, retorna a distância
        if (atual == fim) {
            return dist;                                            
        }

        // Visita os vizinhos do nó atual
        for (int i = 0; i < grau[atual]; i++) {
            int vizinho = grafo[atual][i];                          // Pega um vizinho
            if (!visitado[vizinho]) {                               // Se o vizinho não foi visitado
                visitado[vizinho] = true;                           // Marca como visitado
                fila[tras][0] = vizinho;                            // Enfileira o vizinho
                fila[tras][1] = dist + 1;                           // Distância é a do atual + 1
                tras++;
            }
        }
    }

    return -1; // Caso não seja possível chegar ao destino
}

// Busca em largura para encontrar a maior distância mínima entre quaisquer dois abrigos
int bfsMaximo(int n) {
    int maxDist = 0;                                                // Inicializa a maior distância como 0
    int fila[MAXN];                                                 // Fila para armazenar os nós durante a busca

    // Executa BFS a partir de cada nó
    for (int i = 0; i < n; i++) {
        int dist[MAXN];                                             // Vetor para armazenar as distâncias de um nó até todos os outros
        
        // Inicializa as distâncias como -1 (não visitado)
        for (int j = 0; j < n; j++) {
            dist[j] = -1;                   
        }
        
        int frente = 0;
        int tras = 0;
        fila[tras++] = i;                                           // Enfileira o nó de partida
        dist[i] = 0;                                                // Distância do nó a ele mesmo é 0

        // Enquanto houver elementos na fila
        while (frente < tras) { 
            int v = fila[frente++];                                 // Pega o próximo nó da fila
            
            // Visita os vizinhos do nó
            for (int j = 0; j < grau[v]; j++) {                     
                int viz = grafo[v][j];
                
                // Se o vizinho ainda não foi visitado
                if (dist[viz] == -1) {                              
                    dist[viz] = dist[v] + 1;                        // Define a distância
                    fila[tras++] = viz;                             // Enfileira o vizinho
                    
                    if (dist[viz] > maxDist) {
                        maxDist = dist[viz];                        // Atualiza a maior distância
                    }
                }
            }
        }
    }

    return maxDist; // Retorna a maior distância encontrada
}

// Busca em profundidade para encontrar pontos de articulação (corte) no grafo
void dfs(int u, int pai, int& tempo, int n, int tin[], int low[], bool visitado[], int corte[], int& qtd_corte) {
    visitado[u] = true;                                             // Marca o nó atual como visitado
    tin[u] = low[u] = tempo++;                                      // Registra o tempo de visita do nó (tin) e o tempo de baixo-link (low)
    int filhos = 0;                                                 // Contador de filhos do nó

    // Percorre os vizinhos do nó
    for (int i = 0; i < grau[u]; i++) {                             
        int v = grafo[u][i];

        if (v == pai) {
            continue;                                               // Não volta para o nó pai
        }

        // Se o vizinho já foi visitado
        if (visitado[v]) {                                          
            // Atualiza o low-link do nó atual
            if (low[u] > tin[v]) {
                low[u] = tin[v];
            }
        } else {
            // Chama recursivamente o DFS para o vizinho
            dfs(v, u, tempo, n, tin, low, visitado, corte, qtd_corte);
            // Atualiza o low-link do nó atual após a visita do vizinho
            if (low[u] > low[v]) {
                low[u] = low[v];
            }
            // Verifica se o nó atual é um ponto de articulação
            if (low[v] >= tin[u] && pai != -1) {
                bool existe = false;
                for (int k = 0; k < qtd_corte; k++) {
                    // Verifica se o ponto já foi registrado como corte
                    if (corte[k] == u) {
                        existe = true;
                    } 
                }
                // Se não foi, adiciona ao vetor de cortes
                if (!existe) {
                    corte[qtd_corte++] = u;
                } 
            }
            filhos++; // Conta o número de filhos do nó
        }
    }

    // Caso especial: se o nó for raiz e tiver mais de um filho, é um ponto de articulação
    if (pai == -1 && filhos > 1) {
        bool existe = false;
        for (int k = 0; k < qtd_corte; k++) {
            if (corte[k] == u) {
                existe = true;
            }
        }
        if (!existe) {
            corte[qtd_corte++] = u;
        }
    }
}

void ordenarCortes(int corte[], int qtd_corte) {
    for (int i = 0; i < qtd_corte; i++) {
        for (int j = 0; j + 1 < qtd_corte; j++) {
            if (corte[j] > corte[j + 1]) {
                // Troca os pontos de corte usando o algoritmo de bolha
                int temp = corte[j];
                corte[j] = corte[j + 1];
                corte[j + 1] = temp;
            }
        }
    }
}