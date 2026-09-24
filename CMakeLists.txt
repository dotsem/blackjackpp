cmake_minimum_required(VERSION 4.4)
project(blackjack++)

include(FetchContent)
FetchContent_Declare(SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG release-3.4.16
)
FetchContent_MakeAvailable(SDL3)

add_executable(blackjack++ main.cpp src/domain/game.cpp)

target_include_directories(blackjack++ PRIVATE src)
target_link_libraries(blackjack++ PRIVATE SDL3::SDL3)