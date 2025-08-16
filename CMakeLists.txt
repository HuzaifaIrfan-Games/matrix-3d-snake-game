cmake_minimum_required(VERSION 3.16)
project(matrix_3d_snake_game VERSION 0.1 LANGUAGES C CXX)  # Add C language support

set(CMAKE_C_STANDARD 11)  # For C code
set(CMAKE_CXX_STANDARD 17)  # For C++/Qt code
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 (Quick for QML)
find_package(Qt6 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.5)

# --- C/C++ Mixed Compilation ---
# Compile the C file as a static library (since Qt needs C++)
add_library(matrix_3d_snake_game_c STATIC
    Matrix3DSnakeGame/Matrix3DSnakeGame.c  # Your C file
)

# Qt executable (C++/QML)
qt_add_executable(matrix_3d_snake_game_app
    main.cpp
    LedMatrixCube/LedMatrixCube.cpp
    LedMatrixCube/UserInputController.cpp
)

# Link the C library to the Qt executable
target_link_libraries(matrix_3d_snake_game_app PRIVATE
    matrix_3d_snake_game_c  # Your C code
    Qt6::Quick
)

# QML Module (unchanged)
qt_add_qml_module(matrix_3d_snake_game_app
    URI matrix_3d_snake_game
    VERSION 1.0
    QML_FILES
        LedMatrixCube/LedMatrixCube.qml
        LedMatrixCube/LedMatrixCubeWindow.qml
        LedMatrixCube/ControlPanel.qml
)

# macOS/iOS Bundle Settings (unchanged)
set_target_properties(matrix_3d_snake_game_app PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install (unchanged)
include(GNUInstallDirs)
install(TARGETS matrix_3d_snake_game_app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)