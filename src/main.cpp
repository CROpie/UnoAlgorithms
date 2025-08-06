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

  game.addPlayer(Player("chris", 1, Strategy::colour));
  game.addPlayer(Player("leah", 2, Strategy::number));

  game.dealStartingCards(game.HAND_SIZE);

  Renderer renderer(window);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event))
      if (event.type == sf::Event::Closed) window.close();

      game.play();
      renderer.render(game);

      sf::sleep(sf::milliseconds(100));

      if (game.hasPlayerWon()) {
        game.awardWin();
        if (game.hasPlayerWonNGames(game.REQ_WINS)) { 
          game.logWins("../game_log.txt");
          return 0;
        }

        game.finishAndRestart();
        continue;
      }
      
      game.advanceTurn();
  }
  return 0;
}