#include "deck.h"

std::vector<Card> Deck::generateAllCards() {
    std::vector<Card> cards;
    for (auto colour : all_colours) {
        for (auto value : all_values) {
            cards.emplace_back(colour, value);
        }
    }
    return cards;
}

Deck::Deck()
    : draw_pile(generateAllCards()), discard_pile() {}

void Deck::flipCard() {
    if (draw_pile.empty()) {
        this->addBack();
    }
    Card card = std::move(draw_pile.back());
    draw_pile.pop_back();
    discard_pile.emplace_back(std::move(card));
}

void Deck::printDiscard() {
    discard_pile.back().printCard();
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(draw_pile.begin(), draw_pile.end(), g);
}

void Deck::addBack() {
    draw_pile.insert(
        draw_pile.end(),
        std::make_move_iterator(discard_pile.begin()),
        std::make_move_iterator(discard_pile.end())
    );
    discard_pile.clear();
    this->shuffle();
}