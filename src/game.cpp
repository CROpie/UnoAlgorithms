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

void Game::randomStartingPlayer(int playerCount) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, playerCount - 1);
    turn = dist(gen);
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
    if (deck.discard_pile.back().colour == Colour::black) wildChoice = Colour::red;

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

    if (topOfDiscard.colour == Colour::black) {
        for(const auto& card : hand) {
            if (card.colour == wildChoice) filtered.emplace_back(card);
            if (card.colour == Colour::black) filtered.emplace_back(card);
        }

        return filtered;
    }

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

// CHOOSING A COLOUR TO PLAY FOR WILD CARD
Colour Game::mostCommonColour(std::vector<Card>& hand) {
    std::unordered_map<Colour, int> colourCount;

    for (auto& card : hand) {
        colourCount[card.colour]++;
    }

    Colour maxColour = hand[0].colour;
    int maxCount = 0;
    for (auto& [colour, count] : colourCount) {
        if (count > maxCount) {
            maxCount = count;
            maxColour = colour;
        }
    }
    return maxColour;
}

// MAIN PROCESS

bool Game::doesPlayerLoseTurn(Player& player, Value topOfDiscardValue) {
    if (!justPlayed) return false;
    justPlayed = false;
    int cardsToDraw = 0;
    int loseTurn = false;
    switch (topOfDiscardValue) {
        case Value::skip:
            debug && std::cout << player.name + " was skipped" << std::endl;
            loseTurn = true;
            break;
        case Value::reverse:
            debug && std::cout << player.name + " was reversed" << std::endl;
            loseTurn = true;
            break;
        case Value::drawtwo:
            debug && std::cout << player.name + " has to draw 2" << std::endl;
            loseTurn = true;
            cardsToDraw = 2;
            break;
        case Value::drawfour:
            debug && std::cout << player.name + " has to draw 4" << std::endl;
            loseTurn = true;
            cardsToDraw = 4;
            break;
    }
    
    for (int i = 0; i < cardsToDraw; i++) {
        if (deck.draw_pile.empty()) deck.reshuffle();
        moveTopCard(deck.draw_pile, player.hand);
    }
    return loseTurn;
}

void Game::play() {
    if (turn >= players.size()) throw std::runtime_error("Error: invalid turn index in Game::play()");
    if (deck.discard_pile.empty()) throw std::runtime_error("Error: discard pile is empty in Game::play()");
    
    Player& current = players[turn];

    Card& topOfDiscard = deck.discard_pile.back();

    debug && std::cout << current.name + "'s turn" << std::endl;

    // Player will not get to play a card
    if (doesPlayerLoseTurn(current, topOfDiscard.value)) return;

    // Any playable card including wild cards
    std::vector<Card> filteredHand = filterValidHand(current.hand, topOfDiscard);

    if (filteredHand.size() == 0) {
        debug && std::cout << current.name + " has to pick up a card" << std::endl;
        if (deck.draw_pile.empty()) deck.reshuffle();
        moveTopCard(deck.draw_pile, current.hand);
        justPlayed = false;
        return;
    }


    Card validCard;

    // Return wild card as last resort
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
    if (debug) validCard.printCard();

    // choose what colour to call wild
    if (validCard.colour == Colour::black) {
        // switch strategy: most of that colour vs least of that colour vs random of present colours?

        // Most of that colour
        wildChoice = mostCommonColour(current.hand);
        debug && std::cout << current.name + " has chosen " + Card::toColString(wildChoice) << std::endl;
    }


    justPlayed = true;
}

void Game::advanceTurn() {
    turn = (turn + 1) % players.size();
}

void Game::awardWin() {
    players[turn].wins += 1;
}

bool Game::hasPlayerWon() {
    if (players[turn].hand.size() == 0) {
        debug && std::cout << players[turn].name + " has won the game!" << std::endl;
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
    randomStartingPlayer(players.size());
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