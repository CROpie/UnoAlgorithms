#include "card.h"
#include "strategy.h"
#include <vector>
#include <string>
#include <optional>

#pragma once

class Player {
    public:
        std::string name;
        int playerNumber;
        Strategy strategy;
        std::vector<Card> hand;
        int wins{0};
        std::vector<Colour> playedColours;
        bool hadToPickUp{false};

        std::optional<Condition> selectedCondition;
        std::optional<PlayAction> selectedAction;

        Player(std::string name, int playerNumber, Strategy strategy);

        void clearHand();

        void printName();
        void printHand();
        void printCards(std::vector<Card> cards);
        void printDeckSize();
};