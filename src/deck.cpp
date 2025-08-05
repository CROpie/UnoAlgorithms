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
    std::cout << "--Flipping over a card--" << std::endl;
    if (draw_pile.empty()) {
        this->addBack();
        return;
    }
    
    discard_pile.emplace_back(std::move(draw_pile.back()));
    draw_pile.pop_back();

    discard_pile.back().printCard();
}

void Deck::printDiscard() {
    discard_pile.back().printCard();
}

void Deck::shuffle() {
    std::cout << "--Shuffling the cards--" << std::endl;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(draw_pile.begin(), draw_pile.end(), g);
}

void Deck::addBack() {
    std::cout << "--Adding discard back into the deck--" << std::endl;
    draw_pile.insert(
        draw_pile.end(),
        std::make_move_iterator(discard_pile.begin()),
        std::make_move_iterator(discard_pile.end())
    );
    discard_pile.clear();
    
    this->shuffle();
}

void Deck::deal(std::vector<Player>& players, int cardCount) {
    for (auto& player : players) {
        for (int i = 0; i < cardCount; ++i) {
            player.hand.emplace_back(std::move(draw_pile.back()));
            draw_pile.pop_back();
        }
    }
}

void Deck::playCard(Card&& cardToPlay) {
    // cardToPlay.printCard();

    // std::cout <<  + "...has been played" << std::endl;
    
    discard_pile.emplace_back(std::move(cardToPlay));
}

void Deck::haveCardDrawn(Player& player) {

    // logic for keeping the final card of discard present
    // and shuffling the rest into draw pile
    if (draw_pile.empty()) {
        Card discard = std::move(discard_pile.back());
        discard_pile.pop_back();
        this->addBack();
        discard_pile.emplace_back(std::move(discard));
    }

    Card card = std::move(draw_pile.back());
    draw_pile.pop_back();
    player.hand.emplace_back(std::move(card));
}