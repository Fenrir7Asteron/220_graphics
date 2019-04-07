#include <iostream>
#include "genetic_algorithm/GeneticAlgorithm.h"
#include <SFML/Graphics.hpp>
#include <Magick++.h>

#define INPUT_FILE "gustav_klimt.jpg"
#define MONOCHROME_FILE "monochrome.png"
#define HQ_FILE "hq_output4.png"
#define OUTPUT_FILE "output6.png"

int main(int argc,char **argv)
{
    // Convert input image to monochrome
    Magick::Image image_to_monochrome(INPUT_FILE);
    image_to_monochrome.type(Magick::GrayscaleType);
    image_to_monochrome.write(MONOCHROME_FILE);

    // Load input image
    sf::Image image;
    if (!image.loadFromFile(MONOCHROME_FILE))
    {
        std::cerr << "No input file named " << INPUT_FILE << "\n";
    }

    Ascii drawer = Ascii();
    // Break input image into blocks and calculate their average shade of gray
    auto grayscale_blocks = drawer.calculate_blocks_grayscale(image);

    // Apply genetic algorithm
    GeneticAlgorithm ga;
    auto population = ga.make_initial_population();

    for (int generation = 1; generation <= MAX_GENERATION; ++generation) {
        auto fitnesses = ga.fitness(grayscale_blocks, population);
        if (generation % 10 == 0) {
            std::cout << "Generation: " << generation << std::endl;
            for (auto fit : fitnesses) {
                std::cout << fit << " ";
            }
            std::cout << std::endl;
        }

        ga.crossover(&population, fitnesses);
        ga.mutate(&population, grayscale_blocks);

        // Save intermediate results to see the progress
        if (generation % 200 == 0) {
            auto best = ga.get_best(grayscale_blocks, population);
            drawer.draw_ascii_on_image(population.images[best], {population.ascii_strings[best], population.transparencies[best]},
                                       ASCII_IMAGE_HEIGHT * FONT_SIZE, ASCII_IMAGE_WIDTH * FONT_SIZE);
            population.images[best].saveToFile(HQ_FILE);
        }

    }

    // Get best individ of the final population and write it to file
    auto best = ga.get_best(grayscale_blocks, population);
    drawer.draw_ascii_on_image(population.images[best], {population.ascii_strings[best], population.transparencies[best]},
            ASCII_IMAGE_HEIGHT * FONT_SIZE, ASCII_IMAGE_WIDTH * FONT_SIZE);
    population.images[best].saveToFile(HQ_FILE);

    // Resize the final image to 512x512
    Magick::Image resized_image(HQ_FILE);
    resized_image.resize(Magick::Geometry(512, 512));
    resized_image.write(OUTPUT_FILE);

    return 0;
}