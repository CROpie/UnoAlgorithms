#include "card.h"
#include "player.h"
#include "deck.h"
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <iostream>
#include <string>

#pragma once

class Game {
    public:
        Deck deck;
        std::vector<Player> players;
        int turn{0};
        int HAND_SIZE{7};
        int REQ_WINS{5};

        // Constructor
        Game();

        // starting the game
        void shuffleDeck();
        void addPlayer(Player&& player);

        void resetCards();

        // card movement helpers
        void moveTopCard(std::vector<Card>& source, std::vector<Card>& destination);
        void moveCard(std::vector<Card>& source, std::vector<Card>& destination, Card& card);

        // dealing the cards
        void dealStartingCards(int cardCount);
        void dealToPlayers(int cardCount);

        // Card Filtering Cards
        std::vector<Card> filterValidHand(std::vector<Card>& hand, Card& topOfDiscard);
        std::vector<Card> filterForSameColour(std::vector<Card>& hand, Card& topOfDiscard);
        std::vector<Card> filterForSameValue(std::vector<Card>& hand, Card& topOfDiscard);

        // Card Choice Algorithms
        Card getBackCard(std::vector<Card>& filteredHand);
        Card getSameColourCard(std::vector<Card>& filteredHand, Card& topOfDiscard);
        Card getSameValueCard(std::vector<Card>& filteredHand, Card& topOfDiscard);


        // Printing to terminal
        void printTurn();
        void printDiscard();

        // Main process
        void play();
        void advanceTurn();
        void awardWin();
        bool hasPlayerWon();
        bool hasPlayerWonNGames(int requiredWins);
        void logWins(const std::string& filename);
        void finishAndRestart();
};