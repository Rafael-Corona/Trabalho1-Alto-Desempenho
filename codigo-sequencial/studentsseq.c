#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "funcoes-seq.h"

int main(void)
{
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
    unsigned NRUNS = 10;
    double sum = 0;
    double *iterations = malloc(sizeof(double) * NRUNS);

    for (unsigned i = 0; i < NRUNS; i++)
    {
        double start, end;
        start = omp_get_wtime();
    #endif
    
    // Cálculos sequenciais por cidade:
    unsigned *maior_nota_de_cada_cidade = (unsigned*) malloc(sizeof(unsigned) * cidades_por_regiao * total_regioes);
    unsigned *menor_nota_de_cada_cidade = (unsigned*) malloc(sizeof(unsigned) * cidades_por_regiao * total_regioes);
    unsigned long *soma_de_cada_cidade = (unsigned long*) malloc(sizeof(unsigned long) * cidades_por_regiao * total_regioes);
    unsigned **contagem_de_cada_cidade = (unsigned**) malloc(sizeof(unsigned*) * cidades_por_regiao * total_regioes);

    for (unsigned regiao = 0; regiao < total_regioes; regiao++)
    {
        for (unsigned cidade = 0; cidade < cidades_por_regiao; cidade++)
        {
            unsigned idx_inicio_cid = regiao * cidades_por_regiao * alunos_por_cidade + cidade * alunos_por_cidade;
            unsigned idx_fim_cid = idx_inicio_cid + alunos_por_cidade - 1;
            unsigned idx_cidade_dentro_brasil = regiao*cidades_por_regiao + cidade;
            unsigned menor_nota_cidade, maior_nota_cidade;
            unsigned long soma_cidade;
            obter_menor_maior_soma(notas, &menor_nota_cidade, &maior_nota_cidade, &soma_cidade, idx_inicio_cid, idx_fim_cid);
            unsigned *contagem_cidade = obter_contagem(notas, idx_inicio_cid, idx_fim_cid);
            contagem_de_cada_cidade[idx_cidade_dentro_brasil] = contagem_cidade;

            double media_cidade = (double) soma_cidade / (double) alunos_por_cidade; 

            double dp_cidade = sqrt(calcular_soma_para_dp(notas, media_cidade, idx_inicio_cid, idx_fim_cid) / (alunos_por_cidade - 1)); 
            double mediana_cidade = calcular_mediana(contagem_cidade, alunos_por_cidade);
            maior_nota_de_cada_cidade[idx_cidade_dentro_brasil] = maior_nota_cidade;
            menor_nota_de_cada_cidade[idx_cidade_dentro_brasil] = menor_nota_cidade;
            soma_de_cada_cidade[idx_cidade_dentro_brasil] = soma_cidade;

            #ifndef RESPONSE_TIME_TESTING
            printf("Reg %d - Cid %d: menor: %d, maior %d, mediana: %.2lf, média: %.2lf e DP: %.2lf\n",
                    regiao, cidade, menor_nota_cidade, maior_nota_cidade, mediana_cidade, media_cidade, dp_cidade);
            #endif
        }
    }

    // Cálculos sequenciais por região:

    unsigned *maior_nota_de_cada_regiao = (unsigned*) malloc(sizeof(unsigned) * total_regioes);
    unsigned *menor_nota_de_cada_regiao = (unsigned*) malloc(sizeof(unsigned) * total_regioes);
    unsigned long *soma_de_cada_regiao = (unsigned long*) malloc(sizeof(unsigned long) * total_regioes);
    unsigned **contagem_de_cada_regiao = (unsigned**) malloc(sizeof(unsigned*) * total_regioes);


    #ifndef RESPONSE_TIME_TESTING
    printf("\n");
    #endif
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
        #ifndef RESPONSE_TIME_TESTING
        printf("Reg %d: menor: %d, maior %d, mediana: %.2lf, média: %.2lf e DP: %.2lf\n",
                regiao, menor_nota_regiao, maior_nota_regiao, mediana_regiao, media_regiao, dp_regiao);
        #endif
    }

    free(maior_nota_de_cada_cidade);
    free(menor_nota_de_cada_cidade);
    free_lista_contagem(contagem_de_cada_cidade, cidades_por_regiao * total_regioes);

    // Cálculo sequencial para o Brasil:
    unsigned menor_nota_brasil = obter_menor_valor_int(menor_nota_de_cada_regiao, 0, total_regioes-1);
    unsigned maior_nota_brasil = obter_maior_valor_int(maior_nota_de_cada_regiao, 0, total_regioes-1);
    free(maior_nota_de_cada_regiao);
    free(menor_nota_de_cada_regiao);
    
    unsigned long soma_brasil = soma_vetor_long(soma_de_cada_regiao, 0,total_regioes-1);
    double media_brasil =  (double) soma_brasil / total_final_notas;
    double dp_brasil = sqrt(calcular_soma_para_dp(notas, media_brasil, 0, total_final_notas - 1) / (total_final_notas - 1)); 
    unsigned *contagem_brasil = somar_contagens(contagem_de_cada_regiao, 0, total_regioes-1);

    free_lista_contagem(contagem_de_cada_regiao, total_regioes);

    double mediana_brasil = calcular_mediana(contagem_brasil, total_final_notas);
    #ifndef RESPONSE_TIME_TESTING
    printf("\nBrasil: menor: %d, maior %d, mediana: %.2lf, média: %.2lf e DP: %.2lf\n",
            menor_nota_brasil, maior_nota_brasil, mediana_brasil, media_brasil, dp_brasil);

    #endif
    free(contagem_brasil);

    unsigned melhor_regiao = obter_melhor_regiao(soma_de_cada_regiao, 0, total_regioes-1);
    unsigned regiao_melhor_cidade;
    unsigned melhor_cidade;
    melhor_cidade_e_sua_regiao(soma_de_cada_cidade, total_regioes, cidades_por_regiao, &melhor_cidade, &regiao_melhor_cidade);

    free(soma_de_cada_cidade);
    free(soma_de_cada_regiao);
    

    #ifdef RESPONSE_TIME_TESTING
        end = omp_get_wtime();
        printf("Iteracao %u. Tempo de resposta = %lf\n",i , end-start);
        sum += end-start;
        iterations[i] = end-start;
        clean_cache();
    }
    double average = sum / NRUNS;
    double parcial = 0;
    for (unsigned j = 0; j < NRUNS; j++)
        parcial += pow(iterations[j] - average, 2);

    printf("Tempo médio = %lf\n", average);
    printf("Desvio padrão = %lf\n", parcial/NRUNS-1);

    
    #else
    printf("Melhor região: Região %d\n", melhor_regiao);
    printf("Melhor cidade: Região %d, Cidade %d\n", regiao_melhor_cidade, melhor_cidade);
    #endif

    free(notas);

    return 0;
}
