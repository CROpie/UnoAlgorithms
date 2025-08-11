#include "player.h"

// Control Player (p2)
// Condition: DEFAULT
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

// Condition: DEFAULT
std::vector<PlayAction> p1DefaultAction = {
    PlayAction::FOLLOW_COLOUR_PLAIN,
    PlayAction::SKIP,
    PlayAction::DRAW_TWO,
    PlayAction::FOLLOW_VALUE,
    PlayAction::WILD,
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
    p1OpponentHasNCondition,
    p1DefaultCondition
};

Player::Player(std::string name, int playerNumber, std::vector<Condition> strategy, std::string strategyName)
    : name(name), playerNumber(playerNumber), strategy(strategy), strategyName(strategyName) {}

void Player::clearHand() {
    hand.clear();
}

void Player::printName() {
    std::cout << this->name << std::endl;
}

void Player::printHand() {
    std::cout << this->name + "'s hand" << std::endl;
    for(auto card : hand) {
        card.printCard();
    }
}

void Player::printCards(std::vector<Card> cards) {
    std::cout << this->name + "'s hand" << std::endl;
    for(auto card : cards) {
        card.printCard();
    }
}


void Player::printDeckSize() {
    std::cout << this->hand.size() << std::endl;
}

std::string Player::toPlayActionString(PlayAction action) {
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

std::string Player::toConditionNameString(Condition condition) {
    switch (condition.name) {
        case ConditionName::OPPONENT_HOLDS_N_CARDS: return "opponent holds n cards";
        case ConditionName::DEFAULT: return "default";
        default: return "unknown";
    };
}