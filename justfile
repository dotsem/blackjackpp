default:
    @just --list

build:
    cmake --build build

run:
    just build && ./build/blackjack++

setup:
    cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1 && ln -s build/compile_commands.json .

test filter="*":
    cmake --build build --target unit_tests && ./build/unit_tests --gtest_filter={{filter}}

