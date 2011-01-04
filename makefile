CC=g++
CFLAGS= -g -time 
LDFLAGS= -lboost_thread
SOURCES=main.cpp definitions.h semaphore.h coordinate.h individual.h population.h population_thread.h genetic_algorithm.h genetic_algorithm_thread.h genetic_operator.h genetic_operator_thread.h selection_by_roulette.h selection_by_tournament.h cross_over.h cross_over_thread.h mutate_bit_by_bit.h mutate_bit_by_bit_thread.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ga

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)



