#include <iostream>
#include <vector>
#include <string>
#include "card.h"
#include "deck.h"
#include "game.h"
#include "player.h"
#include "renderer.h"
#include <SFML/Graphics.hpp>

int main() {
   sf::RenderWindow window(sf::VideoMode(800, 600), "Uno Algo");

  Game game;

  game.shuffleDeck();

  game.addPlayer(Player("chris", 1, Strategy::number));
  game.addPlayer(Player("leah", 2, Strategy::colour));

  game.dealToPlayers(7);

  Renderer renderer(window);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event))
      if (event.type == sf::Event::Closed) window.close();

      game.play();
      renderer.render(game);
      sf::sleep(sf::milliseconds(1000));
      if (game.hasPlayerWon()) return 0;
      game.advanceTurn();
  }
  return 0;
}