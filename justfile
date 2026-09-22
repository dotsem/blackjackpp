default:
    @just --list

build:
    cmake --build build

run:
    ./build/blackjack++

setup:
    cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1 && ln -s build/compile_commands.json .