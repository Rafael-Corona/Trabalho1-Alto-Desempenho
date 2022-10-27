'''
    -> Arquivos tempos na forma:
    <linhas do arquivo>
    <total_threads_utilizadas> <tempo_tamanho_1> <tempo_tamanho_2> ... <tempo_tamanho_n>
    <total_threads_utilizadas> <tempo_tamanho_1> <tempo_tamanho_2> ... <tempo_tamanho_n>
    <total_threads_utilizadas> <tempo_tamanho_1> <tempo_tamanho_2> ... <tempo_tamanho_n>
    <total_threads_utilizadas> <tempo_tamanho_1> <tempo_tamanho_2> ... <tempo_tamanho_n>
    ...
'''

import matplotlib.pyplot as plt

arquivo = open('tempos.txt')

populacao = [100, 1000, 100000, 100000, 1000000]
threads = []
matriz_tempos = []

total_plotagens = int(arquivo.readline().strip())

for i in range(total_plotagens):
    linha = arquivo.readline().split()
    threads.append(int(linha[0]))
    tempos = []
    for j in range(len(populacao)):
        tempos.append(float(linha[j+1]))
    matriz_tempos.append(tempos)

plt.xlabel('Total de Alunos')
plt.ylabel('Tempo')

for i in range(total_plotagens):
    plt.plot(populacao, matriz_tempos[i], label= str(threads[i]) + ' threads')

plt.legend()
plt.show()
