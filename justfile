default:
    @just --list

build:
    cmake --build build

run:
    just build && ./build/blackjack++

setup:
    cmake -B build -G Ninja \
            -DCMAKE_CXX_COMPILER=clang++ \
            -DCMAKE_BUILD_TYPE=Debug \
            -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
            -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
            -DCMAKE_C_COMPILER_LAUNCHER=ccache \
            -DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic -Werror"\
            && ln -s build/compile_commands.json . 2>/dev/null

test filter="*":
    cmake --build build --target unit_tests && ./build/unit_tests --gtest_filter={{filter}}

fmt:
    git ls-files '*.cpp' '*.hpp' | xargs clang-format -i

lint:
    git ls-files '*.cpp' | grep -v '_test.cpp' | xargs clang-tidy -p build
lint-fix:
    git ls-files '*.cpp' | grep -v '_test.cpp' | xargs clang-tidy -p build --fix --fix-errors

clean: 
    rm -rf build && rm -rf .cache && rm -f compile_commands.json