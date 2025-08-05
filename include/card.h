#include <string>
#include <array>
#include <iostream>

#pragma once

enum class Value {
    zero,
    one,
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine
};

constexpr std::array<Value, 10> all_values = {
    Value::zero, Value::one, Value::two, Value::three, Value::four, 
    Value::five, Value::six, Value::seven, Value::eight, Value::nine
};

enum class Colour {
    red,
    yellow,
    green,
    blue
};

constexpr std::array<Colour, 4> all_colours = {
    Colour::red, Colour::yellow, Colour::green, Colour::blue
};

class Card {
    public:
        Colour colour;
        Value value;

        Card(Colour colour, Value value);
        Card();

        std::string toString();
        std::string getTextureKey() const;
        void printCard();

        bool cmpColour(Card& other);
        bool cmpValue(Card& other);
        bool isValid(Card& other);

        bool operator==(const Card& other) const;
};