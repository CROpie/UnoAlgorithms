#include "card.h"
#include "player.h"
#include "deck.h"
#include <vector>
#include <algorithm>
#include <random>

#pragma once

class Game {
    public:
        Deck deck;
        std::vector<Player> players;
        int turn;

        Game();

        void shuffleDeck();

        void addPlayer(Player&& player);
        void dealToPlayers(int cardCount);

        void printTurn();
        void printDiscard();

        bool hasPlayerWon();

        void play();

        void advanceTurn();
};