#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "matriz.h"
#include "pilha.h"
#include "colors.h"

struct matriz
{
    int linhas;
    int colunas;
    Pilha ***dados;
};

Tabuleiro *cria(int tamanho)
{
    Tabuleiro *matriz;
    matriz = (Tabuleiro *)malloc(sizeof(Tabuleiro));
    if (matriz == NULL)
    {
        printf("Nao foi possível criar a matriz. Memoria insuficiente.\n");
        exit(1);
    }
    matriz->linhas = tamanho;
    matriz->colunas = tamanho;
    matriz->dados = (Pilha ***)malloc(tamanho * sizeof(Pilha **));
    if (matriz->dados == NULL)
    {
        free(matriz);
        printf("Nao foi possível criar espaços para matriz. Memoria insuficiente.\n");
        exit(1);
    }
    for (int i = 0; i < tamanho; i++)
    {
        matriz->dados[i] = (Pilha **)malloc(tamanho * sizeof(Pilha *));
        if (matriz->dados[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(matriz->dados[j]);
            }
            free(matriz->dados);
            free(matriz);
            printf("Nao foi possível criar linha %d da matriz. Memoria insuficiente.\n", i + 1);
            exit(1);
        }
        for (int j = 0; j < tamanho; j++)
        {
            matriz->dados[i][j] = criar();
            if (matriz->dados[i][j] == NULL)
            {
                for (int k = 0; k < j; k++)
                {
                    destruir(matriz->dados[i][k]);
                }
                for (int k = 0; k < i; k++)
                {
                    for (int l = 0; l < tamanho; l++)
                    {
                        destruir(matriz->dados[k][l]);
                    }
                    free(matriz->dados[k]);
                }
                free(matriz->dados[i]);
                free(matriz->dados);
                free(matriz);
                printf("Nao foi possível criar pilha na posicao [%d][%d]. Memoria insuficiente.\n", i, j);
                exit(1);
            }
            inserir(matriz->dados[i][j]);
        }
    }
    return matriz;
}

void libera(Tabuleiro *matriz)
{
    for (int i = 0; i < matriz->linhas; i++)
    {
        for (int j = 0; j < matriz->colunas; j++)
        {
            destruir(matriz->dados[i][j]);
        }
        free(matriz->dados[i]);
    }
    free(matriz->dados);
    free(matriz);
}

int acessar(Tabuleiro *matriz, int linha, int coluna, Pilha **pd)
{
    if (matriz == NULL)
    {
        return 0;
    }
    if (linha > (matriz->linhas - 1))
    {
        return 0;
    }
    if (coluna > (matriz->colunas - 1))
    {
        return 0;
    }
    *pd = matriz->dados[linha][coluna];
    return 1;
}

int linhas(Tabuleiro *matriz)
{
    return matriz->linhas;
}

int colunas(Tabuleiro *matriz)
{
    return matriz->colunas;
}