#include "card.h"

// printing a card
std::string toString(Value value) {
    switch (value) {
        case Value::zero: return "zero";
        case Value::one: return "one";
        case Value::two: return "two";
        default: return "unknown";
    };
}

std::string toString(Colour colour) {
    switch (colour) {
        case Colour::red: return "red";
        case Colour::yellow: return "yellow";
        default: return "unknown";
    };
}

// constructor
Card::Card(Value value, Colour colour)
    : value(value), colour(colour) {}

std::string Card::toString() {
    return "{ " + ::toString(this->colour) + " " + ::toString(this->value) + " }";
}

bool Card::cmpColour(Card& other) {
    return this->colour == other.colour;
}

bool Card::cmpValue(Card& other) {
    return this->value == other.value;
}