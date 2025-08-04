#include <string>
#include <array>

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
        Value value;
        Colour colour;

        Card(Value value, Colour colour);

        std::string toString();

        bool cmpColour(Card& other);
        bool cmpValue(Card& other);
};