CC = g++
CPPFLAGS = -g -Wall -O2 -std=gnu++11
LDFLAGS =

OBJ = solversexecutor.o LocalSearchSolver.o TabuSearchSolver.o main.o

%.o: %.cpp
		$(CC) $(CPPFLAGS) -c $^ -o $@

main: $(OBJ)
		$(CC) $(CPPFLAGS) $(OBJ) -o main 
		
clean:
		rm -rf $(OBJ) main

.PHONY: clean
