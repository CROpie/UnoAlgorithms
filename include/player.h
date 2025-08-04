#include "card.h"
#include <vector>
#include <string>

#pragma once

class Player {
    public:
        std::string name;
        std::vector<Card> hand;

        Player(std::string name);

        void printName();
        void printHand();
        void printCards(std::vector<Card> cards);
        void printDeckSize();

        std::vector<Card> filterValidHand(Card topOfDiscard);
        void removeCard(Card playedCard);
};