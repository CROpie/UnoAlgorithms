#include <iostream>
#include <vector>
#include <string>
#include "card.h"
#include "deck.h"

int main() {
  Deck deck;
  deck.shuffle();
  std::vector<Player> players;

  players.emplace_back("chris");
  players.emplace_back("leah");

  deck.deal(players, 3);
  deck.flipCard();

  players[0].printName();
  players[0].printCards();
  players[0].filterValidHand(deck.discard_pile.back());

  players[1].printName();
  players[1].printCards();
  players[1].filterValidHand(deck.discard_pile.back());

  return 0;
}