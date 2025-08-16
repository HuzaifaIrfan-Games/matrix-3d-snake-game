

#include "Matrix3DSnakeGame.h"



void game_reset(struct SnakeGame *game){
    game->ledMatrixColorsBuffer[4][4][4] = (ColorRGB){255, 255, 255};
    printf("Game reset requested\n");
}
void game_change_direction(struct SnakeGame *game, SnakeDirection snakeDirection){
    printf("Changing the snake direction to: %d\n", snakeDirection);
}

void game_loop_tick(struct SnakeGame *game){

    game->ledMatrixColorsBuffer[0][0][0] = (ColorRGB){255, 255, 255};


    game->ledMatrixColorsBuffer[1][0][0] = (ColorRGB){255, 0, 0};
    game->ledMatrixColorsBuffer[2][0][0] = (ColorRGB){255, 0, 0};

    game->ledMatrixColorsBuffer[0][1][0] = (ColorRGB){0, 255, 0};
    game->ledMatrixColorsBuffer[0][2][0] = (ColorRGB){0, 255, 0};

    game->ledMatrixColorsBuffer[0][0][1] = (ColorRGB){0, 0, 255};
    game->ledMatrixColorsBuffer[0][0][2] = (ColorRGB){0, 0, 255};
    


}

SnakeGame newSnakeGame(void) {
    SnakeGame game = {0};
    game.reset = game_reset;
    game.change_direction = game_change_direction;
    game.loop_tick = game_loop_tick;
    return game;
}

void start_game() {
    // Initialize game state
    printf("Starting the Matrix 3D Snake Game...\n");
}