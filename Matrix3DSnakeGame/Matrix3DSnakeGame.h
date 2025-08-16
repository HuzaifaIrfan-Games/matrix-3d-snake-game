#pragma once

#define MATRIX_SIZE 5

#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // ---------------- Directions ----------------
    typedef enum
    {
        DIR_X_POS, // +X
        DIR_X_NEG, // -X
        DIR_Y_POS, // +Y
        DIR_Y_NEG, // -Y
        DIR_Z_POS, // +Z
        DIR_Z_NEG  // -Z
    } SnakeDirection;

    const char *snakeDirectionToString(SnakeDirection direction);

    // ---------------- Colors ----------------
    typedef struct
    {
        unsigned char r; // Red   (0–255)
        unsigned char g; // Green (0–255)
        unsigned char b; // Blue  (0–255)
    } ColorRGB;

    // ---------------- Snake Node ----------------
    typedef struct
    {
        unsigned char x;
        unsigned char y;
        unsigned char z;
    } SnakeNode;

    // ---------------- Snake State ----------------
    typedef struct
    {
        SnakeNode positions[MATRIX_SIZE * MATRIX_SIZE * MATRIX_SIZE]; // body
        int length;                                                   // current length
        SnakeDirection direction;                                     // current heading
    } Snake;

    // ---------------- Game State ----------------
    typedef struct SnakeGame
    {
        ColorRGB ledMatrixColorsBuffer[MATRIX_SIZE][MATRIX_SIZE][MATRIX_SIZE]; // render buffer

        Snake snake;    // snake state
        SnakeNode food; // food position
        int score;      // player score
        bool running;   // 1=running, 0=game over

        // Methods
        void (*reset)(struct SnakeGame *);
        void (*change_snake_direction)(struct SnakeGame *, SnakeDirection);
        void (*loop_tick)(struct SnakeGame *);
    } SnakeGame;

    // ---------------- API ----------------
    void game_reset(struct SnakeGame *game);
    void game_change_snake_direction(struct SnakeGame *game, SnakeDirection dir);
    void game_loop_tick(struct SnakeGame *game);

    // Constructor-like init
    SnakeGame newSnakeGame(void);

#ifdef __cplusplus
}
#endif
