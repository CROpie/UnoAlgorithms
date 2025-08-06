#include "game.h"

Game::Game()
    : deck() {}

// Game Set-Up
void Game::addPlayer(Player&& player) {
    players.emplace_back(std::move(player));
}

void Game::shuffleDeck() {
    deck.shuffle();
}

void Game::resetCards() {
    for (auto& player : players) {
        player.clearHand();
    }

    deck.discard_pile.clear();
    deck.draw_pile.clear();
    deck.draw_pile = deck.generateAllCards();
}

// Printing

void Game::printDiscard() {
    std::cout << "--- top of discard ---" << std::endl;
    deck.discard_pile.back().printCard();
    std::cout << "--- --- --- ---" << std::endl;
}

void Game::printTurn() {
    std::cout << players[turn].name + "'s turn" << std::endl;
}

// Helpers for card movement incl. defensive programming
void Game::moveTopCard(std::vector<Card>& source, std::vector<Card>& destination) {
    if (source.empty()) throw std::runtime_error("Error: No cards in source to move in Game::moveTopCard()");
    destination.emplace_back(std::move(source.back()));
    source.pop_back();
}

void Game::moveCard(std::vector<Card>& source, std::vector<Card>& destination, Card& card) {
    auto it = std::find(source.begin(), source.end(), card);
    if (it == source.end()) throw std::runtime_error("Error: card not found in source in Game::moveCard()");
    Card foundCard = std::move(*it);
    source.erase(it);
    destination.emplace_back(std::move(foundCard));
}

// Card Movement Between Deck and Players

void Game::dealToPlayers(int cardCount) {
    for (auto& player : players) {
        for (int i = 0; i < cardCount; ++i) {
            moveTopCard(deck.draw_pile, player.hand);
        }
    }
}

void Game::dealStartingCards(int cardCount = 7) {
    dealToPlayers(cardCount);
    moveTopCard(deck.draw_pile, deck.discard_pile);
}

// FILTERING CARDS
/*
    emplace_back(card) constructs a _new_ card inside the filtered vector using a copy of card
    (despite looping over references to cards in hand)
    therefore changes to filtered[i] don't affect hand[i]

    push_back(&card) can be used to add pointers to card (ie, not copies) to std::vector<Card*>
*/
std::vector<Card> Game::filterValidHand(std::vector<Card>& hand, Card& topOfDiscard) {

    std::vector<Card> filtered;
    for(const auto& card : hand) {
        if (card.isValid(topOfDiscard)) filtered.emplace_back(card);
    }

    return filtered;
}

std::vector<Card> Game::filterForSameColour(std::vector<Card>& hand, Card& topOfDiscard) {
    std::vector<Card> filtered;
    for(auto card : hand) {
        if (card.cmpColour(topOfDiscard)) filtered.emplace_back(card);
    }

    return filtered;
}

std::vector<Card> Game::filterForSameValue(std::vector<Card>& hand, Card& topOfDiscard) {
    std::vector<Card> filtered;
    for(auto card : hand) {
        if (card.cmpValue(topOfDiscard)) filtered.emplace_back(card);
    }

    return filtered;
}

// MAIN PROCESS

void Game::play() {
    if (turn >= players.size()) throw std::runtime_error("Error: invalid turn index in Game::play()");
    if (deck.discard_pile.empty()) throw std::runtime_error("Error: discard pile is empty in Game::play()");
    
    Player& current = players[turn];

    Card& topOfDiscard = deck.discard_pile.back();

    if (justPlayed && topOfDiscard.value == Value::skip) return;

    if (justPlayed && topOfDiscard.value == Value::reverse) return;

    if (justPlayed && topOfDiscard.value == Value::drawtwo) {
        if (deck.draw_pile.empty()) deck.reshuffle();
        moveTopCard(deck.draw_pile, current.hand);
        if (deck.draw_pile.empty()) deck.reshuffle();
        moveTopCard(deck.draw_pile, current.hand);
        justPlayed = false;
        return;
    }

    std::vector<Card> filteredHand = filterValidHand(current.hand, topOfDiscard);

    if (filteredHand.size() == 0) {
        if (deck.draw_pile.empty()) deck.reshuffle();
        moveTopCard(deck.draw_pile, current.hand);
        justPlayed = false;
        return;
    }

    Card validCard;

    switch (current.strategy) {
        case Strategy::back:
            validCard = getBackCard(filteredHand);
            break;
        case Strategy::colour:
            validCard = getSameColourCard(filteredHand, topOfDiscard);
            break;
        case Strategy::number:
            validCard = getSameValueCard(filteredHand, topOfDiscard);
            break;
    }

    moveCard(current.hand, deck.discard_pile, validCard);
    justPlayed = true;
}

void Game::advanceTurn() {
    if (onePause) {
        justPlayed = false;
        onePause = false;
    }

    if (justPlayed) {
        onePause = true;
    }
    turn = (turn + 1) % players.size();
}

void Game::awardWin() {
    players[turn].wins += 1;
}

bool Game::hasPlayerWon() {
    if (players[turn].hand.size() == 0) {
        // std::cout << players[turn].name + " has won the game!" << std::endl;
        return true;
    }
    return false;
}

bool Game::hasPlayerWonNGames(int requiredWins) {
    for (auto& player : players) {
        if (player.wins == requiredWins) return true;
    }
    return false;
}

void Game::logWins(const std::string& filename, int elapsedTime) {
    std::ofstream logFile(filename, std::ios::app);

    if (!logFile.is_open()) throw std::runtime_error("Error: unable to open log file in Game::logWins()");
    logFile << " Game completed in " << elapsedTime << " microseconds. Required wins: " << REQ_WINS << "\n";
    for (const auto& player : players) {
        logFile << "Player: " << strategyToString(player.strategy) << ", Wins: " << player.wins << "\n";
    }
    logFile << "----------------------\n";

    logFile.close();
}

void Game::finishAndRestart() {
    justPlayed = false;
    onePause = false;
    resetCards();
    deck.shuffle();
    dealStartingCards(HAND_SIZE);
}

// Card Choice Algorithms
Card Game::getBackCard(std::vector<Card>& filteredHand) {
    return filteredHand.back();
}

Card Game::getSameColourCard(std::vector<Card>& filteredHand, Card& topOfDiscard) {
    std::vector<Card> subset = filterForSameColour(filteredHand, topOfDiscard);

    if (subset.size() == 0) return filteredHand.back();
    return subset.back();
}

Card Game::getSameValueCard(std::vector<Card>& filteredHand, Card& topOfDiscard) {
    std::vector<Card> subset = filterForSameValue(filteredHand, topOfDiscard);

    if (subset.size() == 0) return filteredHand.back();
    return subset.back();
}