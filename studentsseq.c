#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NOTAS_POSSIVEIS 101
#define NOME_ARQ_ENTRADA "entrada.txt"
#define DEBUG
#define RESPONSE_TIME_TESTING


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


int obter_menor_nota(int *notas, int idx_inicio, int idx_fim)
{
    int menor_nota = NOTAS_POSSIVEIS;

    for (int i = idx_inicio; i <= idx_fim; i++)
    {
        if (notas[i] < menor_nota)
            menor_nota = notas[i];
    }

    return menor_nota;
}


int obter_maior_nota(int *notas, int idx_inicio, int idx_fim)
{
    int maior_nota = -1;

    for (int i = idx_inicio; i <= idx_fim; i++)
    {
        if (notas[i] > maior_nota)
            maior_nota = notas[i];
    }

    return maior_nota;
}


int calcular_soma_para_media(int *notas, int idx_inicio, int idx_fim)
{
    int soma = 0;

    for (int i = idx_inicio; i <= idx_fim; i++)
        soma += notas[i];

    return soma;
}


double calcular_soma_para_dp(int *notas, double media, int idx_inicio, int idx_fim)
{
    double soma = 0;

    for (int i = idx_inicio; i <= idx_fim; i++)
        soma += pow(media - notas[i], 2);

    return soma;
}


int *construir_contagem(int *notas, int idx_inicio, int idx_fim)
{
    int *contagem = calloc(NOTAS_POSSIVEIS, sizeof(int));

    for (int i = idx_inicio; i <= idx_fim; i++)
        contagem[notas[i]]++;
    
    return contagem;
}


double calcular_mediana(int *contagem, int total)
{
    int  qtd_contados = 0, metade = total / 2;

    for (int i = 0; i < NOTAS_POSSIVEIS; i++)
        if (contagem[i]) printf("%d; ", i);
    printf("\n");

    if (total % 2 != 0) {
        for (int i = 0; i < NOTAS_POSSIVEIS; i++)
        {
            for (int j = contagem[i]; j > 0; j--)
            {
                if (qtd_contados == metade)
                    return i;
                qtd_contados++;
            }
        }
    } else {
        int meio1, meio2;
        for (int i = 0; i < NOTAS_POSSIVEIS; i++)
        {
            for (int j = contagem[i]; j > 0; j--)
            {
                qtd_contados++;
                if (qtd_contados == metade)
                {
                    meio1 = i;
                    if (j > 1)
                        return meio1;
                    i++;
                    while (!contagem[i]) i++;
                    meio2 = i;
                    printf("%d e %d\n", meio1, meio2);
                    return (double) (meio1 + meio2) / 2;
                }
            }
        }
    }

    return 0;
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
    
    #ifdef RESPONSE_TIME_TESTING //medicao do tempo de resposta
    double start, end;
    start = omp_get_wtime();
    #endif
    
    // Cálculos sequenciais:
    for (int regiao = 0; regiao < total_regioes; regiao++)
    {
        for (int cidade = 0; cidade < total_cidades; cidade++)
        {
            int *contagem = construir_contagem(
                notas,
                regiao * total_cidades * total_alunos + cidade * total_alunos,
                regiao * total_cidades * total_alunos + cidade * total_alunos + total_alunos - 1
            );
            #ifndef RESPONSE_TIME_TESTING
            printf("Reg %d - Cid %d: mediana: %.2lf\n", regiao, cidade, calcular_mediana(contagem, total_alunos));
            #endif
        }
    }
    #ifndef RESPONSE_TIME_TESTING
    end = omp_get_wtime();
    printf("%lf\n", end-start);
    #endif

    free(notas);

    return 0;
}
