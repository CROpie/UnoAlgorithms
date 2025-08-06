#include <iostream>
#include <vector>
#include <string>
#include "card.h"
#include "deck.h"
#include "game.h"
#include "player.h"
#include "renderer.h"
#include <SFML/Graphics.hpp>
#include <chrono>

int main() {

  auto start = std::chrono::high_resolution_clock::now();

  Game game;

  game.shuffleDeck();

  game.addPlayer(Player("chris", 1, Strategy::colour));
  game.addPlayer(Player("leah", 2, Strategy::back));

  game.dealStartingCards(game.HAND_SIZE);

  while (true) {

      game.play();

      if (game.hasPlayerWon()) {
        game.awardWin();
        if (game.hasPlayerWonNGames(game.REQ_WINS)) { 
          auto end = std::chrono::high_resolution_clock::now();
          auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
          game.logWins("../game_log.txt", duration.count());
          return 0;
        }

        game.finishAndRestart();
        continue;
      }
      
      game.advanceTurn();
  }
  return 0;
}

int main_render() {
  auto start = std::chrono::high_resolution_clock::now();

   sf::RenderWindow window(sf::VideoMode(800, 600), "Uno Algo");

  Game game;

  game.shuffleDeck();

  game.addPlayer(Player("chris", 1, Strategy::colour));
  game.addPlayer(Player("leah", 2, Strategy::back));

  game.dealStartingCards(game.HAND_SIZE);

  Renderer renderer(window);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event))
      if (event.type == sf::Event::Closed) window.close();

      game.play();
      renderer.render(game);

      sf::sleep(sf::milliseconds(2000));

      if (game.hasPlayerWon()) {
        game.awardWin();
        if (game.hasPlayerWonNGames(game.REQ_WINS)) {
          auto end = std::chrono::high_resolution_clock::now();
          auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
          game.logWins("../game_log.txt", duration.count());
          return 0;
        }

        game.finishAndRestart();
        continue;
      }
      
      game.advanceTurn();
  }
  return 0;
}