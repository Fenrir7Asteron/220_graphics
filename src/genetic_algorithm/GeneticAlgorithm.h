//
// Created by bogdan on 30.03.19.
//

#ifndef INC_220_GRAPHICS_GENETICALGORITHM_H
#define INC_220_GRAPHICS_GENETICALGORITHM_H

#include <SFML/Graphics.hpp>
#include <thread>
#include <cmath>
#include <algorithm>
#include "Ascii.h"
#include "Population.h"

#define MAX_GENERATION 6000
#define MAX_POPULATION_SIZE 49
#define MUTATION_RATE 10

class GeneticAlgorithm {
public:
    GeneticAlgorithm();

    Population make_initial_population();
    std::vector <double> fitness(std::vector <double> &grayscale_blocks, Population &population);
    void crossover(Population* population, std::vector <double> &fitnesses);
    void mutate(Population* population, std::vector <double> &grayscale_blocks);
    int get_best(std::vector <double> &grayscale_blocks, Population &population);

    void fitness_thread(std::vector<double> &grayscale_blocks, Population &population, int i, std::vector <double> &result);
    void crossover_thread(Population* population, int i, int j, int k, int index, Population &result);

    Ascii ascii;
private:
    std::uniform_real_distribution <double> unif;
    std::default_random_engine re;
    int generation;
};


#endif //INC_220_GRAPHICS_GENETICALGORITHM_H
