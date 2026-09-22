cmake_minimum_required(VERSION 4.4)

include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v7.0.3
)
FetchContent_MakeAvailable(ftxui)
 
add_executable(blackjack++ main.cpp)
target_link_libraries(blackjack++ PRIVATE ftxui::ftxui)