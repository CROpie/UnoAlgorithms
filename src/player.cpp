#include "player.h"


Player::Player(std::string name, int playerNumber)
    : name(name), playerNumber(playerNumber) {}

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

std::vector<Card> Player::filterValidHand(Card topOfDiscard) {
    std::cout << this->name + "'s valid hand" << std::endl;
    std::vector<Card> filtered;
    for(auto card : hand) {
        if (card.isValid(topOfDiscard)) filtered.emplace_back(card);
    }

    return filtered;
}