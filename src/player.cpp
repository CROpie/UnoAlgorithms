#include "player.h"


Player::Player(std::string name)
    : name(name) {}

void Player::printName() {
    std::cout << this->name << std::endl;
}

void Player::printCards() {
    std::cout << this->name + "'s hand" << std::endl;
    for(auto card : hand) {
        card.printCard();
    }
}

void Player::printValidCards() {
    std::cout << this->name + "'s valid hand" << std::endl;
    for(auto card : filteredHand) {
        card.printCard();
    }
}

void Player::printDeckSize() {
    std::cout << this->hand.size() << std::endl;
}

void Player::filterValidHand(Card topOfDiscard) {
    std::cout << this->name + "'s valid hand" << std::endl;
    std::vector<Card> filtered;
    for(auto card : hand) {
        if (card.isValid(topOfDiscard)) filtered.emplace_back(card);
    }

    this->filteredHand = filtered;
}