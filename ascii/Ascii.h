//
// Created by bogdan on 30.03.19.
//

#include <SFML/Graphics.hpp>
#include <random>

#ifndef INC_220_GRAPHICS_DRAWER_H
#define INC_220_GRAPHICS_DRAWER_H

#define FONT_SIZE 32
#define IMAGE_HEIGHT 512
#define ASCII_IMAGE_HEIGHT 128
#define ASCII_IMAGE_WIDTH 128
#define NUMBER_OF_TRANSPARENCY_STEPS 2
#define NUMBER_OF_TRANSPARENCY_STEPS_PRECISE1 10
#define NUMBER_OF_TRANSPARENCY_STEPS_PRECISE2 20

class Ascii {
public:
    Ascii();

    char generate_ascii_symbol();
    int symbol_grayscale(char symbol);
    char grayscale_symbol(int grayscale);
    char nearest_symbol(int grayscale);
    std::pair<std::string, std::vector <sf::Uint8>> generate_ascii();
    void draw_ascii_on_image(sf::Image &image, std::pair<std::string, std::vector <sf::Uint8>> to_draw, int xr, int yr);
    std::vector <double> calculate_blocks_grayscale(sf::Image &image);

    std::mt19937 gen;
private:
    std::vector <char> available_symbols;
    std::map <int, char> nearest_symbol_map;
    sf::Font font;
};


#endif //INC_220_GRAPHICS_DRAWER_H
