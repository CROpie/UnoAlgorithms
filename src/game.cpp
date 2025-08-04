#include "game.h"

Game::Game()
    : deck() {}

void Game::addPlayer(Player&& player) {
    players.emplace_back(std::move(player));
}

void Game::shuffleDeck() {
    deck.shuffle();
}

void Game::printTurn() {
    std::cout << players[turn].name + "'s turn" << std::endl;
}

void Game::dealToPlayers(int cardCount) {
    deck.deal(players, cardCount);
    deck.flipCard();
    printTurn();
}

bool Game::hasPlayerWon() {
    if (players[turn].hand.size() == 0) {
        std::cout << players[turn].name + "has won the game!" << std::endl;
        return true;
    }
    return false;
}

void Game::play() {
    Player& current = players[turn];
    Card topOfDiscard = deck.discard_pile.back();
    std::vector<Card> filteredHand = current.filterValidHand(topOfDiscard);

    if (filteredHand.size() == 0) {
        deck.haveCardDrawn(current);
        return;
    }


    Card validCard = filteredHand.back();

    auto it = std::find(current.hand.begin(), current.hand.end(), validCard);
    Card cardToPlay = std::move(*it);
    current.hand.erase(it);
    deck.playCard(std::move(cardToPlay));

    
}

void Game::advanceTurn() {
    turn = (turn + 1) % players.size();
}

void Game::printDiscard() {
    std::cout << "--- top of discard ---" << std::endl;
    deck.discard_pile.back().printCard();
    std::cout << "--- --- --- ---" << std::endl;

}