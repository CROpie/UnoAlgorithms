#include "player.h"


Player::Player(std::string name, int playerNumber, Strategy strategy)
    : name(name), playerNumber(playerNumber), strategy(strategy) {}

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

