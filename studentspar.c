#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define NOTAS_POSSIVEIS 101
#define NOME_ARQ_ENTRADA "entrada.txt"
#define DEBUG


void atribuir_parametros_entrada(int *total_regioes, int *total_cidades, int *total_alunos, int *semente)
{
    FILE *fp = fopen(NOME_ARQ_ENTRADA, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Erro ao abrir arquivo com as informações de entrada!\n");
        fprintf(stderr, "Verifique o arquivo de nome \"entrada.txt\"\n");
        exit(-1);
    }

    fscanf(fp, "%d", total_regioes);
    fscanf(fp, "%d", total_cidades);
    fscanf(fp, "%d", total_alunos);
    fscanf(fp, "%d", semente);

    fclose(fp);
} 


int *gerar_notas(int total_notas)
{
    int *notas = malloc(total_notas * sizeof(int *));

    for (int i = 0; i < total_notas; i++)
    {
        notas[i] = rand() % NOTAS_POSSIVEIS;
    }

    return notas;
}
 

#ifdef DEBUG
void imprimir_matriz(int *notas, int regiao, int total_cidades, int total_alunos)
{
    int idx_inicio = regiao * total_cidades * total_alunos;

    printf("-> Região %d:\n", regiao);
    for (int i = 0; i < total_cidades; i++)
    {
        for (int j = 0; j < total_alunos; j++)
            printf("%3d  ", notas[idx_inicio + i * total_alunos + j]);
        printf("\n");
    }
    printf("\n");
}
#endif


int main(void)
{
    // Leitura dos parâmetros de entrada de arquivo de texto:
    int total_regioes, total_cidades, total_alunos, semente;
    atribuir_parametros_entrada(&total_regioes, &total_cidades, &total_alunos, &semente);

    // Geração das notas aleatórias de acordo com a semente lida:
    srand(semente);
    int *notas = gerar_notas(total_regioes * total_cidades * total_alunos);

    // Impressão das matrizes de notas:
    #ifdef DEBUG
    printf("\n");
    for (int regiao = 0; regiao < total_regioes; regiao++)
        imprimir_matriz(notas, regiao, total_cidades, total_alunos);
    #endif

    free(notas);

    return 0;
}
