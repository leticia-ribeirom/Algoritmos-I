#ifndef GRAFO_HPP
#define GRAFO_HPP

const int MAXN = 1000;

extern int r[MAXN], x[MAXN], y[MAXN];
extern int grafo[MAXN][MAXN], grau[MAXN];

void construirGrafoComGrid(int N);
bool dentro(int r, int x, int y, int px, int py);
int bfsMinimo(int inicio, int fim);
int bfsMaximo(int n);
void dfs(int u, int pai, int& tempo, int n, int tin[], int low[], bool visitado[], int corte[], int& qtd_corte);
void ordenarCortes(int corte[], int qtd_corte);

#endif
