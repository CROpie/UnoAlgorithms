cmake_minimum_required(VERSION 3.10)
project(UnoAlgo)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

# Create a library from your source files
add_library(card_lib src/card.cpp)

add_executable(card_tests test/card_tests.cpp)

# Link the library to the test executable
target_link_libraries(card_tests PRIVATE card_lib)

# (Optional) Use FetchContent to get Catch2 for testing
include(FetchContent)

FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.9
)

FetchContent_MakeAvailable(catch2)

target_link_libraries(card_tests PRIVATE Catch2::Catch2)

# Enable CTest and register your test executable
enable_testing()
add_test(NAME CardTests COMMAND card_tests)

add_executable(uno_algo src/main.cpp src/card.cpp src/deck.cpp src/player.cpp src/game.cpp)