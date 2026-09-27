cmake_minimum_required(VERSION 3.28)
project(blackjack++)

set(CMAKE_CXX_STANDARD 20)  
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

include(FetchContent)
FetchContent_Declare(SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG release-3.4.16
)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.18.0
)
FetchContent_MakeAvailable(SDL3 googletest)

add_library(domain STATIC
    src/domain/game.cpp
)

add_executable(blackjack++ main.cpp src/app.cpp)

target_include_directories(blackjack++ PRIVATE src)
target_include_directories(domain PUBLIC src)
target_compile_definitions(blackjack++ PRIVATE SDL_MAIN_USE_CALLBACKS=1)
target_link_libraries(blackjack++ PRIVATE domain SDL3::SDL3)

enable_testing()
add_executable(unit_tests
    src/domain/card_test.cpp
    src/domain/deck_test.cpp
    src/domain/hand_test.cpp
    src/domain/player_test.cpp
    src/domain/dealer_test.cpp
    src/domain/game_test.cpp
    src/domain/game_event_test.cpp
)

target_include_directories(unit_tests PRIVATE src)
target_link_libraries(unit_tests PRIVATE domain GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)
