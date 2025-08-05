#include "card.h"

// printing a card
std::string toString(Value value) {
    switch (value) {
        case Value::zero: return "zero";
        case Value::one: return "one";
        case Value::two: return "two";
        case Value::three: return "three";
        case Value::four: return "four";
        case Value::five: return "five";
        case Value::six: return "six";
        case Value::seven: return "seven";
        case Value::eight: return "eight";
        case Value::nine: return "nine";
        default: return "unknown";
    };
}

std::string toString(Colour colour) {
    switch (colour) {
        case Colour::red: return "red";
        case Colour::yellow: return "yellow";
        case Colour::green: return "green";
        case Colour::blue: return "blue";
        default: return "unknown";
    };
}

Card::Card(Colour colour, Value value)
    : colour(colour), value(value) {}

Card::Card()
    : colour(Colour::red), value(Value::zero) {}

std::string Card::toString() {
    return "{ " + ::toString(this->colour) + " " + ::toString(this->value) + " }";
}

std::string Card::getTextureKey() const {
    return ::toString(this->colour) + "_" + ::toString(this->value);
}

void Card::printCard() {
    std::cout << this->toString() << std::endl;
}

bool Card::cmpColour(Card& other) {
    return this->colour == other.colour;
}

bool Card::cmpValue(Card& other) {
    return this->value == other.value;
}

bool Card::isValid(Card& other) {
    if ((this->colour == other.colour) || (this->value == other.value)) return true;
    return false;
}

bool Card::operator==(const Card& other) const {
    return ((this->colour == other.colour) && (this->value == other.value));
}