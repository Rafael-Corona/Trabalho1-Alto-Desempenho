#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NOTAS_POSSIVEIS 101
#define NOME_ARQ_ENTRADA "entrada.txt"

#ifdef DEBUG
int teste[72] = {
    30, 40, 20, 80, 85, 10,
    10, 20, 30, 40, 50, 60,
    60, 50, 40, 30, 20, 10,
    70, 55, 35, 80, 95, 27,
    35, 45, 25, 85, 90, 15,
    15, 25, 35, 45, 55, 65,
    65, 55, 45, 35, 25, 15,
    75, 60, 40, 85, 100, 32,
    20, 30, 10, 70, 75, 00,
    00, 10, 20, 30, 40, 50,
    50, 40, 30, 20, 10, 00,
    60, 45, 25, 70, 85, 17
};
#endif

void atribuir_parametros_entrada(int *total_regioes, int *cidades_por_regiao, int *alunos_por_cidade, int *semente)
{
    FILE *fp = fopen(NOME_ARQ_ENTRADA, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Erro ao abrir arquivo com as informações de entrada!\n");
        fprintf(stderr, "Verifique o arquivo de nome \"entrada.txt\"\n");
        exit(-1);
    }

    fscanf(fp, "%d", total_regioes);
    fscanf(fp, "%d", cidades_por_regiao);
    fscanf(fp, "%d", alunos_por_cidade);
    fscanf(fp, "%d", semente);

    fclose(fp);
}


int *gerar_notas(int total_notas)
{
    int *notas = malloc(total_notas * sizeof(int *));

    for (int i = 0; i < total_notas; i++)
    {
        #ifdef DEBUG
        notas[i] = teste[i];
        #else
        notas[i] = rand() % NOTAS_POSSIVEIS;
        #endif
    }

    return notas;
}


double obter_menor_valor_double(double *notas, int idx_inicio, int idx_fim)
{
    double menor_media = NOTAS_POSSIVEIS;

    for (int i = idx_inicio; i <= idx_fim; i++)
    {
        if (notas[i] < menor_media)
            menor_media = notas[i];
    }

    return menor_media;
}

double obter_maior_valor_double(double *notas, int idx_inicio, int idx_fim)
{
    double maior_media = NOTAS_POSSIVEIS;

    for (int i = idx_inicio; i <= idx_fim; i++)
    {
        if (notas[i] < maior_media)
            maior_media = notas[i];
    }

    return maior_media;
}

int obter_menor_valor_int(int *notas, int idx_inicio, int idx_fim)
{
    int menor_nota = NOTAS_POSSIVEIS;

    for (int i = idx_inicio; i <= idx_fim; i++)
    {
        if (notas[i] < menor_nota)
            menor_nota = notas[i];
    }

    return menor_nota;
}

int obter_maior_valor_int(int *notas, int idx_inicio, int idx_fim)
{
    int maior_nota = -1;

    for (int i = idx_inicio; i <= idx_fim; i++)
    {
        if (notas[i] > maior_nota)
            maior_nota = notas[i];
    }

    return maior_nota;
}

void obter_menor_e_maior_nota(int *notas, int *menor_nota, int *maior_nota, int idx_inicio, int idx_fim)
{
    int maior_nota_tmp = -1;
    int menor_nota_tmp = NOTAS_POSSIVEIS;

    for (int i = idx_inicio; i <= idx_fim; i++)
    {
        if (notas[i] > maior_nota_tmp){
            maior_nota_tmp = notas[i];
        }
        if (notas[i] < menor_nota_tmp){
            menor_nota_tmp = notas[i];
        }
    }  
    *maior_nota = maior_nota_tmp;
    *menor_nota = menor_nota_tmp;
}

int obter_melhor_regiao(int *soma_de_cada_regiao, int idx_inicio, int idx_fim)
{
    int regiao = 0;
    int maior_soma = -1;

    for (int i = idx_inicio; i <= idx_fim; i++)
    {
        if (soma_de_cada_regiao[i] > maior_soma){
            regiao = i;
            maior_soma = soma_de_cada_regiao[i];
        }
    }
    return regiao;
}
void melhor_cidade_e_sua_regiao(int *soma_de_cada_cidade, int total_regioes, int cidades_por_regiao ,int *melhor_cidade, int*regiao)
{
    *melhor_cidade = 0;
    *regiao = 0;

    int maior_soma = -1;
    int last = total_regioes * cidades_por_regiao-1;

    for (int i = 0; i <= last; i++)
    {
        if (soma_de_cada_cidade[i] > maior_soma){
            *melhor_cidade = i % cidades_por_regiao;
            maior_soma = soma_de_cada_cidade[i];
            *regiao = i / cidades_por_regiao;
        }
    }
}

int soma_vetor_int(int *notas, int idx_inicio, int idx_fim)
{
    int soma = 0;

    for (int i = idx_inicio; i <= idx_fim; i++)
        soma += notas[i];

    return soma;
}

double soma_vetor_double(double *medias, int idx_inicio, int idx_fim)
{
    double soma = 0;

    for (int i = idx_inicio; i <= idx_fim; i++)
        soma += medias[i];

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
void imprimir_matriz(int *notas, int regiao, int cidades_por_regiao, int alunos_por_cidade)
{
    int idx_inicio = regiao * cidades_por_regiao * alunos_por_cidade;

    printf("-> Região %d:\n", regiao);
    for (int i = 0; i < cidades_por_regiao; i++)
    {
        for (int j = 0; j < alunos_por_cidade; j++)
            printf("%3d  ", notas[idx_inicio + i * alunos_por_cidade + j]);
        printf("\n");
    }
    printf("\n");
}
#endif


int main(void)
{
    // Leitura dos parâmetros de entrada de arquivo de texto:
    int total_regioes, cidades_por_regiao, alunos_por_cidade, semente;
    atribuir_parametros_entrada(&total_regioes, &cidades_por_regiao, &alunos_por_cidade, &semente);

    // Cálculo do total final de notas:
    int total_final_notas = total_regioes * cidades_por_regiao * alunos_por_cidade;
    
    // Geração das notas aleatórias de acordo com a semente lida:
    srand(semente);
    int *notas = gerar_notas(total_final_notas);

    // Impressão das matrizes de notas:
    #ifdef DEBUG
    printf("\n");
    for (int regiao = 0; regiao < total_regioes; regiao++)
        imprimir_matriz(notas, regiao, cidades_por_regiao, alunos_por_cidade);
    #endif

    // Medição do tempo de resposta:
    #ifdef RESPONSE_TIME_TESTING 
    double start, end;
    start = omp_get_wtime();
    #endif
    
    // Cálculos sequenciais por cidade:
    int *maior_nota_de_cada_cidade = malloc(sizeof(int) * cidades_por_regiao * total_regioes);
    int *menor_nota_de_cada_cidade = malloc(sizeof(int) * cidades_por_regiao * total_regioes);
    int *soma_de_cada_cidade = malloc(sizeof(int) * cidades_por_regiao * total_regioes);

    for (int regiao = 0; regiao < total_regioes; regiao++)
    {
        for (int cidade = 0; cidade < cidades_por_regiao; cidade++)
        {
            int idx_inicio_cid = regiao * cidades_por_regiao * alunos_por_cidade + cidade * alunos_por_cidade;
            int idx_fim_cid = regiao * cidades_por_regiao * alunos_por_cidade + cidade * alunos_por_cidade + alunos_por_cidade - 1;
            int menor_nota_cidade;
            int maior_nota_cidade;
            obter_menor_e_maior_nota(notas, &menor_nota_cidade, &maior_nota_cidade, idx_inicio_cid, idx_fim_cid);
            int soma_cidade = soma_vetor_int(notas, idx_inicio_cid, idx_fim_cid);
            double media_cidade = (double) soma_cidade / (double) alunos_por_cidade; 
            double dp_cidade = sqrt(calcular_soma_para_dp(notas, media_cidade, idx_inicio_cid, idx_fim_cid) / (alunos_por_cidade - 1)); 
            int *contagem_cidade = construir_contagem(notas, idx_inicio_cid, idx_fim_cid);
            double mediana_cidade = calcular_mediana(contagem_cidade, alunos_por_cidade);
            maior_nota_de_cada_cidade[regiao*cidades_por_regiao + cidade] = maior_nota_cidade;
            menor_nota_de_cada_cidade[regiao*cidades_por_regiao + cidade] = menor_nota_cidade;
            soma_de_cada_cidade[regiao*cidades_por_regiao + cidade] = soma_cidade;

            #ifndef RESPONSE_TIME_TESTING
            printf("Reg %d - Cid %d: menor: %d, maior %d, mediana: %.2lf, média: %.2lf e DP: %.2lf\n",
                    regiao, cidade, menor_nota_cidade, maior_nota_cidade, mediana_cidade, media_cidade, dp_cidade);
            #endif
            free(contagem_cidade);
        }
    }

    // Cálculos sequenciais por região:

    int *maior_nota_de_cada_regiao = malloc(sizeof(int) * total_regioes);
    int *menor_nota_de_cada_regiao= malloc(sizeof(int) * total_regioes);
    int *soma_de_cada_regiao = malloc(sizeof(int) * total_regioes);

    #ifndef RESPONSE_TIME_TESTING
    printf("\n");
    #endif
    for (int regiao = 0; regiao < total_regioes; regiao++)
    {
        int alunos_por_regiao = cidades_por_regiao * alunos_por_cidade;
        int idx_inicio_reg = regiao * cidades_por_regiao * alunos_por_cidade;
        int idx_fim_reg = regiao * cidades_por_regiao * alunos_por_cidade + cidades_por_regiao * alunos_por_cidade - 1;
        int idx_inicio_vetor_cidade = regiao * cidades_por_regiao;
        int idx_fim_vetor_cidade = idx_inicio_vetor_cidade + cidades_por_regiao-1; 
        int menor_nota_regiao = obter_menor_valor_int(menor_nota_de_cada_cidade, idx_inicio_vetor_cidade, idx_fim_vetor_cidade);
        int maior_nota_regiao = obter_maior_valor_int(maior_nota_de_cada_cidade, idx_inicio_vetor_cidade, idx_fim_vetor_cidade);
        int soma_regiao = soma_vetor_int(soma_de_cada_cidade, idx_inicio_vetor_cidade, idx_fim_vetor_cidade);
        double media_regiao = (double) soma_regiao / (double) (alunos_por_regiao);
        double dp_regiao = sqrt(calcular_soma_para_dp(notas, media_regiao, idx_inicio_reg, idx_fim_reg) / (alunos_por_cidade * cidades_por_regiao - 1)); 
        int *contagem_regiao = construir_contagem(notas, idx_inicio_reg, idx_fim_reg);
        double mediana_regiao = calcular_mediana(contagem_regiao, alunos_por_cidade * cidades_por_regiao);
        maior_nota_de_cada_regiao[regiao] = maior_nota_regiao;
        menor_nota_de_cada_regiao[regiao] = menor_nota_regiao;
        soma_de_cada_regiao[regiao] = soma_regiao;
        #ifndef RESPONSE_TIME_TESTING
        printf("Reg %d: menor: %d, maior %d, mediana: %.2lf, média: %.2lf e DP: %.2lf\n",
                regiao, menor_nota_regiao, maior_nota_regiao, mediana_regiao, media_regiao, dp_regiao);
        #endif
        free(contagem_regiao);
    }
    // Cálculo sequencial para o Brasil:
    int menor_nota_brasil = obter_menor_valor_int(menor_nota_de_cada_regiao, 0, total_regioes-1);
    int maior_nota_brasil = obter_maior_valor_int(maior_nota_de_cada_regiao, 0, total_regioes-1);
    int soma_brasil = soma_vetor_int(soma_de_cada_regiao, 0,total_regioes-1);
    double media_brasil =  (double) soma_brasil / total_final_notas;
    double dp_brasil = sqrt(calcular_soma_para_dp(notas, media_brasil, 0, total_final_notas - 1) / (total_final_notas - 1)); 
    int *contagem_brasil = construir_contagem(notas, 0, total_final_notas - 1);
    double mediana_brasil = calcular_mediana(contagem_brasil, total_final_notas);
    #ifndef RESPONSE_TIME_TESTING
    printf("\nBrasil: menor: %d, maior %d, mediana: %.2lf, média: %.2lf e DP: %.2lf\n",
            menor_nota_brasil, maior_nota_brasil, mediana_brasil, media_brasil, dp_brasil);

    #endif
    free(contagem_brasil);

    int melhor_regiao = obter_melhor_regiao(soma_de_cada_regiao, 0, total_regioes-1);
    int regiao_melhor_cidade;
    int melhor_cidade;
    melhor_cidade_e_sua_regiao(soma_de_cada_cidade, total_regioes, cidades_por_regiao, &melhor_cidade, &regiao_melhor_cidade);

    free(soma_de_cada_cidade);
    free(soma_de_cada_regiao);
    free(menor_nota_de_cada_cidade);
    free(menor_nota_de_cada_regiao);
    free(maior_nota_de_cada_cidade);
    free(maior_nota_de_cada_regiao);

    #ifdef RESPONSE_TIME_TESTING
    end = omp_get_wtime();
    printf("Tempo de resposta = %lf\n", end-start);
    #else
    printf("Melhor região: Região %d\n", melhor_regiao);
    printf("Melhor cidade: Região %d, Cidade %d\n", regiao_melhor_cidade, melhor_cidade);
    #endif

    free(notas);

    return 0;
}
