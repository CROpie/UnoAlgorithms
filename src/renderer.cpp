#include <renderer.h>

Renderer::Renderer(sf::RenderWindow& window): window(window) { 

    if (!font.loadFromFile("../assets/fonts/arial.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }
    loadTextures();
}

void Renderer::loadTextures() {
    for (Colour colour: all_colours) {
        if (colour == Colour::black) {
            std::string key = "black_wild";
            std::string path = "../assets/cards/black/wild.png";

            sf::Texture texture;
            if (!texture.loadFromFile(path)) {
                throw std::runtime_error("Failed to load texture: " + path);
            }
            textures[key] = std::move(texture);
            std::string key2 = "black_drawfour";
            std::string path2 = "../assets/cards/black/drawfour.png";

            sf::Texture texture2;
            if (!texture2.loadFromFile(path2)) {
                throw std::runtime_error("Failed to load texture: " + path2);
            }
            textures[key2] = std::move(texture2);
            continue;
        }

        for (Value value : all_values) {
            if (value == Value::wild || value == Value::drawfour) continue;
            std::string colStr = Card::toColString(colour);
            std::string valStr = Card::toValString(value);
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

void Renderer::render(Game& game) {
    window.clear();

    for (Player& player : game.players) {

        int yPos = player.playerNumber == 1 ? TOP : BOTTOM;

        sf::Color nameCol = player.playerNumber == game.players[game.turn].playerNumber ? CURRENT_COLOUR : TEXT_COLOUR;

        int yOffset = 0;
        sf::Text playerText;
        playerText.setFont(font);
        playerText.setCharacterSize(FONT_SIZE);
        playerText.setFillColor(nameCol);
        playerText.setPosition(RIGHT_PADDING, yPos + yOffset);
        playerText.setString(player.name);
        window.draw(playerText);

        yOffset += 30;
        playerText.setPosition(RIGHT_PADDING, yPos + yOffset);
        playerText.setString(player.strategyName);
        window.draw(playerText);

        yOffset += 30;
        playerText.setPosition(RIGHT_PADDING, yPos + yOffset);
        playerText.setString(std::to_string(player.wins));
        window.draw(playerText);

        if (player.selectedCondition.has_value()) {
            yOffset += 30;
            playerText.setPosition(RIGHT_PADDING, yPos + yOffset);
            playerText.setString(player.toConditionNameString(player.selectedCondition.value()));
            window.draw(playerText);
        }

        if (player.selectedAction.has_value()) {
            yOffset += 30;
            playerText.setPosition(RIGHT_PADDING, yPos + yOffset);
            playerText.setString(player.toPlayActionString(player.selectedAction.value()));
            window.draw(playerText);
        }

        int offset = 0;
        for (Card& card : player.hand) {
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