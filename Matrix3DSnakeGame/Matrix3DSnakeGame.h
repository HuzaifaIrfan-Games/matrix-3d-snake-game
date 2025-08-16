
#pragma once

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DIR_X_POS,  // +X direction
    DIR_X_NEG,  // -X direction
    DIR_Y_POS,  // +Y direction
    DIR_Y_NEG,  // -Y direction
    DIR_Z_POS,  // +Z direction
    DIR_Z_NEG   // -Z direction
} SnakeDirection;


typedef struct {
    unsigned char r: 8; // Red component (0-255)
    unsigned char g: 8; // Green component (0-255)
    unsigned char b: 8; // Blue component (0-255)
} ColorRGB;


#define MATRIX_SIZE 5

// struct {
//     ColorRGB ledMatrixBuffer[MATRIX_SIZE][MATRIX_SIZE][MATRIX_SIZE];
// } snakeGame;

// Matrix3DBuffer matrix3DBuffer;


// Function to start the game
void start_game();

#ifdef __cplusplus
}
#endif