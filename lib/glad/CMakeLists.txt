
# Include directories
include_directories(include)

add_library(glad STATIC
    src/gl.c
)

target_include_directories(glad 
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)