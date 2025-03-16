cmake_minimum_required(VERSION 3.25)
project(game_engine)

set(CMAKE_CXX_STANDARD 17)

add_executable(game_engine WinMain.cpp WindowsMessageMap.cpp WindowsMessageMap.h Window/Window.cpp Window/Window.h Exception/CustomException.cpp Exception/CustomException.h WinInclude.h IO/Keyboard/Keyboard.cpp IO/Keyboard/Keyboard.h
        IO/Mouse/Mouse.cpp
        IO/Mouse/Mouse.h
        App/App.cpp
        App/App.h
        Timer/Timer.cpp
        Timer/Timer.h)

SET(BUILD_SHARED_LIBS OFF)
SET(ASSIMP_BUILD_TESTS OFF)
SET(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
SET(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
SET(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
SET(ASSIMP_BUILD_FBX_IMPORTER ON)
SET(ASSIMP_BUILD_OBJ_IMPORTER ON)
add_subdirectory(third_party/assimp)

target_link_libraries(game_engine PRIVATE assimp)
