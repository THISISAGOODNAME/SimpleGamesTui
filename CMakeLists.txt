cmake_minimum_required(VERSION 3.24)
project(SimpleGamesTui)

set(CMAKE_CXX_STANDARD 17)

option(FTXUI_BUILD_DOCS OFF)
option(FTXUI_BUILD_EXAMPLES OFF)
option(FTXUI_ENABLE_INSTALL OFF)
add_subdirectory(3rd_party/FTXUI)

# flower
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

# snake
add_executable(snake
        snake/bitmap.h
        snake/bitmap.cpp
        snake/board.h
        snake/board.cpp
        snake/game.h
        snake/game.cpp
        snake/main.cpp)
target_link_libraries(snake
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component)

# nes emu (SimpleNes backend)
add_subdirectory(3rd_party/SimpleNES)
add_executable(nes
        nes/controller.hpp
        nes/emulator.hpp
        nes/main.cpp)
target_link_libraries(nes
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
        PRIVATE SimpleNES)

# nes emu (agnes backend)
add_executable(nes-agnes
        nes-agnes/agnes.h
        nes-agnes/agnes.c
        nes-agnes/main.cpp)
target_link_libraries(nes-agnes
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
        PRIVATE SimpleNES)

# nes emu (knes backend)
add_executable(nes-knes
        nes-knes/NES.h
        nes-knes/NES.cpp
        nes-knes/cpu.cpp
        nes-knes/memory.cpp
        nes-knes/main.cpp)
target_link_libraries(nes-knes
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
        PRIVATE SimpleNES)
