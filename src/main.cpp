#include <iostream>
#include <vector>
#include <string>
#include "card.h"
#include "deck.h"
#include "game.h"

int main() {
  Game game;

  game.shuffleDeck();

  game.addPlayer(Player("chris"));
  game.addPlayer(Player("leah"));

  game.dealToPlayers(3);

  // while (true) {
  for (int i = 0; i < 20; i++) {
    game.printDiscard();
    game.play();
    game.printDiscard();

    if (game.hasPlayerWon()) break;

    game.advanceTurn();
  }

  return 0;
}