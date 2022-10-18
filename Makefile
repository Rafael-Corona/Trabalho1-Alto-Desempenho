CC = gcc
FLAGS = -Wall -Werror -Wextra -lm -fopenmp
SEQUENCIAL = studentsseq
PARALELO = studentspar

all:
	$(CC) -o $(SEQUENCIAL) $(SEQUENCIAL).c $(FLAGS)
	$(CC) -o $(PARALELO) $(PARALELO).c $(FLAGS)

sequencial:
	./$(SEQUENCIAL)

paralelo:
	./$(PARALELO)
	