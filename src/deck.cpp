#include "deck.h"

std::vector<Card> Deck::generateAllCards() {
    std::vector<Card> cards;
    for (auto colour : all_colours) {
        for (auto value : all_values) {
            if (value == Value::zero) {
                cards.emplace_back(colour, value);
            } else {
                cards.emplace_back(colour, value);
                // cards.emplace_back(colour, value);
            }

        }
    }
    return cards;
}

Deck::Deck()
    : draw_pile(generateAllCards()), discard_pile() {}

void Deck::printDiscard() {
    discard_pile.back().printCard();
}

void Deck::shuffle() {
    std::cout << "--Shuffling the cards--" << std::endl;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(draw_pile.begin(), draw_pile.end(), g);
}

void Deck::reshuffle() {
    if (discard_pile.size() <= 1) throw std::runtime_error("Error: no discarded cards in Deck::reshuffle()");

    draw_pile.insert(
        draw_pile.end(),
        std::make_move_iterator(discard_pile.begin()),
        std::make_move_iterator(discard_pile.end() - 1)
    );
    discard_pile.erase(discard_pile.begin(), discard_pile.end() - 1);
    
    shuffle();
}