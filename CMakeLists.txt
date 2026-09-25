cmake_minimum_required(VERSION 4.4)
project(blackjack++)

set(CMAKE_CXX_STANDARD 20)  
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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


add_executable(blackjack++ main.cpp src/domain/game.cpp)

target_include_directories(blackjack++ PRIVATE src)
target_link_libraries(blackjack++ PRIVATE SDL3::SDL3)

enable_testing()
add_executable(unit_tests
    src/domain/game.cpp

    src/domain/card_test.cpp
    src/domain/deck_test.cpp
    src/domain/hand_test.cpp
    src/domain/player_test.cpp
    # src/domain/dealer_test.cpp
    # src/domain/game_test.cpp
)

target_include_directories(unit_tests PRIVATE src)
target_link_libraries(unit_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)
