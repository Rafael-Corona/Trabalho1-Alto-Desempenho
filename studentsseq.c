#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NOTAS_POSSIVEIS 101
#define NOME_ARQ_ENTRADA "entrada.txt"


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
        soma += pow(notas[i] - media, 2);

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

    if (total % 2 != 0)
    {
        for (int i = 0; i < NOTAS_POSSIVEIS; i++)
        {
            for (int j = contagem[i]; j > 0; j--)
            {
                if (qtd_contados == metade)
                    return i;
                qtd_contados++;
            }
        }
    }
    else
    {
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

    // Cálculo do total final de notas:
    int total_final_notas = total_regioes * total_cidades * total_alunos;
    
    // Geração das notas aleatórias de acordo com a semente lida:
    srand(semente);
    int *notas = gerar_notas(total_final_notas);

    // Impressão das matrizes de notas:
    #ifdef DEBUG
    printf("\n");
    for (int regiao = 0; regiao < total_regioes; regiao++)
        imprimir_matriz(notas, regiao, total_cidades, total_alunos);
    #endif

    // Medição do tempo de resposta:
    #ifdef RESPONSE_TIME_TESTING 
    double start, end;
    start = omp_get_wtime();
    #endif
    
    // Cálculos sequenciais por cidade:
    int melhor_cidade = 0, regiao_melhor_cidade = 0;
    double media_melhor_cidade = -1;
    for (int regiao = 0; regiao < total_regioes; regiao++)
    {
        for (int cidade = 0; cidade < total_cidades; cidade++)
        {
            int idx_inicio_cid = regiao * total_cidades * total_alunos + cidade * total_alunos;
            int idx_fim_cid = regiao * total_cidades * total_alunos + cidade * total_alunos + total_alunos - 1;
            int menor_nota_cidade = obter_menor_nota(notas, idx_inicio_cid, idx_fim_cid);
            int maior_nota_cidade = obter_maior_nota(notas, idx_inicio_cid, idx_fim_cid);
            double media_cidade = calcular_soma_para_media(notas, idx_inicio_cid, idx_fim_cid) / (double) total_alunos;
            double dp_cidade = sqrt(calcular_soma_para_dp(notas, media_cidade, idx_inicio_cid, idx_fim_cid) / (total_alunos - 1)); 
            int *contagem_cidade = construir_contagem(notas, idx_inicio_cid, idx_fim_cid);
            double mediana_cidade = calcular_mediana(contagem_cidade, total_alunos);
            if (media_cidade > media_melhor_cidade)
            {
                melhor_cidade = cidade;
                regiao_melhor_cidade = regiao;
                media_melhor_cidade = media_cidade;
            }
            #ifndef RESPONSE_TIME_TESTING
            printf("Reg %d - Cid %d: menor: %d, maior %d, mediana: %.2lf, média: %.2lf e DP: %.2lf\n",
                    regiao, cidade, menor_nota_cidade, maior_nota_cidade, mediana_cidade, media_cidade, dp_cidade);
            #endif
            free(contagem_cidade);
        }
    }

    // Cálculos sequenciais por região:
    int melhor_regiao = 0;
    double media_melhor_regiao = -1;
    #ifndef RESPONSE_TIME_TESTING
    printf("\n");
    #endif
    for (int regiao = 0; regiao < total_regioes; regiao++)
    {
        int idx_inicio_reg = regiao * total_cidades * total_alunos;
        int idx_fim_reg = regiao * total_cidades * total_alunos + total_cidades * total_alunos - 1;
        int menor_nota_regiao = obter_menor_nota(notas, idx_inicio_reg, idx_fim_reg);
        int maior_nota_regiao = obter_maior_nota(notas, idx_inicio_reg, idx_fim_reg);
        double media_regiao = calcular_soma_para_media(notas, idx_inicio_reg, idx_fim_reg) / (double) (total_alunos * total_cidades);
        double dp_regiao = sqrt(calcular_soma_para_dp(notas, media_regiao, idx_inicio_reg, idx_fim_reg) / (total_alunos * total_cidades - 1)); 
        int *contagem_regiao = construir_contagem(notas, idx_inicio_reg, idx_fim_reg);
        double mediana_regiao = calcular_mediana(contagem_regiao, total_alunos * total_cidades);
        if (media_regiao > media_melhor_regiao)
        {
            melhor_regiao = regiao;
            media_melhor_regiao = media_regiao;
        }
        #ifndef RESPONSE_TIME_TESTING
        printf("Reg %d: menor: %d, maior %d, mediana: %.2lf, média: %.2lf e DP: %.2lf\n",
                regiao, menor_nota_regiao, maior_nota_regiao, mediana_regiao, media_regiao, dp_regiao);
        #endif
        free(contagem_regiao);
    }

    // Cálculo sequencial para o Brasil:
    int menor_nota_brasil = obter_menor_nota(notas, 0, total_final_notas - 1);
    int maior_nota_brasil = obter_maior_nota(notas, 0, total_final_notas - 1);
    double media_brasil = calcular_soma_para_media(notas, 0, total_final_notas - 1) / (double) total_final_notas;
    double dp_brasil = sqrt(calcular_soma_para_dp(notas, media_brasil, 0, total_final_notas - 1) / (total_final_notas - 1)); 
    int *contagem_brasil = construir_contagem(notas, 0, total_final_notas - 1);
    double mediana_brasil = calcular_mediana(contagem_brasil, total_final_notas);
    #ifndef RESPONSE_TIME_TESTING
    printf("\nBrasil: menor: %d, maior %d, mediana: %.2lf, média: %.2lf e DP: %.2lf\n",
            menor_nota_brasil, maior_nota_brasil, mediana_brasil, media_brasil, dp_brasil);
    #endif
    free(contagem_brasil);

    #ifndef RESPONSE_TIME_TESTING
    printf("Melhor região: Região %d\n", melhor_regiao);
    printf("Melhor cidade: Região %d, Cidade %d\n", regiao_melhor_cidade, melhor_cidade);
    #endif

    #ifdef RESPONSE_TIME_TESTING
    end = omp_get_wtime();
    printf("%lf\n", end-start);
    #endif

    free(notas);

    return 0;
}
