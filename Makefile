CC = gcc
FLAGS = -Wall -Wextra -lm -fopenmp
SEQUENCIAL = studentsseq
PARALELO = studentspar
SEQUENCIAL_FOLDER = codigo-sequencial
PARALLEL_FOLDER = codigo-paralelo

all:
	$(CC) -o $(SEQUENCIAL) ./$(SEQUENCIAL_FOLDER)/$(SEQUENCIAL).c $(FLAGS)  
	$(CC) -o $(PARALELO) ./$(PARALLEL_FOLDER)/$(PARALELO).c $(FLAGS)

debug:
	$(CC) -o $(SEQUENCIAL) ./$(SEQUENCIAL_FOLDER)/$(SEQUENCIAL).c $(FLAGS) -DDEBUG
	$(CC) -o $(PARALELO) ./$(PARALLEL_FOLDER)/$(PARALELO).c $(FLAGS) -DDEBUG

time:
	$(CC) -o $(SEQUENCIAL) ./$(SEQUENCIAL_FOLDER)/$(SEQUENCIAL).c $(FLAGS) -DRESPONSE_TIME_TESTING -Wno-unused
	$(CC) -o $(PARALELO) ./$(PARALLEL_FOLDER)/$(PARALELO).c $(FLAGS) -DRESPONSE_TIME_TESTING -Wno-unused

sequencial:
	./$(SEQUENCIAL)

paralelo:
	./$(PARALELO)
	
