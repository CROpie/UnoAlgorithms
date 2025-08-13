#include <SFML/Graphics.hpp>
#include <game.h>
#include <card.h>
#include <string.h>
#include <player.h>
#include <strategy.h>

#pragma once

class Renderer {
    public:
        Renderer(sf::RenderWindow& window);
        sf::Font font;

        void render(Game& game);

    private:
        sf::RenderWindow& window;
        std::map<std::string, sf::Texture> textures;
        // cards
        int TOP = 22;
        int CENTER_X = 347;
        int CENTER_Y = 211;
        int BOTTOM = 400;
        int LEFT_PADDING = 50;
        int OFFSET = 30;

        // player stats
        int FONT_SIZE = 20;
        int RIGHT_PADDING = 600;
        sf::Color TEXT_COLOUR = sf::Color::White;
        sf::Color CURRENT_COLOUR = sf::Color::Yellow;

        void loadTextures();
};