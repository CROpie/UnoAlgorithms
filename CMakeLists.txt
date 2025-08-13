cmake_minimum_required(VERSION 3.10)
project(UnoAlgo)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

# ---- Card Library ----
add_library(card_lib src/card.cpp)

# ---- Unit Tests ----
add_executable(card_tests test/card_tests.cpp)

target_link_libraries(card_tests PRIVATE card_lib)

# Catch2
include(FetchContent)

FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.9
)

FetchContent_MakeAvailable(catch2)

target_link_libraries(card_tests PRIVATE Catch2::Catch2)

enable_testing()
add_test(NAME CardTests COMMAND card_tests)

# ---- SFML Executable
# Find SMFL

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

set(CMAKE_BUILD_TYPE Debug)  # Enables -g and disables optimizations

add_executable(uno_algo src/main.cpp src/card.cpp src/deck.cpp src/player.cpp src/game.cpp src/renderer.cpp src/strategy.cpp)

# Link SFML and card lib

target_link_libraries(uno_algo PRIVATE card_lib sfml-graphics sfml-window sfml-system)