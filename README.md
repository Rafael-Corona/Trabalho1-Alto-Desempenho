# Trabalho-Alto-Desempenho
Repositório destinado para produção do trabalho 1 da disciplina de Computação de Alto Desempenho

## Descrição do Projeto
Este projeto tem como objetivo analisar as notas de alunos do ensino médio de todo o país. O problema consiste em processar e extrair informações estatísticas sobre as notas em diferentes níveis: municipal, regional e nacional.

O código desenvolvido tem duas versões:
1. **Versão Sequencial**: Utiliza um único núcleo para realizar todas as computações.
2. **Versão Paralela**: Permite a execução em múltiplas threads, sendo o número de threads definido no código.

Ao final, os resultados das duas abordagens são comparados para analisar o speedup e a eficiência da versão paralela em relação à versão sequencial.

---
## Entrada
O código recebe as notas dos alunos, considerando a seguinte estrutura:
- **R**: Número de regiões.
- **C**: Número de cidades por região.
- **A**: Número de alunos por cidade.

Cada aluno possui uma nota individual, e o conjunto de notas será processado para gerar os resultados.

---
## Saída
O código responde as seguintes questões, nos níveis municipal, regional e nacional:
1. **Menor nota registrada**
2. **Maior nota registrada**
3. **Contagem de frequência das notas**
4. **Soma total das notas**

Os resultados são apresentados para cada cidade, região e para todo o país.

---
## Implementação
### Versão Sequencial
- Realiza todas as computações utilizando um único núcleo.
- Adequado para comparação de desempenho com a versão paralela.

### Versão Paralela
- Utiliza processamento paralelo para dividir a carga de trabalho entre múltiplas threads.
- Melhor performance em sistemas multicore.

---
## Avaliação de Desempenho
Os resultados obtidos são analisados sob os seguintes aspectos:
- **Speedup**: Mede o ganho de desempenho da versão paralela em relação à sequencial.
- **Eficiência**: Avalia a escalabilidade da versão paralela e o impacto do aumento do número de threads.

---
## Como Executar
### Compilar e Executar a Versão Sequencial:
```sh
make all
make sequencial
```

### Compilar e Executar a Versão Paralela:
```sh
make all
make paralelo
```

---
## Tecnologias Utilizadas
- Linguagem C
- OpenMP (para paralelismo)
- Análise de desempenho com medição de tempo de execução


