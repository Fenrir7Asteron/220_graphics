//
// Created by bogdan on 30.03.19.
//

#include "GeneticAlgorithm.h"
#include <iostream>

GeneticAlgorithm::GeneticAlgorithm() {
    Ascii ascii = Ascii();

    unif = std::uniform_real_distribution <double> (0, 255);
    std::random_device rd;
    re = std::default_random_engine(rd());

    generation = 1;
}

Population GeneticAlgorithm::make_initial_population() {
    Population result;
    for (int i = 0; i < MAX_POPULATION_SIZE; ++i) {
        auto text = ascii.generate_ascii();
        auto& ascii_string = text.first;
        auto& transparencies = text.second;
        sf::Image image;
        image.create(ASCII_IMAGE_WIDTH * 4, ASCII_IMAGE_HEIGHT * 4, sf::Color(255, 255, 255));
        result.images.push_back(image);
        result.ascii_strings.push_back(ascii_string);
        result.transparencies.push_back(transparencies);
    }
    return result;
}

void GeneticAlgorithm::fitness_thread(std::vector<double> &grayscale_blocks, Population &population, int i, std::vector <double> &result) {
    double score = 0;

    for (unsigned row = 0; row < ASCII_IMAGE_HEIGHT; ++row) {
        for (unsigned column = 0; column < ASCII_IMAGE_WIDTH; ++column) {
            int index = row * ASCII_IMAGE_WIDTH + column;
            auto image_gray = grayscale_blocks[index];
            double individual_gray = ascii.symbol_grayscale(population.ascii_strings[i][index]);
            individual_gray = 255 - individual_gray;
            individual_gray *= (double) population.transparencies[i][index] / 255;
            individual_gray = 255 - individual_gray;

            double distance = abs((int) image_gray - (int) individual_gray);

            score += distance;
        }
    }

    result[i] = score;
}

std::vector <double> GeneticAlgorithm::fitness(std::vector <double> &grayscale_blocks, Population &population) {
    std::vector <double> result(population.size());

    std::vector <std::thread> threads;
    for (int i = 0; i < population.size(); ++i) {
        threads.emplace_back(std::thread(&GeneticAlgorithm::fitness_thread, this, std::ref(grayscale_blocks),
                std::ref(population), i, std::ref(result)));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return result;
}

void GeneticAlgorithm::crossover_thread(Population* population, int i, int j, int k, int index, Population &result) {
    auto father_string = (population->ascii_strings)[i];
    auto mother_string = (population->ascii_strings)[j];
    auto  child_string = (population->ascii_strings)[k];
    auto father_transparency = (population->transparencies)[i];
    auto mother_transparency = (population->transparencies)[j];
    auto  child_transparency = (population->transparencies)[k];

    for (int z = 0; z < father_string.size(); ++z) {
        int choice = ascii.gen() % 2;
        if (choice) {
            child_string[z] = father_string[z];
            child_transparency[z] = father_transparency[z];
        } else {
            child_string[z] = mother_string[z];
            child_transparency[z] = mother_transparency[z];
        }
    }
    result.images[index] = (population->images)[k];
    result.ascii_strings[index] = child_string;
    result.transparencies[index] = child_transparency;
}

void GeneticAlgorithm::crossover(Population* population, std::vector <double> &fitnesses) {
    std::vector<std::pair <double, int>> sorted_individual_numbers(fitnesses.size());
    for (int i = 0; i < fitnesses.size(); ++i) {
        sorted_individual_numbers[i] = std::make_pair(fitnesses[i], i);
    }
    std::sort(sorted_individual_numbers.begin(), sorted_individual_numbers.end());

    std::vector <std::thread> threads;
    Population result(population->size());
    int k = 0;
    int to_select = (int) (sqrt(population->size()) + 0.01);
    for (int i = 0; i < to_select; ++i) {
        for (int j = 0; j < to_select; ++j) {
            int sorted_i = sorted_individual_numbers[i].second;
            int sorted_j = sorted_individual_numbers[j].second;
            int sorted_k = sorted_individual_numbers[k].second;

            threads.emplace_back(std::thread(&GeneticAlgorithm::crossover_thread, this,
                                             population, sorted_i, sorted_j, sorted_k, k, std::ref(result)));
            ++k;
        }
    }

    for (auto& thread : threads) {
        thread.join();
    }

    *population = result;
}


void GeneticAlgorithm::mutate(Population* population, std::vector <double> &grayscale_blocks) {
    for (int i = 0; i < population->size(); ++i) {
        auto& ascii_string = (population->ascii_strings)[i];
        auto& transparencies = (population->transparencies)[i];
        int choice = (int) ascii.gen() % MUTATION_RATE;
        if (choice == 0) {
            for (int z = 0; z < ascii_string.size(); ++z) {
                if (ascii_string[z] == '\n') {
                    continue;
                }
                auto image_grayscale = grayscale_blocks[z];
                double individual_grayscale = ascii.symbol_grayscale(ascii_string[z]);
                int transparency = transparencies[z];
                individual_grayscale = 255 - individual_grayscale;
                individual_grayscale *= (double) transparency / 255;
                individual_grayscale = 255 - individual_grayscale;
                double distance = std::abs(individual_grayscale - image_grayscale);

                double probability = unif(re);
                if (probability * 10 <= distance) {
                    int grayscale = ascii.symbol_grayscale(ascii_string[z]);
                    if (generation < 2000) {
                        transparency = 255 / (NUMBER_OF_TRANSPARENCY_STEPS - 1);
                    } else if (generation < 4000) {
                        transparency = 255 / (NUMBER_OF_TRANSPARENCY_STEPS_PRECISE1 - 1);
                    } else {
                        transparency = 255 / (NUMBER_OF_TRANSPARENCY_STEPS_PRECISE2 - 1);
                    }
                    choice = ascii.gen() % 2;
                    if (choice) {
                        grayscale += ascii.gen() % 120;
                        ascii_string[z] = ascii.nearest_symbol(std::min(grayscale, 255));
                        transparencies[z] = std::max(1, transparencies[z] - transparency);
                    } else {
                        grayscale -= ascii.gen() % 120;
                        ascii_string[z] = ascii.nearest_symbol(std::max(grayscale, 0));
                        transparencies[z] = std::min(255, transparencies[z] + transparency);
                    }
                }
            }
        }
    }
    ++generation;
}

int GeneticAlgorithm::get_best(std::vector <double> &grayscale_blocks, Population &population) {
    auto fitnesses = fitness(grayscale_blocks, population);
    double max_fitness = fitnesses[0];
    int best = 0;
    for (int i = 1; i < fitnesses.size(); ++i) {
        if (fitnesses[i] > max_fitness) {
            max_fitness = fitnesses[i];
            best = i;
        }
    }

    return best;
}
