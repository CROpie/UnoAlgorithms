#include "player.h"

// Control Player (p2)
// Condition: DEFAULT

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
    p2NumberFirstAction
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
        case ConditionName::PLAYER_HOLDS_N_CARDS: return "player holds N cards";
        case ConditionName::OPPONENT_HOLDS_N_CARDS: return "opponent holds N cards";
        case ConditionName::OPPONENT_PLAYED_N_COLOUR_IN_A_ROW: return "opponent played N in a row";
        case ConditionName::OPPONENT_HAD_TO_PICK_UP: return "opponent had to pick up";
        case ConditionName::DEFAULT: return "default";
        default: return "unknown";
    };
}