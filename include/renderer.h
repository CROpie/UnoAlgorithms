#include <SFML/Graphics.hpp>
#include <game.h>
#include <card.h>
#include <string.h>

class Renderer {
    public:
        Renderer(sf::RenderWindow& window);

        void render(const Game& game);

    private:
        sf::RenderWindow& window;
        std::map<std::string, sf::Texture> textures;
        int TOP = 22;
        int CENTER_X = 347;
        int CENTER_Y = 211;
        int BOTTOM = 400;
        int LEFT_PADDING = 50;
        int OFFSET = 30;

        void loadTextures();
};