#!/bin/bash

g++ -g -l boost_thread -o ga main.cpp definitions.h semaphore.h coordinate.h individual.h population.h population_thread.h genetic_algorithm.h genetic_algorithm_thread.h genetic_operator.h genetic_operator_thread.h selection_by_roulette.h selection_by_tournament.h cross_over.h cross_over_thread.h mutate_bit_by_bit.h mutate_bit_by_bit_thread.h selection_by_tournament_operator.h 2>erro
grep -i error erro
grep -c error erro

