#include "card.h"
#include "player.h"
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

        void deal(std::vector<Player>& players, int cardCount);

        // draw -> discard
        void flipCard();
        void printDiscard();

        void haveCardDrawn(Player& player);

        void playCard(Card&& cardToPlay);


        Deck();
};