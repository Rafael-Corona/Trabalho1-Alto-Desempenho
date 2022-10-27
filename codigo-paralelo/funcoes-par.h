#define NOTAS_POSSIVEIS 101
#define NOME_ARQ_ENTRADA "entrada.txt"

#define TOTAL_THREADS 2

#ifdef DEBUG
unsigned teste[72] = {
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

void atribuir_parametros_entrada(unsigned *total_regioes, unsigned *cidades_por_regiao, unsigned *alunos_por_cidade, unsigned *semente)
{

    #ifdef DEBUG
    *total_regioes = 3;
    *cidades_por_regiao = 4;
    *alunos_por_cidade = 6;
    *semente = 7;
    #else
    FILE *fp = fopen(NOME_ARQ_ENTRADA, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Erro ao abrir arquivo com as informações de entrada!\n");
        fprintf(stderr, "Verifique o arquivo de nome \"entrada.txt\"\n");
        exit(-1);
    }

    if(fscanf(fp, "%u", total_regioes) == 1);
    else
    {
        fprintf(stderr, "Erro ao extrair informações do arquivo de entrada!\n");
        fprintf(stderr, "Verifique o arquivo de nome \"entrada.txt\" e seu conteúdo\n");
        exit(-1);
    }

    if(fscanf(fp, "%u", cidades_por_regiao) == 1);
    else
    {
        fprintf(stderr, "Erro ao extrair informações do arquivo de entrada!\n");
        fprintf(stderr, "Verifique o arquivo de nome \"entrada.txt\" e seu conteúdo\n");
        exit(-1);
    }

    if(fscanf(fp, "%u", alunos_por_cidade) == 1);
    else
    {
        fprintf(stderr, "Erro ao extrair informações do arquivo de entrada!\n");
        fprintf(stderr, "Verifique o arquivo de nome \"entrada.txt\" e seu conteúdo\n");
        exit(-1);
    }

    if(fscanf(fp, "%u", semente) == 1);
    else
    {
        fprintf(stderr, "Erro ao extrair informações do arquivo de entrada!\n");
        fprintf(stderr, "Verifique o arquivo de nome \"entrada.txt\" e seu conteúdo\n");
        exit(-1);
    }

    fclose(fp);
    #endif
}


unsigned *gerar_notas(unsigned total_notas)
{
    unsigned *notas = (unsigned*) malloc(total_notas * sizeof(unsigned));

    for (unsigned i = 0; i < total_notas; i++)
    {
        #ifdef DEBUG
        notas[i] = teste[i];
        #else
        notas[i] = rand() % NOTAS_POSSIVEIS;
        #endif
    }

    return notas;
}

double obter_menor_valor_double(double *notas, unsigned idx_inicio, unsigned idx_fim)
{
    double menor_media = NOTAS_POSSIVEIS;

    for (unsigned i = idx_inicio; i <= idx_fim; i++)
    {
        if (notas[i] < menor_media)
            menor_media = notas[i];
    }

    return menor_media;
}

double obter_maior_valor_double(double *notas, unsigned idx_inicio, unsigned idx_fim)
{
    double maior_media = NOTAS_POSSIVEIS;

    for (unsigned i = idx_inicio; i <= idx_fim; i++)
    {
        if (notas[i] < maior_media)
            maior_media = notas[i];
    }

    return maior_media;
}

unsigned obter_menor_valor_int(unsigned *notas, unsigned idx_inicio, unsigned idx_fim)
{
    unsigned menor_nota = NOTAS_POSSIVEIS;

    for (unsigned i = idx_inicio; i <= idx_fim; i++)
    {
        if (notas[i] < menor_nota)
            menor_nota = notas[i];
    }

    return menor_nota;
}

unsigned obter_maior_valor_int(unsigned *notas, unsigned idx_inicio, unsigned idx_fim)
{
    unsigned maior_nota = 0;

    for (unsigned i = idx_inicio; i <= idx_fim; i++)
    {
        if (notas[i] > maior_nota)
            maior_nota = notas[i];
    }

    return maior_nota;
}

unsigned obter_menor_valor_long(unsigned long *notas, unsigned idx_inicio, unsigned idx_fim)
{
    unsigned menor_nota = NOTAS_POSSIVEIS;

    for (unsigned i = idx_inicio; i <= idx_fim; i++)
    {
        if (notas[i] < menor_nota)
            menor_nota = notas[i];
    }

    return menor_nota;
}

unsigned obter_maior_valor_long(unsigned long *notas, unsigned idx_inicio, unsigned idx_fim)
{
    unsigned maior_nota = 0;

    for (unsigned i = idx_inicio; i <= idx_fim; i++)
    {
        if (notas[i] > maior_nota)
            maior_nota = notas[i];
    }

    return maior_nota;
}

unsigned *somar_contagens(unsigned **lista_contagens, unsigned idx_inicio, unsigned idx_fim)
{
    unsigned *resultado = (unsigned*) calloc(NOTAS_POSSIVEIS, sizeof(unsigned));

    for (unsigned i = idx_inicio; i <= idx_fim; i++)
    {
        for (unsigned j = 0; j < NOTAS_POSSIVEIS; j++)
        {  
            resultado[j] += lista_contagens[i][j];
            //resultado[j] += 1;
            
        }
    }
    return resultado;
}

void free_lista_contagem(unsigned **lista_contagens, unsigned size)
{
    for (unsigned i = 0; i < size; i++)
    {
        free(lista_contagens[i]);
    }
    free(lista_contagens);

}

void obter_menor_maior_soma(unsigned *notas, unsigned *menor_nota, unsigned *maior_nota, 
                                            unsigned long *soma_notas, unsigned idx_inicio, unsigned idx_fim)
{
    unsigned maior = 0, menor = NOTAS_POSSIVEIS;
    unsigned long soma = 0;

    #pragma omp sections
    {
        #pragma omp section
        {
            for (unsigned i = idx_inicio; i <= idx_fim; i++)
            {
                soma += notas[i];
            }  
        }
        #pragma omp section
        {
            for (unsigned i = idx_inicio; i <= idx_fim; i++)
            {
                if (notas[i] > maior)
                    maior = notas[i];
            }  
        }
        #pragma omp section
        {
            for (unsigned i = idx_inicio; i <= idx_fim; i++)
            {
                if (notas[i] < menor)
                    menor = notas[i];
            }  
        }
    }

    *maior_nota = maior;
    *menor_nota = menor;
    *soma_notas = soma;
}

unsigned *obter_contagem(unsigned *notas, unsigned idx_inicio, unsigned idx_fim)
{
    unsigned *contagem = (unsigned*) calloc(NOTAS_POSSIVEIS, sizeof(unsigned));

    for (unsigned i = idx_inicio; i <= idx_fim; i++)
    {
        contagem[notas[i]]++;
    }  

    return contagem;
}

unsigned obter_melhor_regiao(unsigned long *soma_de_cada_regiao, unsigned idx_inicio, unsigned idx_fim)
{
    unsigned regiao = 0;
    unsigned maior_soma = 0;

    for (unsigned i = idx_inicio; i <= idx_fim; i++)
    {
        if (soma_de_cada_regiao[i] > maior_soma){
            regiao = i;
            maior_soma = soma_de_cada_regiao[i];
        }
    }
    return regiao;
}

void melhor_cidade_e_sua_regiao(unsigned long *soma_de_cada_cidade, unsigned total_regioes, unsigned cidades_por_regiao ,unsigned *melhor_cidade, unsigned*regiao)
{
    *melhor_cidade = 0;
    *regiao = 0;

    unsigned long maior_soma = 0;
    unsigned last = total_regioes * cidades_por_regiao-1;

    for (unsigned i = 0; i <= last; i++)
    {
        if (soma_de_cada_cidade[i] > maior_soma){
            *melhor_cidade = i % cidades_por_regiao;
            maior_soma = soma_de_cada_cidade[i];
            *regiao = i / cidades_por_regiao;
        }
    }
}

unsigned soma_vetor_int(unsigned *notas, unsigned idx_inicio, unsigned idx_fim)
{
    unsigned soma = 0;

    for (unsigned i = idx_inicio; i <= idx_fim; i++)
        soma += notas[i];

    return soma;
}

unsigned long soma_vetor_long(unsigned long *notas, unsigned idx_inicio, unsigned idx_fim)
{
    unsigned long soma = 0;

    for (unsigned i = idx_inicio; i <= idx_fim; i++)
        soma += notas[i];

    return soma;
}

double soma_vetor_double(double *medias, unsigned idx_inicio, unsigned idx_fim)
{
    double soma = 0;

    for (unsigned i = idx_inicio; i <= idx_fim; i++)
        soma += medias[i];

    return soma;
}


double calcular_soma_para_dp(unsigned *notas, double media, unsigned idx_inicio, unsigned idx_fim)
{
    double soma = 0;

    for (unsigned i = idx_inicio; i <= idx_fim; i++)
        soma += pow(notas[i] - media, 2);

    return soma;
}


unsigned *construir_contagem(unsigned *notas, unsigned idx_inicio, unsigned idx_fim)
{
    unsigned *contagem = (unsigned*) calloc(NOTAS_POSSIVEIS, sizeof(unsigned));

    for (unsigned i = idx_inicio; i <= idx_fim; i++)
        contagem[notas[i]]++;
    
    return contagem;
}

double calcular_mediana(unsigned *contagem, unsigned total)
{
    unsigned  qtd_contados = 0, metade = total / 2;

    if (total % 2 != 0)
    {
        for (unsigned i = 0; i < NOTAS_POSSIVEIS; i++)
        {
            for (unsigned j = contagem[i]; j > 0; j--)
            {
                if (qtd_contados == metade)
                    return i;
                qtd_contados++;
            }
        }
    }
    else
    {
        unsigned meio1, meio2;
        for (unsigned i = 0; i < NOTAS_POSSIVEIS; i++)
        {
            for (unsigned j = contagem[i]; j > 0; j--)
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
void imprimir_matriz(unsigned *notas, unsigned regiao, unsigned cidades_por_regiao, unsigned alunos_por_cidade)
{
    unsigned idx_inicio = regiao * cidades_por_regiao * alunos_por_cidade;

    printf("-> Região %d:\n", regiao);
    for (unsigned i = 0; i < cidades_por_regiao; i++)
    {
        for (unsigned j = 0; j < alunos_por_cidade; j++)
            printf("%3d  ", notas[idx_inicio + i * alunos_por_cidade + j]);
        printf("\n");
    }
    printf("\n");
}
#endif

#ifdef RESPONSE_TIME_TESTING
#define MB (1024 * 1024)
#define LARGEST_CACHE_SZ (8 * MB) //Tamanho da L3 dos pcs do cluster
static unsigned char dummy_buffer[LARGEST_CACHE_SZ];

void clean_cache()
{
    unsigned long long i;
    for (i=0; i<LARGEST_CACHE_SZ; i++)
    dummy_buffer[i] += 1;
}
#endif
