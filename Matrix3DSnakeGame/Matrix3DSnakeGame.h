#pragma once


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


#define MATRIX_SIZE 5


typedef struct {
    int x;
    int y;
    int z;
} Position3D;

typedef struct {
    Position3D positions[MATRIX_SIZE * MATRIX_SIZE * MATRIX_SIZE];
    int length;
    SnakeDirection direction;
} Snake;

struct SnakeGame; // Forward declaration

// Function prototype
void game_reset(struct SnakeGame *game);
void game_change_direction(struct SnakeGame *game, SnakeDirection snakeDirection);
void game_loop_tick(struct SnakeGame *game);

// Struct definition
typedef struct SnakeGame {
    ColorRGB ledMatrixColorsBuffer[MATRIX_SIZE][MATRIX_SIZE][MATRIX_SIZE];
    SnakeDirection direction;
    unsigned char running;
    void (*reset)(struct SnakeGame *);
    void (*change_direction)(struct SnakeGame * , SnakeDirection);
    void (*loop_tick)(struct SnakeGame *);
} SnakeGame;

// Constructor-like init
SnakeGame newSnakeGame(void);



#ifdef __cplusplus
}
#endif
