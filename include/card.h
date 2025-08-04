#include <string>
#include <array>
#include <iostream>

#pragma once

enum class Value {
    zero,
    one,
    two
};

constexpr std::array<Value, 3> all_values = {
    Value::zero, Value::one, Value::two
};

enum class Colour {
    red,
    yellow
};

constexpr std::array<Colour, 2> all_colours = {
    Colour::red, Colour::yellow
};

class Card {
    public:
        Colour colour;
        Value value;

        Card(Colour colour, Value value);

        std::string toString();
        void printCard();

        bool cmpColour(Card& other);
        bool cmpValue(Card& other);
};