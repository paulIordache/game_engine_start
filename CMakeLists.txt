cmake_minimum_required(VERSION 3.25)
project(game_engine)

set(CMAKE_CXX_STANDARD 17)

add_executable(game_engine WinMain.cpp WindowsMessageMap.cpp WindowsMessageMap.h Window.cpp Window.h CustomException.cpp CustomException.h WinInclude.h Keyboard.cpp Keyboard.h
        Mouse.cpp
        Mouse.h)
