cmake_minimum_required(VERSION 3.24)
project(testFTXUI)

set(CMAKE_CXX_STANDARD 17)

option(FTXUI_BUILD_DOCS OFF)
option(FTXUI_BUILD_EXAMPLES ON)
option(FTXUI_ENABLE_INSTALL OFF)
add_subdirectory(3rd_party/FTXUI)

add_executable(testFTXUI main.cpp)
target_link_libraries(testFTXUI
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component)

add_executable(flowers
        flowers/bitmap.h
        flowers/bitmap.cpp
        flowers/board.h
        flowers/board.cpp
        flowers/game.h
        flowers/game.cpp
        flowers/main.cpp)
target_link_libraries(flowers
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component)
