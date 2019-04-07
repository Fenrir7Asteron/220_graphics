//
// Created by bogdan on 05.04.19.
//

#ifndef INC_220_GRAPHICS_POPULATION_H
#define INC_220_GRAPHICS_POPULATION_H

#include <SFML/Graphics.hpp>
#include <vector>

class Population {
public:
    Population();
    Population(int sz);

    std::vector<sf::Image> images;
    std::vector<std::string> ascii_strings;
    std::vector<std::vector<sf::Uint8>> transparencies;

    int size();
};


#endif //INC_220_GRAPHICS_POPULATION_H
