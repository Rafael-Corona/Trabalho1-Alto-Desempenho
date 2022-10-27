#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "funcoes-par.h"
#include <string.h>

double keep_valueD[2]; // buffers para garantir que certas variaveis sejam calculadas
unsigned keep_valueU[3]; //já que no modo de afericao de tempo, elas não seriam utilizadas
                        // ou seja, para enganar o compilador


int main(void)
{
    // Configuração do número padrão de threads:
    omp_set_num_threads(TOTAL_THREADS);

    // Habilitação de paralelismo aninhado:
    omp_set_nested(1);

    // Leitura dos parâmetros de entrada de arquivo de texto:
    unsigned total_regioes, cidades_por_regiao, alunos_por_cidade, semente;
    atribuir_parametros_entrada(&total_regioes, &cidades_por_regiao, &alunos_por_cidade, &semente);

    // Cálculo do total final de notas:
    unsigned total_final_notas = total_regioes * cidades_por_regiao * alunos_por_cidade;
    unsigned alunos_por_regiao = cidades_por_regiao * alunos_por_cidade;

    // Geração das notas aleatórias de acordo com a semente lida:
    srand(semente);
    unsigned *notas = gerar_notas(total_final_notas);

    // Impressão das matrizes de notas:
    #ifdef DEBUG
    printf("\n");
    for (unsigned regiao = 0; regiao < total_regioes; regiao++)
        imprimir_matriz(notas, regiao, cidades_por_regiao, alunos_por_cidade);
    #endif

    // Medição do tempo de resposta:
    #ifdef RESPONSE_TIME_TESTING
    char filename[255] = "par-";
    char  r[15], c[15], a[15], t[10];
    sprintf(t, "%d", TOTAL_THREADS);
    sprintf(r, "%d", total_regioes);
    sprintf(c, "%d", cidades_por_regiao);
    sprintf(a, "%d", alunos_por_cidade);
    strcat(filename, t);
    strcat(filename, "thread-RCA");
    strcat(filename, r);
    strcat(filename, "-");
    strcat(filename, c);
    strcat(filename, "-");
    strcat(filename, a);    
    FILE *output_fp = fopen(filename, "a");

    double start, end;
    start = omp_get_wtime();
    #endif

    // Cálculos paralelos por cidade:

    unsigned *maior_nota_de_cada_cidade = (unsigned*) malloc(sizeof(unsigned) * cidades_por_regiao * total_regioes);
    unsigned *menor_nota_de_cada_cidade = (unsigned*) malloc(sizeof(unsigned) * cidades_por_regiao * total_regioes);
    double *mediana_de_cada_cidade = (double*) malloc(sizeof(double) * cidades_por_regiao * total_regioes);
    double *media_de_cada_cidade = (double*) malloc(sizeof(double) * cidades_por_regiao * total_regioes);
    double *dp_de_cada_cidade = (double*) malloc(sizeof(double) * cidades_por_regiao * total_regioes);
    unsigned long *soma_de_cada_cidade = (unsigned long*) malloc(sizeof(unsigned long) * cidades_por_regiao * total_regioes);
    unsigned **contagem_de_cada_cidade = (unsigned**) malloc(sizeof(unsigned*) * cidades_por_regiao * total_regioes);

    #pragma omp parallel for collapse(2)
    for (unsigned regiao = 0; regiao < total_regioes; regiao++)
    {
        for (unsigned cidade = 0; cidade < cidades_por_regiao; cidade++)
        {
            unsigned idx_inicio_cid = regiao * cidades_por_regiao * alunos_por_cidade + cidade * alunos_por_cidade;
            unsigned idx_fim_cid = idx_inicio_cid + alunos_por_cidade - 1;
            unsigned idx_cidade_dentro_brasil = regiao * cidades_por_regiao + cidade;
            unsigned menor_nota_cidade, maior_nota_cidade;
            unsigned long soma_cidade;
            double media_cidade, mediana_cidade, dp_cidade;
            unsigned *contagem_cidade = obter_contagem(notas, idx_inicio_cid, idx_fim_cid);
            mediana_cidade = calcular_mediana(contagem_cidade, alunos_por_cidade);
            mediana_de_cada_cidade[idx_cidade_dentro_brasil] = mediana_cidade;
            contagem_de_cada_cidade[idx_cidade_dentro_brasil] = contagem_cidade;
            obter_menor_maior_soma(notas, &menor_nota_cidade, &maior_nota_cidade, &soma_cidade, idx_inicio_cid, idx_fim_cid);
            maior_nota_de_cada_cidade[idx_cidade_dentro_brasil] = maior_nota_cidade;
            menor_nota_de_cada_cidade[idx_cidade_dentro_brasil] = menor_nota_cidade;
            soma_de_cada_cidade[idx_cidade_dentro_brasil] = soma_cidade;
            media_cidade = (double) soma_cidade / (double) alunos_por_cidade;
            media_de_cada_cidade[idx_cidade_dentro_brasil] = media_cidade;
            dp_cidade = sqrt(calcular_soma_para_dp(notas, media_cidade, idx_inicio_cid, idx_fim_cid) / (alunos_por_cidade - 1));
            dp_de_cada_cidade[idx_cidade_dentro_brasil] = dp_cidade;
            #ifndef RESPONSE_TIME_TESTING
            #endif
        }
    }

    // Saída com as respostas por cidade:
    #ifndef RESPONSE_TIME_TESTING
    for (unsigned i = 0, cidade = 0, regiao = 0; i < cidades_por_regiao * total_regioes; i++)
    {
        printf("Reg %d - Cid %d: menor: %d, maior %d, mediana: %.2lf, média: %.2lf e DP: %.2lf\n",
                regiao, cidade, menor_nota_de_cada_cidade[i], maior_nota_de_cada_cidade[i],
                mediana_de_cada_cidade[i], media_de_cada_cidade[i], dp_de_cada_cidade[i]);
        cidade = (cidade + 1) % cidades_por_regiao;
        if (cidade == 0) regiao++;
    }
    #endif

    // Cálculos paralelos por região:

    unsigned *maior_nota_de_cada_regiao = (unsigned*) malloc(sizeof(unsigned) * total_regioes);
    unsigned *menor_nota_de_cada_regiao = (unsigned*) malloc(sizeof(unsigned) * total_regioes);
    double *media_de_cada_regiao = (double *) malloc(sizeof(double) * total_regioes);
    double *mediana_de_cada_regiao = (double *) malloc(sizeof(double) * total_regioes);
    double *dp_de_cada_regiao = (double *) malloc(sizeof(double) * total_regioes);
    unsigned long *soma_de_cada_regiao = (unsigned long*) malloc(sizeof(unsigned long) * total_regioes);
    unsigned **contagem_de_cada_regiao = (unsigned**) malloc(sizeof(unsigned*) * total_regioes);

    #ifndef RESPONSE_TIME_TESTING
    printf("\n");
    #endif

    #pragma omp parallel for
    for (unsigned regiao = 0; regiao < total_regioes; regiao++)
    {
        unsigned idx_inicio_reg = regiao * alunos_por_regiao;
        unsigned idx_fim_reg = idx_inicio_reg + alunos_por_regiao - 1;
        unsigned idx_inicio_vetor_cidade = regiao * cidades_por_regiao;
        unsigned idx_fim_vetor_cidade = idx_inicio_vetor_cidade + cidades_por_regiao-1;
        unsigned menor_nota_regiao = obter_menor_valor_int(menor_nota_de_cada_cidade, idx_inicio_vetor_cidade, idx_fim_vetor_cidade);
        unsigned maior_nota_regiao = obter_maior_valor_int(maior_nota_de_cada_cidade, idx_inicio_vetor_cidade, idx_fim_vetor_cidade);
        unsigned long soma_regiao = soma_vetor_long(soma_de_cada_cidade, idx_inicio_vetor_cidade, idx_fim_vetor_cidade);
        double media_regiao =  soma_regiao / (double) (alunos_por_regiao);

        double dp_regiao = sqrt(calcular_soma_para_dp(notas, media_regiao, idx_inicio_reg, idx_fim_reg) / (alunos_por_cidade * cidades_por_regiao - 1));

        unsigned *contagem_regiao = somar_contagens(contagem_de_cada_cidade, idx_inicio_vetor_cidade, idx_fim_vetor_cidade);
        contagem_de_cada_regiao[regiao] = contagem_regiao;
        double mediana_regiao = calcular_mediana(contagem_regiao, alunos_por_cidade * cidades_por_regiao);
        maior_nota_de_cada_regiao[regiao] = maior_nota_regiao;
        menor_nota_de_cada_regiao[regiao] = menor_nota_regiao;
        soma_de_cada_regiao[regiao] = soma_regiao;
        media_de_cada_regiao[regiao] = media_regiao;
        mediana_de_cada_regiao[regiao] = mediana_regiao;
        dp_de_cada_regiao[regiao] = dp_regiao;
    }

    // Saída com as respostas por região:
    #ifndef RESPONSE_TIME_TESTING
    for (unsigned regiao = 0; regiao < total_regioes; regiao++)
    {
        printf("Reg %d: menor: %d, maior %d, mediana: %.2lf, média: %.2lf e DP: %.2lf\n",
                regiao, menor_nota_de_cada_regiao[regiao], maior_nota_de_cada_regiao[regiao], mediana_de_cada_regiao[regiao],
                media_de_cada_regiao[regiao], dp_de_cada_regiao[regiao]);
    }
    #endif

    free(maior_nota_de_cada_cidade);
    free(menor_nota_de_cada_cidade);
    free_lista_contagem(contagem_de_cada_cidade, cidades_por_regiao * total_regioes);

   

    // Cálculo paralelo para o Brasil:
    unsigned menor_nota_brasil, maior_nota_brasil;
    unsigned long soma_brasil;
    double mediana_brasil, media_brasil, dp_brasil;
    unsigned *contagem_brasil;
    unsigned melhor_regiao, regiao_melhor_cidade, melhor_cidade;
    
    #pragma omp sections
    {
        #pragma omp section
        {
            maior_nota_brasil = obter_maior_valor_int(maior_nota_de_cada_regiao, 0, total_regioes-1);
            free(maior_nota_de_cada_regiao);
        }
        #pragma omp section
        {
            menor_nota_brasil = obter_menor_valor_int(menor_nota_de_cada_regiao, 0, total_regioes-1);
            free(menor_nota_de_cada_regiao);
        }
        #pragma omp section
        {
            soma_brasil = soma_vetor_long(soma_de_cada_regiao, 0,total_regioes-1);
            media_brasil =  (double) soma_brasil / total_final_notas;
            dp_brasil = sqrt(calcular_soma_para_dp(notas, media_brasil, 0, total_final_notas - 1) / (total_final_notas - 1));
        }
        #pragma omp section
        {
            contagem_brasil = somar_contagens(contagem_de_cada_regiao, 0, total_regioes-1);
            free_lista_contagem(contagem_de_cada_regiao, total_regioes);
            mediana_brasil = calcular_mediana(contagem_brasil, total_final_notas);
            free(contagem_brasil);
        }
        #pragma omp section
        {
            melhor_regiao = obter_melhor_regiao(soma_de_cada_regiao, 0, total_regioes-1);
            free(soma_de_cada_regiao);
        }
        #pragma omp section
        {
            melhor_cidade_e_sua_regiao(soma_de_cada_cidade, total_regioes, cidades_por_regiao, &melhor_cidade, &regiao_melhor_cidade);
            free(soma_de_cada_cidade);
        }
    }


    #ifndef RESPONSE_TIME_TESTING
    printf("\nBrasil: menor: %d, maior %d, mediana: %.2lf, média: %.2lf e DP: %.2lf\n",
            menor_nota_brasil, maior_nota_brasil, mediana_brasil, media_brasil, dp_brasil);
    #endif
    keep_valueU[0] = melhor_regiao;
    keep_valueU[1] = menor_nota_brasil;
    keep_valueU[2] = maior_nota_brasil;
    keep_valueD[0] = mediana_brasil;
    keep_valueD[1] = dp_brasil;
    


    #ifdef RESPONSE_TIME_TESTING
    end = omp_get_wtime();
    fprintf(output_fp,"%lf\n", end-start);
    fclose(output_fp);
    #else
    printf("Melhor região: Região %d\n", melhor_regiao);
    printf("Melhor cidade: Região %d, Cidade %d\n", regiao_melhor_cidade, melhor_cidade);
    #endif

    free(notas);

    return 0;
}
