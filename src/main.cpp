#include <iostream>
#include "card.h"

int main() {
    Card redTwo = Card{Value::two, Colour::red};
    Card yellowTwo = Card{Value::two, Colour::yellow};
    Card redZero = Card{Value::zero, Colour::red};
    Card yellowZero = Card{Value::zero, Colour::yellow};

    if (redTwo.cmpColour(redZero)) {
      std::cout << redTwo.toString() + " colour matches " + redZero.toString()  << std::endl;
    }

    if (redTwo.cmpColour(yellowZero)) {
      std::cout << redTwo.toString() + " colour matches " + yellowZero.toString()  << std::endl;
    } else {
      std::cout << redTwo.toString() + " colour doesn't match " + yellowZero.toString()  << std::endl;
    }

    return 0;
}