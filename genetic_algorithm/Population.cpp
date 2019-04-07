//
// Created by bogdan on 05.04.19.
//

#include "Population.h"

Population::Population() {

}

Population::Population(int sz) {
    ascii_strings = std::vector<std::string> (sz);
    images = std::vector<sf::Image> (sz);
    transparencies = std::vector<std::vector<sf::Uint8>> (sz);
}

int Population::size() {
    return (int) images.size();
}