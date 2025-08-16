#pragma once

#ifndef MATRIX_SIZE
#define MATRIX_SIZE 5
#endif

#include <stdio.h>
#include <stdbool.h>

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

const char* snakeDirectionToString(SnakeDirection direction);

typedef struct {
    unsigned char r: 8; // Red component (0-255)
    unsigned char g: 8; // Green component (0-255)
    unsigned char b: 8; // Blue component (0-255)
} ColorRGB;



typedef struct {
    __uint8_t x;
    __uint8_t y;
    __uint8_t z;
} SnakeNode;

typedef struct {
    SnakeNode positions[MATRIX_SIZE * MATRIX_SIZE * MATRIX_SIZE];
    int length;
    SnakeDirection snakeDirection;
} Snake;

struct SnakeGame; // Forward declaration

// Function prototype
void game_reset(struct SnakeGame *game);
void game_change_snake_direction(struct SnakeGame *game, SnakeDirection snakeDirection);
void game_loop_tick(struct SnakeGame *game);

// Struct definition
typedef struct SnakeGame {
    ColorRGB ledMatrixColorsBuffer[MATRIX_SIZE][MATRIX_SIZE][MATRIX_SIZE];
    SnakeDirection snakeDirection;
    unsigned char running;
    void (*reset)(struct SnakeGame *);
    void (*change_snake_direction)(struct SnakeGame * , SnakeDirection);
    void (*loop_tick)(struct SnakeGame *);
} SnakeGame;

// Constructor-like init
SnakeGame newSnakeGame(void);



#ifdef __cplusplus
}
#endif
