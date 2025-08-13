#include <strategy.h>

std::vector<PlayAction> allActions = {
    PlayAction::DRAW_FOUR,
    PlayAction::WILD,
    PlayAction::DRAW_TWO,
    PlayAction::SKIP,
    PlayAction::FOLLOW_COLOUR_PLAIN,
    PlayAction::FOLLOW_VALUE,
    PlayAction::RANDOM
};

Strategy::Strategy(std::vector<Condition> conditions, std::string name) : conditions(conditions), name(name) {}

Strategy::Strategy(std::vector<ConditionName> conditionNames, std::string name) : name(name) {

    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(conditionNames.begin(), conditionNames.end(), rng);

    for (auto& conditionName : conditionNames) {
        Condition condition;
        std::vector<PlayAction> actionList = generateRandomActions();
        condition.name = conditionName;
        condition.modifier = 2;
        condition.actionPriorityList = actionList;
        conditions.emplace_back(condition);
    }
}

Strategy::Strategy(std::vector<Condition> inputConditions, std::vector<ConditionName> conditionNames, std::string name) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(conditionNames.begin(), conditionNames.end(), rng);

    for (auto& conditionName : conditionNames) {
        Condition condition;
        std::vector<PlayAction> actionList = generateRandomActions();
        condition.name = conditionName;

        std::random_device rd2;
        std::mt19937 rng2(rd2());
        std::uniform_int_distribution<> dist(1, 2);

        condition.modifier = dist(rng2);
        condition.actionPriorityList = actionList;
        inputConditions.emplace_back(condition);
    }

    std::random_device rd3;
    std::mt19937 rng3(rd3());
    std::shuffle(inputConditions.begin(), inputConditions.end(), rng3);
    conditions = inputConditions;

}

std::vector<PlayAction> Strategy::generateRandomActions() {

    std::random_device rd;
    std::mt19937 rng(rd());

    std::vector<PlayAction> actionList = allActions;
    std::shuffle(actionList.begin(), actionList.end(), rng);
    return actionList;
}

std::string Strategy::actionListToString(std::vector<PlayAction> actions) {
    std::ostringstream actionListString;
    int i = 0;
    for (auto& action : actions) {
        actionListString << "  " << i++ << ": " << toPlayActionString(action) << "\n";
    }
    return actionListString.str();
}

std::string Strategy::conditionListToString(std::vector<Condition> conditions) {
    std::ostringstream conditionListString;
    int i = 0;
    for (auto& condition : conditions) {
        conditionListString << i++ << ": " << toConditionNameString(condition) << "\n";
        conditionListString << "  Modifier: " << condition.modifier << "\n";
        conditionListString << actionListToString(condition.actionPriorityList);

    }
    return conditionListString.str();
}

void Strategy::printStrategy() {
    std::cout << conditionListToString(conditions) << std::endl;
}

std::string Strategy::toPlayActionString(PlayAction action) {
    switch (action) {
        case PlayAction::DRAW_FOUR: return "draw four";
        case PlayAction::WILD: return "wild";
        case PlayAction::DRAW_TWO: return "draw two";
        case PlayAction::SKIP: return "skip";
        case PlayAction::FOLLOW_COLOUR_PLAIN: return "follow colour plain";
        case PlayAction::FOLLOW_VALUE: return "follow value";
        case PlayAction::RANDOM: return "random";
        default: return "unknown";
    };
}

std::string Strategy::toConditionNameString(Condition condition) {
    switch (condition.name) {
        case ConditionName::PLAYER_HOLDS_N_CARDS: return "player holds N cards";
        case ConditionName::OPPONENT_HOLDS_N_CARDS: return "opponent holds N cards";
        case ConditionName::OPPONENT_PLAYED_N_COLOUR_IN_A_ROW: return "opponent played N in a row";
        case ConditionName::OPPONENT_HAD_TO_PICK_UP: return "opponent had to pick up";
        case ConditionName::DEFAULT: return "default";
        default: return "unknown";
    };
}




/* PRE-BUILT STRATEGIES*/

// Aggressive
std::vector<PlayAction> p2AggressiveAction = {
    PlayAction::DRAW_FOUR,
    PlayAction::DRAW_TWO,
    PlayAction::WILD,
    PlayAction::SKIP,
    PlayAction::FOLLOW_COLOUR_PLAIN,
    PlayAction::FOLLOW_VALUE,
    PlayAction::RANDOM
};

// Number first
std::vector<PlayAction> p2NumberFirstAction = {
    PlayAction::FOLLOW_VALUE,
    PlayAction::FOLLOW_COLOUR_PLAIN,
    PlayAction::SKIP,
    PlayAction::DRAW_TWO,
    PlayAction::WILD,
    PlayAction::DRAW_FOUR,
    PlayAction::RANDOM
};

// Random
std::vector<PlayAction> p2DefaultAction = {
    PlayAction::RANDOM
};

Condition p2DefaultCondition = {
    ConditionName::DEFAULT,
    1,
    p2DefaultAction
};

// Condition priority lists
std::vector<Condition> p2ConditionList = {
    p2DefaultCondition
};

// Player (p1)
// Condition: OPPONENT_HOLDS_N_CARDS
std::vector<PlayAction> p1OpponentHasNAction = {
    PlayAction::DRAW_FOUR,
    PlayAction::WILD,
    PlayAction::DRAW_TWO,
    PlayAction::SKIP,
    PlayAction::FOLLOW_COLOUR_PLAIN,
    PlayAction::FOLLOW_VALUE,
    PlayAction::RANDOM
};

Condition p1OpponentHasNCondition = {
    ConditionName::OPPONENT_HOLDS_N_CARDS,
    2,
    p1OpponentHasNAction
};

// Condition: OPPONENT_HAD_TO_PICK_UP
std::vector<PlayAction> p1OpponentHadToPickUpAction = {
    PlayAction::FOLLOW_COLOUR_PLAIN,
    PlayAction::SKIP,
    PlayAction::DRAW_TWO,
    PlayAction::FOLLOW_VALUE,
    PlayAction::WILD,
    PlayAction::DRAW_FOUR,
    PlayAction::RANDOM
};

Condition p1OpponentHadToPickUpCondition = {
    ConditionName::OPPONENT_HAD_TO_PICK_UP,
    2,
    p1OpponentHadToPickUpAction
};

// Condition: OPPONENT_PLAYED_N_COLOUR_IN_A_ROW
std::vector<PlayAction> p1OpponentPlayedNColourAction = {
    PlayAction::FOLLOW_COLOUR_PLAIN,
    PlayAction::WILD,
    PlayAction::FOLLOW_VALUE,
    PlayAction::DRAW_TWO,
    PlayAction::SKIP,
    PlayAction::DRAW_FOUR,
    PlayAction::RANDOM
};

Condition p1OpponentPlayedNColourCondition = {
    ConditionName::OPPONENT_PLAYED_N_COLOUR_IN_A_ROW,
    2,
    p1OpponentPlayedNColourAction
};

// Condition: PLAYER_HOLDS_N_CARDS
std::vector<PlayAction> p1PlayerHasNCardsAction = {
    PlayAction::WILD,
    PlayAction::SKIP,
    PlayAction::DRAW_TWO,
    PlayAction::DRAW_FOUR,
    PlayAction::FOLLOW_COLOUR_PLAIN,
    PlayAction::FOLLOW_VALUE,
    PlayAction::RANDOM
};

Condition p1PlayerHasNCardsCondition = {
    ConditionName::PLAYER_HOLDS_N_CARDS,
    2,
    p1PlayerHasNCardsAction
};


// Condition: DEFAULT
std::vector<PlayAction> p1DefaultAction = {
    PlayAction::FOLLOW_VALUE,
    PlayAction::SKIP,
    PlayAction::DRAW_TWO,
    PlayAction::WILD,
    PlayAction::FOLLOW_COLOUR_PLAIN,
    PlayAction::DRAW_FOUR,
    PlayAction::RANDOM
};

Condition p1DefaultCondition = {
    ConditionName::DEFAULT,
    1,
    p1DefaultAction
};

// Condition priority lists
std::vector<Condition> p1ConditionList = {
    p1PlayerHasNCardsCondition,
    p1OpponentHasNCondition,
    p1OpponentHadToPickUpCondition,
    p1OpponentPlayedNColourCondition,
    p1DefaultCondition
};