#include "game.h"

Game::Game()
    : deck() {}

// Game Set-Up
void Game::addPlayer(Player& player) {
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

std::vector<Card> Game::filterForSameColourAndValue(std::vector<Card>& hand, Card& topOfDiscard) {
    std::vector<Card> filtered;
    for(auto card : hand) {
        if (card.cmpColour(topOfDiscard) && card.cmpValue(topOfDiscard)) filtered.emplace_back(card);
    }

    return filtered;
}


std::vector<Card> Game::filterForSameColourOrValue(std::vector<Card>& hand, Card& topOfDiscard) {
    std::vector<Card> filtered;
    for(auto card : hand) {
        if (card.cmpColour(topOfDiscard) || card.cmpValue(topOfDiscard)) filtered.emplace_back(card);
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

std::vector<Card> Game::filterForSameColourPlain(std::vector<Card>& hand, Card& topOfDiscard) {
    std::vector<Card> filtered;
    for(auto card : hand) {
        if (card.colour == Colour::black) continue;
        if (card.value == Value::drawtwo) continue;
        if (card.value == Value::skip) continue;
        if (card.value == Value::reverse) continue;
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
    Player& next = players[(turn + 1) % players.size()];

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
        current.hadToPickUp = true;
        return;
    }

    // perhaps could update this to use compound conditions somehow (??)
    std::vector<Condition> validConditions = validateConditions(current, next);

    Condition selectedCondition = validConditions.empty() ? p2ConditionList[0] : validConditions.front();
    current.selectedCondition = selectedCondition;

    // for debug only (?)
    std::vector<PlayAction> validActions = validateActions(filteredHand, topOfDiscard, selectedCondition.actionPriorityList);

    current.selectedAction = validActions.front();

    // Card validCard = chooseCardFromActionPriority(filteredHand, topOfDiscard, priorityCondition.actionPriorityList);
    Card validCard = chooseCardFromValidAction(filteredHand, topOfDiscard, current.selectedAction.value());

    moveCard(current.hand, deck.discard_pile, validCard);
    current.playedColours.emplace_back(validCard.colour);
    current.hadToPickUp = false;

    if (debug) validCard.printCard();

    // choose what colour to call wild
    if (validCard.colour == Colour::black) {
        // switch strategy: most of that colour vs least of that colour vs random of present colours?

        // Most of that colour
        wildChoice = mostCommonColour(current.hand);
        debug && std::cout << current.name + " has chosen " + Card::toColString(wildChoice) << std::endl;
    }

    // Penalty for not finishing with plain card
    if (current.hand.empty() && !validCard.isPlain()) {
        debug && std::cout << current.name + " was penalized for not finishing with a number" << std::endl;
        if (deck.draw_pile.empty()) deck.reshuffle();
        moveTopCard(deck.draw_pile, current.hand);
        justPlayed = true;
        current.hadToPickUp = true;
        return;  
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

void Game::logData(const std::string& filename, int elapsedTime) {
    std::ofstream logFile(filename, std::ios::app);

    if (!logFile.is_open()) throw std::runtime_error("Error: unable to open log file in Game::logWins()");
    logFile << "Game completed in " << elapsedTime << " microseconds. Required wins: " << REQ_WINS << "\n";

    for (auto it = players.begin(); it != players.end(); ++it) {
        logFile << it->wins;
        if (it != players.end() - 1) {
            logFile << " VS ";
        } else {
            logFile << "\n";
        }
    }

    for (auto& player : players) {
        // logFile << "Player: " << player.strategy.name << ", Wins: " << player.wins << "\n";
        logFile << "Player: " << player.name << "\n" << player.strategy.conditionListToString(player.strategy.conditions) << "\n";

    }
    logFile << "----------------------\n";

    logFile.close();
}

void Game::logWins(const std::string& filename) {
    std::ofstream logFile(filename, std::ios::app);

    for (auto it = players.begin(); it != players.end(); ++it) {
        logFile << it->wins;
        if (it != players.end() - 1) {
            logFile << " VS ";
        } else {
            logFile << "\n";
        }
    }

    logFile.close();
}

void Game::finishAndRestart() {
    justPlayed = false;
    resetCards();
    deck.shuffle();
    dealStartingCards(HAND_SIZE);
    randomStartingPlayer(players.size());
    for (auto& player : players) {
        player.playedColours.clear();
    }
}

// Card Choice Algorithms

// random non-wild card
bool Game::getSameColourOrValue(std::vector<Card>& filteredHand, Card& topOfDiscard, Card& cardToPlay) {
    std::vector<Card> subset = filterForSameColourOrValue(filteredHand, topOfDiscard);

    if (subset.empty()) return false;
    cardToPlay = subset.back();
    return true;
}

// random same colour card
bool Game::getSameColourCard(std::vector<Card>& filteredHand, Card& topOfDiscard, Card& cardToPlay) {
    std::vector<Card> subset = filterForSameColour(filteredHand, topOfDiscard);

    if (subset.empty()) return false;
    cardToPlay = subset.back();
    return true;
}

// random same colour card (no special cards allowed)
bool Game::getSameColourCardPlain(std::vector<Card>& filteredHand, Card& topOfDiscard, Card& cardToPlay) {
    std::vector<Card> subset = filterForSameColourPlain(filteredHand, topOfDiscard);

    if (subset.empty()) return false;
    cardToPlay = subset.back();
    return true;
}

// random same value card
bool Game::getSameValueCard(std::vector<Card>& filteredHand, Card& topOfDiscard, Card& cardToPlay) {
    std::vector<Card> subset = filterForSameValue(filteredHand, topOfDiscard);

    if (subset.empty()) return false;
    cardToPlay = subset.back();
    return true;
}

// Had to use either Card instead of Card& because "initial value of reference to non-const must be an lvalue"
bool Game::getSpecificCard(std::vector<Card>& filteredHand, Card specificCard, Card& cardToPlay) {
    std::vector<Card> subset = filterForSameColourAndValue(filteredHand, specificCard);

    if (subset.empty()) return false;
    cardToPlay = subset.back();
    return true;  
}

// random same value card
void Game::getRandomCard(std::vector<Card>& filteredHand, Card& cardToPlay) {

    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(filteredHand.begin(), filteredHand.end(), rng);

    cardToPlay = filteredHand.back();
}


/* DEBUG */

// Version for just checking, not returning a card
bool Game::getSameColourCardPlain(std::vector<Card>& filteredHand, Card& topOfDiscard) {
    std::vector<Card> subset = filterForSameColourPlain(filteredHand, topOfDiscard);

    if (subset.size() == 0) return false;
    return true;
}

bool Game::getSameValueCard(std::vector<Card>& filteredHand, Card& topOfDiscard) {
    std::vector<Card> subset = filterForSameValue(filteredHand, topOfDiscard);

    if (subset.size() == 0) return false;
    return true;
}

bool Game::getSpecificCard(std::vector<Card>& filteredHand, Card specificCard) {
    std::vector<Card> subset = filterForSameColourAndValue(filteredHand, specificCard);

    if (subset.size() == 0) return false;
    return true;  
}

// Figure out which conditions are valid
std::vector<Condition> Game::validateConditions(Player& current, Player& opponent) {
    std::vector<Condition> validConditions;

    for (auto& cond : current.strategy.conditions) {
        switch (cond.name) {
            case ConditionName::PLAYER_HOLDS_N_CARDS:
                if (current.hand.size() <= cond.modifier) validConditions.emplace_back(cond);
                break;
            case ConditionName::OPPONENT_HOLDS_N_CARDS:
                if (opponent.hand.size() <= cond.modifier) validConditions.emplace_back(cond);
                break;
            case ConditionName::OPPONENT_HAD_TO_PICK_UP:
                if (opponent.hadToPickUp) validConditions.emplace_back(cond);
                break;
            case ConditionName::OPPONENT_PLAYED_N_COLOUR_IN_A_ROW:
            {

                if (opponent.playedColours.empty()) break;

                int last = opponent.playedColours.size() - 1;

                bool shouldExit = false;
                for (int i = 0; i < cond.modifier; i++) {
                    if (last - i < 0) {
                        shouldExit = true;
                        break;
                    }
                    if (opponent.playedColours[last] != opponent.playedColours[last - i]) {
                        shouldExit = true;
                        break;
                    }
                }

                if (shouldExit) break;
                
                validConditions.emplace_back(cond);
                break;
            }
            case ConditionName::DEFAULT:
                validConditions.emplace_back(cond);
                break;
            default:
                validConditions.emplace_back(cond);
                break;
        }
    }

    return validConditions;
}

// Only for debug chooseCardFromActionPriority makes more sense..
std::vector<PlayAction> Game::validateActions(std::vector<Card>& filteredHand, Card& topOfDiscard, std::vector<PlayAction>& actions) {
    std::vector<PlayAction> validActions;
    for (auto& action : actions) {
        switch (action) {
            case PlayAction::DRAW_FOUR:
                if (getSpecificCard(filteredHand, {Colour::black, Value::drawfour})) validActions.emplace_back(PlayAction::DRAW_FOUR);
                break;
            case PlayAction::WILD:
                if (getSpecificCard(filteredHand, {Colour::black, Value::wild})) validActions.emplace_back(PlayAction::WILD);
                break;
            case PlayAction::DRAW_TWO:
                if (getSpecificCard(filteredHand, {topOfDiscard.colour, Value::drawtwo})) validActions.emplace_back(PlayAction::DRAW_TWO);
                break;
            case PlayAction::SKIP:
                if (getSpecificCard(filteredHand, {topOfDiscard.colour, Value::skip}) || 
                    getSpecificCard(filteredHand, {topOfDiscard.colour, Value::reverse})) validActions.emplace_back(PlayAction::SKIP);
                break;
            case PlayAction::FOLLOW_COLOUR_PLAIN:
                if (getSameColourCardPlain(filteredHand, topOfDiscard)) validActions.emplace_back(PlayAction::FOLLOW_COLOUR_PLAIN);
                break;
            case PlayAction::FOLLOW_VALUE:
                if (getSameValueCard(filteredHand, topOfDiscard)) validActions.emplace_back(PlayAction::FOLLOW_VALUE);
                break;
            case PlayAction::RANDOM:
            default:
                validActions.emplace_back(PlayAction::RANDOM);
        }
    }

    return validActions;
}

Card Game::chooseCardFromValidAction(std::vector<Card>& filteredHand, Card& topOfDiscard, PlayAction& action) {
    Card cardToPlay;
        switch (action) {
            case PlayAction::DRAW_FOUR:
                if (getSpecificCard(filteredHand, {Colour::black, Value::drawfour}, cardToPlay)) return cardToPlay;
                break;
            case PlayAction::WILD:
                if (getSpecificCard(filteredHand, {Colour::black, Value::wild}, cardToPlay)) return cardToPlay;
                break;
            case PlayAction::DRAW_TWO:
                if (getSpecificCard(filteredHand, {topOfDiscard.colour, Value::drawtwo}, cardToPlay)) return cardToPlay;
                break;
            case PlayAction::SKIP:
                if (getSpecificCard(filteredHand, {topOfDiscard.colour, Value::skip}, cardToPlay)) return cardToPlay;
                if (getSpecificCard(filteredHand, {topOfDiscard.colour, Value::reverse}, cardToPlay)) return cardToPlay;
                break;
            case PlayAction::FOLLOW_COLOUR_PLAIN:
                if (getSameColourCardPlain(filteredHand, topOfDiscard, cardToPlay)) return cardToPlay;
                break;
            case PlayAction::FOLLOW_VALUE:
                if (getSameValueCard(filteredHand, topOfDiscard, cardToPlay)) return cardToPlay;
                break;
            case PlayAction::RANDOM:
            default:
                 getRandomCard(filteredHand, cardToPlay);
                 return cardToPlay;
        }
    
    // not possible
    return filteredHand.back();
}


// ~ Combination of the above 2 functions
Card Game::chooseCardFromActionPriority(std::vector<Card>& filteredHand, Card& topOfDiscard, std::vector<PlayAction>& actions) {
    Card cardToPlay;
    for (auto& action : actions) {
        switch (action) {
            case PlayAction::DRAW_FOUR:
                if (getSpecificCard(filteredHand, {Colour::black, Value::drawfour}, cardToPlay)) return cardToPlay;
                break;
            case PlayAction::WILD:
                if (getSpecificCard(filteredHand, {Colour::black, Value::wild}, cardToPlay)) return cardToPlay;
                break;
            case PlayAction::DRAW_TWO:
                if (getSpecificCard(filteredHand, {topOfDiscard.colour, Value::drawtwo}, cardToPlay)) return cardToPlay;
                break;
            case PlayAction::SKIP:
                if (getSpecificCard(filteredHand, {topOfDiscard.colour, Value::skip}, cardToPlay)) return cardToPlay;
                if (getSpecificCard(filteredHand, {topOfDiscard.colour, Value::reverse}, cardToPlay)) return cardToPlay;
                break;
            case PlayAction::FOLLOW_COLOUR_PLAIN:
                if (getSameColourCardPlain(filteredHand, topOfDiscard, cardToPlay)) return cardToPlay;
                break;
            case PlayAction::FOLLOW_VALUE:
                if (getSameValueCard(filteredHand, topOfDiscard, cardToPlay)) return cardToPlay;
                break;
            case PlayAction::RANDOM:
            default:
                 getRandomCard(filteredHand, cardToPlay);
                 return cardToPlay;
        }
    }

    // not possible
    return filteredHand.back();
}