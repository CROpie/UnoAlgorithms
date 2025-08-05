#include "card.h"
#include <vector>
#include <string>

#pragma once

enum class Strategy {
    back,
    random,
    colour,
    number
};

class Player {
    public:
        std::string name;
        int playerNumber;
        Strategy strategy;
        std::vector<Card> hand;

        Player(std::string name, int playerNumber, Strategy strategy);

        void printName();
        void printHand();
        void printCards(std::vector<Card> cards);
        void printDeckSize();

        std::vector<Card> filterValidHand(Card topOfDiscard);
        std::vector<Card> filterForSameColour(std::vector<Card> filteredHand, Card topOfDiscard);
        std::vector<Card> filterForSameValue(std::vector<Card> filteredHand, Card topOfDiscard);
};