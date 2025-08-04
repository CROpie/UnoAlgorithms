#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "card.h"

Card card1{Colour::red, Value::two};
Card card2{Colour::red, Value::zero};
Card card3{Colour::yellow, Value::two};

TEST_CASE("Card colour comparison works", "[Card]") {
    REQUIRE(card1.cmpColour(card2) == true);
    REQUIRE(card1.cmpColour(card3) == true);
}

TEST_CASE("Card value comparison works", "[Card]") {
    REQUIRE(card1.cmpValue(card2) == false);
    REQUIRE(card1.cmpValue(card3) == true);
}