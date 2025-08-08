#include "card.h"

// printing a card
std::string Card::toValString(Value value) {
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
        case Value::reverse: return "reverse";
        case Value::skip: return "skip";
        case Value::drawtwo: return "drawtwo";
        case Value::wild: return "wild";
        case Value::drawfour: return "drawfour";
        default: return "unknown";
    };
}

std::string Card::toColString(Colour colour) {
    switch (colour) {
        case Colour::red: return "red";
        case Colour::yellow: return "yellow";
        case Colour::green: return "green";
        case Colour::blue: return "blue";
        case Colour::black: return "black";
        default: return "unknown";
    };
}

Card::Card(Colour colour, Value value)
    : colour(colour), value(value) {}

Card::Card()
    : colour(Colour::red), value(Value::zero) {}

std::string Card::toString() {
    return "{ " + toColString(this->colour) + " " + toValString(this->value) + " }";
}

std::string Card::getTextureKey() {
    return toColString(this->colour) + "_" + toValString(this->value);
}

void Card::printCard() {
    std::cout << this->toString() << std::endl;
}

bool Card::cmpColour(const Card& other) const {
    return this->colour == other.colour;
}

bool Card::cmpValue(const Card& other) const {
    return this->value == other.value;
}

bool Card::isValid(const Card& other) const {
    if ((this->colour == other.colour) || (this->value == other.value)) return true;
    if (this->colour == Colour::black) return true;
    return false;
}

bool Card::operator==(const Card& other) const {
    return ((this->colour == other.colour) && (this->value == other.value));
}