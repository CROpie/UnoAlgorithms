#include <renderer.h>

Renderer::Renderer(sf::RenderWindow& window): window(window) { 

    if (!font.loadFromFile("../assets/fonts/arial.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }
    loadTextures();
}

std::string toVString(Value value) {
    switch (value) {
        case Value::zero: return "zero";
        case Value::one: return "one";
        case Value::two: return "two";
        case Value::three: return "three";
        case Value::four: return "four";
        case Value::five: return "five";
        case Value::six: return "six";
        case Value::seven: return "seven";
        case Value::eight: return "eight";
        case Value::nine: return "nine";
        case Value::reverse: return "reverse";
        case Value::skip: return "skip";
        case Value::drawtwo: return "drawtwo";
        default: return "unknown";
    };
}

std::string toCString(Colour colour) {
    switch (colour) {
        case Colour::red: return "red";
        case Colour::yellow: return "yellow";
        case Colour::green: return "green";
        case Colour::blue: return "blue";
        default: return "unknown";
    };
}

void Renderer::loadTextures() {
    for (Colour colour: all_colours) {
        for (Value value : all_values) {
            std::string colStr = toCString(colour);
            std::string valStr = toVString(value);
            std::string key = colStr + "_" + valStr;
            std::string path = "../assets/cards/" + colStr + "/" + valStr + ".png";

            sf::Texture texture;
            if (!texture.loadFromFile(path)) {
                throw std::runtime_error("Failed to load texture: " + path);
            }
            textures[key] = std::move(texture);
        }
    }
}

void Renderer::render(const Game& game) {
    window.clear();

    for (const Player& player : game.players) {

        int yPos = player.playerNumber == 1 ? TOP : BOTTOM;

        int yOffset = 0;
        sf::Text playerText;
        playerText.setFont(font);
        playerText.setCharacterSize(FONT_SIZE);
        playerText.setFillColor(TEXT_COLOUR);
        playerText.setPosition(RIGHT_PADDING, yPos + yOffset);
        playerText.setString(player.name);
        window.draw(playerText);

        yOffset += 30;
        playerText.setPosition(RIGHT_PADDING, yPos + yOffset);
        playerText.setString(strategyToString(player.strategy));
        window.draw(playerText);

        yOffset += 30;
        playerText.setPosition(RIGHT_PADDING, yPos + yOffset);
        playerText.setString(std::to_string(player.wins));
        window.draw(playerText);

        int offset = 0;
        for (const Card& card : player.hand) {
            sf::Sprite sprite;
            sprite.setTexture(textures.at(card.getTextureKey()));
            sprite.setPosition(LEFT_PADDING + offset, yPos);
            window.draw(sprite);
            offset += OFFSET;
        }
    }
    
    sf::Sprite sprite;
    sprite.setTexture(textures.at(game.deck.discard_pile.back().getTextureKey()));
    sprite.setPosition(CENTER_X, CENTER_Y);
    window.draw(sprite);
    window.display();
}