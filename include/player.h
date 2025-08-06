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

std::string strategyToString(Strategy strategy);

class Player {
    public:
        std::string name;
        int playerNumber;
        Strategy strategy;
        std::vector<Card> hand;
        int wins{0};

        Player(std::string name, int playerNumber, Strategy strategy);

        void clearHand();

        void printName();
        void printHand();
        void printCards(std::vector<Card> cards);
        void printDeckSize();


};