CC = gcc
FLAGS = -Wall -Werror -Wextra -lm -fopenmp
SEQUENCIAL = studentsseq
PARALELO = studentspar

all:
	$(CC) -o $(SEQUENCIAL) $(SEQUENCIAL).c $(FLAGS)
	$(CC) -o $(PARALELO) $(PARALELO).c $(FLAGS)
debug:
	$(CC) -o $(SEQUENCIAL) $(SEQUENCIAL).c $(FLAGS) -DDEBUG
	$(CC) -o $(PARALELO) $(PARALELO).c $(FLAGS) -DDEBUG

time:
	$(CC) -o $(SEQUENCIAL) $(SEQUENCIAL).c $(FLAGS) -DRESPONSE_TIME_TESTING
	$(CC) -o $(PARALELO) $(PARALELO).c $(FLAGS) -DRESPONSE_TIME_TESTING

sequencial:
	./$(SEQUENCIAL)

paralelo:
	./$(PARALELO)
	
