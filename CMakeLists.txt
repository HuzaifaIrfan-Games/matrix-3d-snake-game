cmake_minimum_required(VERSION 3.16)
project(matrix_3d_snake_game VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 (Quick for QML)
find_package(Qt6 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.5)

# --- C/C++ Mixed Compilation ---
add_library(matrix_3d_snake_game_c STATIC
    Matrix3DSnakeGame/Matrix3DSnakeGame.c
)

# Qt executable (C++/QML)
qt_add_executable(matrix_3d_snake_game_app
    main.cpp
    LedMatrixCube/LedMatrixCube.cpp
    LedMatrixCube/UserInputController.cpp
)

# Link the C library to the Qt executable
target_link_libraries(matrix_3d_snake_game_app PRIVATE
    matrix_3d_snake_game_c
    Qt6::Quick
)

# QML Module
qt_add_qml_module(matrix_3d_snake_game_app
    URI matrix_3d_snake_game
    VERSION 1.0
    QML_FILES
        LedMatrixCube/qml/LedMatrixCube.qml
        LedMatrixCube/qml/LedMatrixCubeWindow.qml
        LedMatrixCube/qml/ControlPanel.qml
)

# macOS/iOS Bundle Settings
set_target_properties(matrix_3d_snake_game_app PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install
include(GNUInstallDirs)
install(TARGETS matrix_3d_snake_game_app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Windows: Automatically deploy Qt dependencies using windeployqt
if(WIN32)
    # Find windeployqt
    get_target_property(QMAKE_EXE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QMAKE_EXE}" DIRECTORY)
    find_program(WINDEPLOYQT_EXE windeployqt HINTS "${QT_BIN_DIR}")

    if(WINDEPLOYQT_EXE)
        # Run windeployqt after build to gather all dependencies
        add_custom_command(TARGET matrix_3d_snake_game_app POST_BUILD
            COMMAND "${WINDEPLOYQT_EXE}"
                    --qmldir "${CMAKE_CURRENT_SOURCE_DIR}/LedMatrixCube/qml"  # Path to QML files
                    --no-translations
                    --no-system-d3d-compiler
                    --no-opengl-sw
                    "$<TARGET_FILE_DIR:matrix_3d_snake_game_app>"
            COMMENT "Deploying Qt runtime dependencies with windeployqt..."
        )
    else()
        message(WARNING "windeployqt not found! Qt DLLs must be manually copied.")
    endif()

endif()