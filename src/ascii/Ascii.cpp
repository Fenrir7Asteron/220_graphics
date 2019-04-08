//
// Created by bogdan on 30.03.19.
//

#include "Ascii.h"
#include <iostream>

Ascii::Ascii() {
    gen.seed(time(NULL));

    available_symbols.emplace_back('#');
    available_symbols.emplace_back('2');
    available_symbols.emplace_back('D');
    available_symbols.emplace_back('0');
    available_symbols.emplace_back(':');
    available_symbols.emplace_back('.');
    available_symbols.emplace_back(' ');

    for (int grayscale = 0; grayscale < 256; ++grayscale) {
        nearest_symbol_map[grayscale] = grayscale_symbol(grayscale);

        if (nearest_symbol_map[grayscale] == -1) {
            for (int delta = 1; delta <= std::min(grayscale, 255 - grayscale); ++delta) {
                if (grayscale_symbol(grayscale - delta) != -1) {
                    nearest_symbol_map[grayscale] = grayscale_symbol(grayscale - delta);
                    break;
                }

                if (grayscale_symbol(grayscale + delta) != -1) {
                    nearest_symbol_map[grayscale] = grayscale_symbol(grayscale + delta);
                    break;
                }
            }

            if (nearest_symbol_map[grayscale] == -1) {
                for (int i = grayscale; i >= 0; --i) {
                    if (grayscale_symbol(i) != -1) {
                        nearest_symbol_map[grayscale] = grayscale_symbol(i);
                        break;
                    }
                }

                for (int i = grayscale; i < 256; ++i) {
                    if (grayscale_symbol(i) != -1) {
                        nearest_symbol_map[grayscale] = grayscale_symbol(i);
                        break;
                    }
                }
            }
        }
    }

    font.loadFromFile("courier.ttf");
}

char Ascii::generate_ascii_symbol() {
    return available_symbols[gen() % available_symbols.size()];
}

int Ascii::symbol_grayscale(char symbol) {
    switch (symbol) {
        case '#':
            return 0;
        case '2':
            return 50;
        case 'D':
            return 70;
        case '0':
            return 100;
        case ':':
            return 130;
        case '.':
            return 180;
        case ' ':
            return 220;
        default:
            return -1;
    }
}

char Ascii::grayscale_symbol(int grayscale) {
    switch (grayscale) {
        case 0:
            return '#';
        case 50:
            return '2';
        case 70:
            return 'D';
        case 100:
            return '0';
        case 130:
            return ':';
        case 180:
            return '.';
        case 220:
            return ' ';
        default:
            return -1;
    }
}

char Ascii::nearest_symbol(int grayscale) {
    return nearest_symbol_map[grayscale];
}


std::pair<std::string, std::vector <sf::Uint8>> Ascii::generate_ascii() {
    std::string to_draw;
    std::vector <sf::Uint8> transparencies;
    for (int i = 0; i < ASCII_IMAGE_HEIGHT; ++i) {
        for (int j = 0; j < ASCII_IMAGE_WIDTH; ++j) {
            char random_char = generate_ascii_symbol();
            to_draw += random_char;
            int transparency = gen() % NUMBER_OF_TRANSPARENCY_STEPS * (255 / (NUMBER_OF_TRANSPARENCY_STEPS - 1));
            transparencies.emplace_back(std::max(1, transparency));
        }

        to_draw += '\n';
        transparencies.emplace_back(1);
    }

    return {to_draw, transparencies};
}

sf::Image Ascii::draw_ascii_on_image(std::pair<std::string, std::vector <sf::Uint8>> to_draw, int xr, int yr) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(FONT_SIZE);
    text.setStyle(sf::Text::Bold);

    sf::RenderTexture renderTexture;
    renderTexture.create(xr * 0.65, yr * 0.65);
    renderTexture.clear(sf::Color::White);

    int z = 0;
    for (double i = 0, a = 0; a < ASCII_IMAGE_HEIGHT; i += FONT_SIZE * 0.65, ++a) {
        for (double j = 0, b = 0; b < ASCII_IMAGE_WIDTH; j += FONT_SIZE * 0.65, ++b) {
            text.setString(to_draw.first[z]);
            text.setFillColor(sf::Color(0, 0, 0, to_draw.second[z]));
            text.setPosition(j, i);
            renderTexture.draw(text);
            ++z;
        }
        ++z;
    }

    renderTexture.display();
    auto texture = renderTexture.getTexture();
    return texture.copyToImage();
}

std::vector <double> Ascii::calculate_blocks_grayscale(sf::Image &image) {
    std::vector <double> result;

    for (int i = 0; i < ASCII_IMAGE_HEIGHT; ++i) {
        for (int j = 0; j < ASCII_IMAGE_WIDTH; ++j) {
            double block_grayscale = 0;
            int block_size = IMAGE_HEIGHT / ASCII_IMAGE_HEIGHT;

            for (int a = 0; a < block_size; ++a) {
                for (int b = 0; b < block_size; ++b) {
                    block_grayscale += image.getPixel(j * block_size + b, i * block_size + a).r;
                }
            }
            block_grayscale /= block_size * block_size;
            result.push_back(block_grayscale);
        }
        result.push_back(0);
    }
    return result;
}
