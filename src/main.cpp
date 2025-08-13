#include <iostream>
#include <vector>
#include <string>
#include "card.h"
#include "deck.h"
#include "game.h"
#include "player.h"
#include "renderer.h"
#include "strategy.h"
#include <SFML/Graphics.hpp>
#include <chrono>

int main_norender() {

  std::vector<ConditionName> conditionNames{ConditionName::DEFAULT};

  // std::vector<ConditionName> conditionNames{ConditionName::OPPONENT_HAD_TO_PICK_UP, ConditionName::OPPONENT_HOLDS_N_CARDS, ConditionName::DEFAULT};
  Strategy randomStrat{conditionNames, "Opponent had to pick up + Opponent holds N cards"};
  Player playerOne{"player", 1, randomStrat};

  Strategy p2PlaysRandom{p2ConditionList, "Playing randomly"};
  Player playerTwo{"opponent", 2, p2PlaysRandom};

  auto start = std::chrono::high_resolution_clock::now();

  Game game;

  game.shuffleDeck();

  game.addPlayer(playerOne);
  game.addPlayer(playerTwo);

  game.dealStartingCards(game.HAND_SIZE);

  while (true) {

      game.play();

      if (game.hasPlayerWon()) {
        game.awardWin();
        if (game.hasPlayerWonNGames(game.REQ_WINS)) { 
          auto end = std::chrono::high_resolution_clock::now();
          auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
          game.logData("../game_data.txt", duration.count());
          game.logWins("../game_wins.txt");
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

  std::vector<ConditionName> conditionNames{ConditionName::OPPONENT_HOLDS_N_CARDS};
  Strategy randomStrat{conditionNames, "Opponent holds 2 cards"};
  Player playerOne{"player", 1, randomStrat};

  Strategy p2PlaysRandom{p2ConditionList, "Playing randomly"};
  Player playerTwo{"opponent", 2, p2PlaysRandom};

  auto start = std::chrono::high_resolution_clock::now();

   sf::RenderWindow window(sf::VideoMode(800, 600), "Uno Algo");

  Game game;

  game.shuffleDeck();

  game.addPlayer(playerOne);

  game.addPlayer(playerTwo);

  game.dealStartingCards(game.HAND_SIZE);
  game.randomStartingPlayer(game.players.size());

  Renderer renderer(window);
  renderer.render(game);

  while (window.isOpen()) {
    bool keyPressed = false;
    sf::Event event;
    // while (window.pollEvent(event))
    //   if (event.type == sf::Event::Closed) window.close();
    while (window.waitEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
        break;
      }

      if (event.type == sf::Event::KeyPressed) {
        keyPressed = true;
        break;
      }
    }

      game.play();
      renderer.render(game);

      // sf::sleep(sf::milliseconds(1000));

      if (game.hasPlayerWon()) {
        game.awardWin();
        if (game.hasPlayerWonNGames(game.REQ_WINS)) {
          auto end = std::chrono::high_resolution_clock::now();
          auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
          game.logData("../game_data.txt", duration.count());
          game.logWins("../game_wins.txt");
          return 0;
        }

        game.finishAndRestart();
        continue;
      }
      
      game.advanceTurn();
  }
  return 0;
}

// int main(int argc, char* argv[]) {
//   std::string choice = argv[1] ? argv[1] : "";

//   int response = 0;

//   if (choice == "render") {
//     response = main_render();
//   } else {
//       response = main_norender();
//   }

//   return response;
// }

Result test_optimized(std::vector<Condition> improvedConditions) {

  Strategy optimized{improvedConditions, "optimized"};
  Player playerOne{"player", 1, optimized};

  Strategy p2PlaysRandom{p2ConditionList, "Playing randomly"};
  Player playerTwo{"opponent", 2, p2PlaysRandom};

  auto start = std::chrono::high_resolution_clock::now();

  Game game;

  game.shuffleDeck();

  game.addPlayer(playerOne);
  game.addPlayer(playerTwo);

  game.dealStartingCards(game.HAND_SIZE);

  while (true) {

      game.play();

      if (game.hasPlayerWon()) {
        game.awardWin();
        if (game.hasPlayerWonNGames(game.REQ_WINS)) { 
          auto end = std::chrono::high_resolution_clock::now();
          auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
          // game.logData("../game_data.txt", duration.count());
          // game.logWins("../game_wins.txt");
          float p1winPercentage = (static_cast<float>(game.players[0].wins)) / (game.players[0].wins + game.players[1].wins) * 100;
          Result result{game.players[0].strategy.conditions, p1winPercentage};
          return result;
        }

        game.finishAndRestart();
        continue;
      }
      
      game.advanceTurn();
  }
  return {};
}

Result gather_stats(ConditionName conditionToTest, std::vector<Condition> improvedConditions) {

  std::vector<ConditionName> conditionNames{conditionToTest};

  Strategy randomStrat{improvedConditions, conditionNames, "name of strategy"};
  Player playerOne{"player", 1, randomStrat};

  Strategy p2PlaysRandom{p2ConditionList, "Playing randomly"};
  Player playerTwo{"opponent", 2, p2PlaysRandom};

  auto start = std::chrono::high_resolution_clock::now();

  Game game;

  game.shuffleDeck();

  game.addPlayer(playerOne);
  game.addPlayer(playerTwo);

  game.dealStartingCards(game.HAND_SIZE);

  while (true) {

      game.play();

      if (game.hasPlayerWon()) {
        game.awardWin();
        if (game.hasPlayerWonNGames(game.REQ_WINS)) { 
          auto end = std::chrono::high_resolution_clock::now();
          auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
          // game.logData("../game_data.txt", duration.count());
          // game.logWins("../game_wins.txt");
          float p1winPercentage = (static_cast<float>(game.players[0].wins)) / (game.players[0].wins + game.players[1].wins) * 100;
          Result result{game.players[0].strategy.conditions, p1winPercentage};
          return result;
        }

        game.finishAndRestart();
        continue;
      }
      
      game.advanceTurn();
  }
  return {};
}


int main() {

  std::vector<Condition> improvedConditions;



  std::vector<ConditionName> conditionsToLearn{
    ConditionName::DEFAULT,
    ConditionName::OPPONENT_HOLDS_N_CARDS,
    ConditionName::OPPONENT_HAD_TO_PICK_UP,
    ConditionName::PLAYER_HOLDS_N_CARDS,
  };

  // number of conditions to iterate over
  for (int i = 0; i < 4; i++) {

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<Result> results;

    // number of tests for a given condition
    for (int j = 0; j < 100; j++) {
      Result result = gather_stats(conditionsToLearn[i], improvedConditions);
      results.emplace_back(result);
    }

    Result best = results.front();
    for (auto& result : results) {
      if (result.winPercentage > best.winPercentage) best = result;
    }
    std::cout << best.winPercentage << std::endl;
    std::cout << Strategy::conditionListToString(best.condition) << std::endl;
    improvedConditions = best.condition;

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << (duration.count() / 60000000) << std::endl;

  }
  std::cout << "STRATEGY GENERATED:\n" << std::endl;
  std::cout << Strategy::conditionListToString(improvedConditions) << std::endl;

  Result result = test_optimized(improvedConditions);
  std::cout << result.winPercentage << std::endl;

  return 0;
}