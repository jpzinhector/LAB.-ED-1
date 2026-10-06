/*
 * Atividade 7 - Complexidade de Algoritmos: Busca e Ordenação com raylib
 * Exercícios 1 e 2 implementados:
 * - Inclusão do Insertion Sort (tecla I)
 * - Medição de tempo real em milissegundos com GetTime()
 */

#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define LARGURA_JANELA  800
#define ALTURA_JANELA   600
#define TOTAL_PLACARES  15
#define PONTUACAO_MAX   100

typedef struct {
    char nome[16];
    int  pontuacao;
} Placar;

Placar *criarPlacares(int quantidade) {
    Placar *placares = (Placar *)malloc(quantidade * sizeof(Placar));
    if (placares == NULL) return NULL;

    for (int i = 0; i < quantidade; i++) {
        Placar *p = (placares + i);
        TextCopy(p->nome, TextFormat("J%02d", i + 1));
        p->pontuacao = GetRandomValue(10, PONTUACAO_MAX);
    }
    return placares;
}

/* ---- Bubble Sort: O(n^2) ---- */
void ordenarBubbleSort(Placar *vetor, int n, long *comparacoes, long *trocas) {
    *comparacoes = 0;
    *trocas = 0;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparacoes)++;
            if (vetor[j].pontuacao > vetor[j + 1].pontuacao) {
                Placar temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
                (*trocas)++;
            }
        }
    }
}

/* ---- Exercício 1: Insertion Sort: O(n^2) no pior caso, O(n) no melhor ---- */
void ordenarInsertionSort(Placar *vetor, int n, long *comparacoes, long *trocas) {
    *comparacoes = 0;
    *trocas = 0;

    for (int i = 1; i < n; i++) {
        Placar chave = vetor[i];
        int j = i - 1;

        // Cada teste da condição do while representa uma comparação
        while (j >= 0) {
            (*comparacoes)++;
            if (vetor[j].pontuacao > chave.pontuacao) {
                vetor[j + 1] = vetor[j]; // Deslocamento
                (*trocas)++;
                j--;
            } else {
                break; // Elemento na posição correta
            }
        }
        vetor[j + 1] = chave;
    }
}

/* ---- Busca Sequencial: O(n) ---- */
int buscaSequencial(Placar *vetor, int n, int alvo, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n; i++) {
        (*comparacoes)++;
        if (vetor[i].pontuacao == alvo) return i;
    }
    return -1;
}

/* ---- Busca Binária: O(log n), exige vetor ordenado ---- */
int buscaBinaria(Placar *vetor, int n, int alvo, long *comparacoes) {
    *comparacoes = 0;
    int inicio = 0, fim = n - 1;

    while (inicio <= fim) {
        (*comparacoes)++;
        int meio = (inicio + fim) / 2;

        if (vetor[meio].pontuacao == alvo) return meio;
        if (vetor[meio].pontuacao < alvo) inicio = meio + 1;
        else                              fim = meio - 1;
    }
    return -1;
}

void desenharBarras(Placar *vetor, int n, int indiceDestacado) {
    int larguraBarra = LARGURA_JANELA / n;

    for (int i = 0; i < n; i++) {
        int altura = vetor[i].pontuacao * 4;
        int x = i * larguraBarra;
        int y = ALTURA_JANELA - 60 - altura;

        Color cor = (i == indiceDestacado) ? LIME : SKYBLUE;
        DrawRectangle(x + 2, y, larguraBarra - 4, altura, cor);
        DrawText(TextFormat("%d", vetor[i].pontuacao), x + 4, y - 18, 12, DARKGRAY);
    }
}

int main(void) {
    srand((unsigned int)time(NULL));

    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "Atividade 7 - Complexidade: Busca e Ordenacao");
    SetTargetFPS(60);

    Placar *placares = criarPlacares(TOTAL_PLACARES);
    bool ordenado = false;

    long comparacoes = 0, trocas = 0;
    double tempoMs = 0.0;
    int alvo = placares[GetRandomValue(0, TOTAL_PLACARES - 1)].pontuacao;
    int indiceEncontrado = -1;
    char resultado[128] = "Pressione O/I para ordenar, Q/W para buscar ou R para resetar";

    while (!WindowShouldClose()) {

        // Ordena com Bubble Sort (Exercício 2: medição com GetTime)
        if (IsKeyPressed(KEY_O)) {
            double inicio = GetTime();
            ordenarBubbleSort(placares, TOTAL_PLACARES, &comparacoes, &trocas);
            double fim = GetTime();
            tempoMs = (fim - inicio) * 1000.0;

            ordenado = true;
            indiceEncontrado = -1;
            TextCopy(resultado, TextFormat("Bubble Sort: %ld comp, %ld trocas | Tempo: %.4f ms",
                                           comparacoes, trocas, tempoMs));
        }

        // Exercício 1: Ordena com Insertion Sort
        if (IsKeyPressed(KEY_I)) {
            double inicio = GetTime();
            ordenarInsertionSort(placares, TOTAL_PLACARES, &comparacoes, &trocas);
            double fim = GetTime();
            tempoMs = (fim - inicio) * 1000.0;

            ordenado = true;
            indiceEncontrado = -1;
            TextCopy(resultado, TextFormat("Insertion Sort: %ld comp, %ld desloc | Tempo: %.4f ms",
                                           comparacoes, trocas, tempoMs));
        }

        // Reseta o vetor com novos valores desordenados para poder retestar
        if (IsKeyPressed(KEY_R)) {
            free(placares);
            placares = criarPlacares(TOTAL_PLACARES);
            ordenado = false;
            indiceEncontrado = -1;
            alvo = placares[GetRandomValue(0, TOTAL_PLACARES - 1)].pontuacao;
            TextCopy(resultado, "Vetor reiniciado com novos valores aleatorios.");
        }

        // Sorteia novo alvo
        if (IsKeyPressed(KEY_N)) {
            alvo = placares[GetRandomValue(0, TOTAL_PLACARES - 1)].pontuacao;
            indiceEncontrado = -1;
            TextCopy(resultado, TextFormat("Novo alvo sorteado: %d", alvo));
        }

        // Busca sequencial: O(n)
        if (IsKeyPressed(KEY_Q)) {
            double inicio = GetTime();
            indiceEncontrado = buscaSequencial(placares, TOTAL_PLACARES, alvo, &comparacoes);
            double fim = GetTime();
            tempoMs = (fim - inicio) * 1000.0;

            TextCopy(resultado, TextFormat("Busca Sequencial: %ld comp | Tempo: %.4f ms",
                                           comparacoes, tempoMs));
        }

        // Busca binária: O(log n)
        if (IsKeyPressed(KEY_W)) {
            if (!ordenado) {
                TextCopy(resultado, "Ordene primeiro com O ou I -- busca binaria exige vetor ordenado!");
            } else {
                double inicio = GetTime();
                indiceEncontrado = buscaBinaria(placares, TOTAL_PLACARES, alvo, &comparacoes);
                double fim = GetTime();
                tempoMs = (fim - inicio) * 1000.0;

                TextCopy(resultado, TextFormat("Busca Binaria: %ld comp | Tempo: %.4f ms",
                                               comparacoes, tempoMs));
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            desenharBarras(placares, TOTAL_PLACARES, indiceEncontrado);

            DrawText(TextFormat("Alvo da busca: %d   Vetor ordenado: %s", alvo, ordenado ? "SIM" : "NAO"),
                     10, 10, 20, DARKGRAY);
            DrawText(resultado, 10, 36, 18, MAROON);
            DrawText("O Bubble | I Insertion | R Reset | N Novo alvo | Q Seq | W Bin | ESC Sai",
                     10, ALTURA_JANELA - 25, 15, GRAY);

        EndDrawing();
    }

    free(placares);
    CloseWindow();
    return 0;
}