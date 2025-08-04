#include "card.h"
#include <vector>
#include <algorithm>
#include <random>

#pragma once

class Deck {
    public:
        std::vector<Card> draw_pile;
        std::vector<Card> discard_pile;

        std::vector<Card> generateAllCards();
        void shuffle();
        void addBack();

        // draw -> discard
        void flipCard();
        void printDiscard();



        Deck();
};