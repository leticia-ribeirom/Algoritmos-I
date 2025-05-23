#include <iostream>
#include "grafo.hpp"

int main() {
    int Ax, Ay, Bx, By, N;
    std::cin >> Ax >> Ay >> Bx >> By >> N;

    // Leitura dos abrigos (raio e posição)
    for (int i = 0; i < N; i++) {
        std::cin >> r[i] >> x[i] >> y[i];
    }

    // Construção do grafo por Grid Espacial
    construirGrafoComGrid(N);

    // Identifica em quais abrigos Ana e Bernardo estão
    int ana = -1, bernardo = -1;
    for (int i = 0; i < N; i++) {
        // Verifica se Ana está dentro do abrigo
        if (dentro(r[i], x[i], y[i], Ax, Ay)) {
            ana = i;
        }
        // Verifica se Bernardo está dentro do abrigo 
        if (dentro(r[i], x[i], y[i], Bx, By)) {
            bernardo = i;
        } 
    }

    // Parte 1: menor número de abrigos a serem atravessados
    int s = bfsMinimo(ana, bernardo);
    std::cout << "Parte 1: " << s << std::endl;

    // Parte 2: maior distância mínima entre quaisquer dois abrigos
    int t = bfsMaximo(N);
    std::cout << "Parte 2: " << t << std::endl;

    // Parte 3: encontrar pontos de articulação no grafo
    int tin[MAXN], low[MAXN];
    bool visitado[MAXN] = {false};
    int corte[MAXN], qtd_corte = 0;
    int tempo = 0;

    // Inicialização dos vetores tin e low
    for (int i = 0; i < N; i++) {
        tin[i] = -1;
        low[i] = -1;
    }

    // Executa DFS para cada componente conexo
    for (int i = 0; i < N; i++) {
        if (!visitado[i]) {
            dfs(i, -1, tempo, N, tin, low, visitado, corte, qtd_corte);
        }
    }

    // Chama a função para ordenar os pontos de corte
    ordenarCortes(corte, qtd_corte);

    // Impressão dos pontos de corte
    std::cout << "Parte 3: " << qtd_corte;
    for (int i = 0; i < qtd_corte; i++)
        std::cout << " " << (corte[i] + 1); 
    std::cout << std::endl;

    return 0;
}
