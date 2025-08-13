#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>

#pragma once

// What about drawing a card when it is not needed? i.e. to preserve drawfour
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
    OPPONENT_PLAYED_N_COLOUR_IN_A_ROW,
    OPPONENT_HAD_TO_PICK_UP,
    PLAYER_HOLDS_N_CARDS,
    DEFAULT
};

struct Condition {
    ConditionName name;
    int modifier;
    std::vector<PlayAction> actionPriorityList;
};

extern std::vector<Condition> p2ConditionList;

struct Result {
    std::vector<Condition> condition;
    float winPercentage;
};

class Strategy {
    public:
        std::vector<Condition> conditions;
        std::string name;

        // input a list of conditions, using the order as priority
        Strategy(std::vector<Condition> conditions, std::string name);

        // generate a condition for each conditionName with random action list
        // the list of conditions will be randomized
        Strategy(std::vector<ConditionName> conditionNames, std::string name);

        // generate a strategy from existing conditions + a condition with random action list for each conditionName entered
        // the order of conditions will be randomized
        Strategy(std::vector<Condition> conditions, std::vector<ConditionName> conditionNames, std::string name);

        std::vector<PlayAction> generateRandomActions();

        static std::string actionListToString(std::vector<PlayAction> actions);
        static std::string conditionListToString(std::vector<Condition> conditions);
        void printStrategy();

        static std::string toConditionNameString(Condition condition);
        static std::string toPlayActionString(PlayAction action);
};