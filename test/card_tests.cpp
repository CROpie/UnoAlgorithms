#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "card.h"

Card card1{Value::two, Colour::red};
Card card2{Value::zero, Colour::red};
Card card3{Value::two, Colour::yellow};

TEST_CASE("Card colour comparison works", "[Card]") {
    REQUIRE(card1.cmpColour(card2) == true);
    REQUIRE(card1.cmpColour(card3) == true);
}

TEST_CASE("Card value comparison works", "[Card]") {
    REQUIRE(card1.cmpValue(card2) == false);
    REQUIRE(card1.cmpValue(card3) == true);
}