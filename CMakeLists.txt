cmake_minimum_required(VERSION 3.28)

# Project name and version
project(Matrix3DSnakeGame VERSION 1.0)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)
# Source files
set(SOURCES
    src/main.cpp
)

add_library(matrix_3d_snake_game STATIC
    src/matrix_3d_snake_game/game.c
)


add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} matrix_3d_snake_game)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

