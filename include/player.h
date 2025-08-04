#include "card.h"
#include <vector>
#include <string>

#pragma once

class Player {
    public:
        std::string name;
        std::vector<Card> hand;
        std::vector<Card> filteredHand;

        Player(std::string name);

        void printName();
        void printCards();
        void printDeckSize();
        void printValidCards();

        void filterValidHand(Card topOfDiscard);
};