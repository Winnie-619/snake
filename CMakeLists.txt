cmake_minimum_required(VERSION 3.20)
project(CoolSnake)

set(CMAKE_CXX_STANDARD 20)

# FTXUI
include(FetchContent)
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG v4.0
)
FetchContent_MakeAvailable(ftxui)

add_executable(CoolSnake src/main.cpp)

target_link_libraries(CoolSnake PRIVATE ftxui::screen ftxui::dom ftxui::component)