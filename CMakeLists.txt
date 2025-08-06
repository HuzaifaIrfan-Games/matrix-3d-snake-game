cmake_minimum_required(VERSION 3.28)

# Project name and version
project(Matrix3DSnakeGame VERSION 1.0)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)


find_package(OpenGL REQUIRED)

# Include directories
include_directories(include)
include_directories(lib/glad/include)

# Source files
set(SOURCES
    src/main.cpp
)

add_library(matrix_3d_snake_game STATIC
    src/matrix_3d_snake_game/game.c
)


add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(lib/glad)

target_link_libraries(${PROJECT_NAME} matrix_3d_snake_game 
    glad
    glfw
    OpenGL::GL
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)







## GTest setup

find_package(GTest REQUIRED)

enable_testing()

add_executable(Matrix3DSnakeGameTest test/matrix_3d_snake_game_test.cc)
target_link_libraries(Matrix3DSnakeGameTest matrix_3d_snake_game GTest::gtest_main GTest::gtest)

add_test(NAME MyTestSuite COMMAND Matrix3DSnakeGameTest)