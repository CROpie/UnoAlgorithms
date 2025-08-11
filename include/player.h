#include "card.h"
#include <vector>
#include <string>

#pragma once

enum class PlayAction {
    DRAW_FOUR,
    WILD,
    DRAW_TWO,
    SKIP,
    FOLLOW_COLOUR_PLAIN,
    FOLLOW_VALUE,
    RANDOM
};

enum class ConditionName {
    OPPONENT_HOLDS_N_CARDS,
    DEFAULT
};

struct Condition {
    ConditionName name;
    int modifier;
    std::vector<PlayAction> actionPriorityList;
};

extern std::vector<Condition> p1ConditionList;
extern std::vector<Condition> p2ConditionList;

// std::string strategyToString(Strategy strategy);

class Player {
    public:
        std::string name;
        int playerNumber;
        std::vector<Condition> strategy;
        std::string strategyName;
        std::vector<Card> hand;
        int wins{0};

        Player(std::string name, int playerNumber, std::vector<Condition> strategy, std::string strategyName);

        void clearHand();

        void printName();
        void printHand();
        void printCards(std::vector<Card> cards);
        void printDeckSize();


};