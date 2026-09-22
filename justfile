default:
    @just --list

build:
    cmake --build build

run:
    ./build/blackjack++