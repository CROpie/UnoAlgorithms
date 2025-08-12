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
    nine,
    reverse,
    skip,
    drawtwo,
    wild,
    drawfour
};

constexpr std::array<Value, 15> all_values = {
    Value::zero, Value::one, Value::two, Value::three, Value::four, 
    Value::five, Value::six, Value::seven, Value::eight, Value::nine,
    Value::reverse, Value::skip, Value::drawtwo, Value::wild, Value::drawfour

};

enum class Colour {
    red,
    yellow,
    green,
    blue,
    black
};

constexpr std::array<Colour, 5> all_colours = {
    Colour::red, Colour::yellow, Colour::green, Colour::blue, Colour::black
};

class Card {
    public:
        Colour colour;
        Value value;

        Card(Colour colour, Value value);
        Card();

        std::string toString();
        static std::string toColString(Colour colour);
        static std::string toValString(Value value);
        std::string getTextureKey();
        void printCard();

        bool cmpColour(const Card& other) const;
        bool cmpValue(const Card& other) const;
        bool isValid(const Card& other) const;
        bool isPlain();

        bool operator==(const Card& other) const;
};