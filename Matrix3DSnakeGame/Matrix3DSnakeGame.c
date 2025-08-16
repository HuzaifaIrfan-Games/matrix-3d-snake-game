

#include "Matrix3DSnakeGame.h"


const char* snakeDirectionToString(SnakeDirection direction) {
    switch (direction) {
        case DIR_X_POS: return "+X";
        case DIR_X_NEG: return "-X";
        case DIR_Y_POS: return "+Y";
        case DIR_Y_NEG: return "-Y";
        case DIR_Z_POS: return "+Z";
        case DIR_Z_NEG: return "-Z";
        default: return "Unknown";
    }
}


void game_reset(struct SnakeGame *game){
    game->running = 1;

    game->ledMatrixColorsBuffer[0][0][0] = (ColorRGB){255, 255, 255};


    game->ledMatrixColorsBuffer[1][0][0] = (ColorRGB){255, 0, 0};
    game->ledMatrixColorsBuffer[2][0][0] = (ColorRGB){255, 0, 0};

    game->ledMatrixColorsBuffer[0][1][0] = (ColorRGB){0, 255, 0};
    game->ledMatrixColorsBuffer[0][2][0] = (ColorRGB){0, 255, 0};

    game->ledMatrixColorsBuffer[0][0][1] = (ColorRGB){0, 0, 255};
    game->ledMatrixColorsBuffer[0][0][2] = (ColorRGB){0, 0, 255};

}
void game_change_direction(struct SnakeGame *game, SnakeDirection snakeDirection){

    printf("Current Snake direction: %s\n", snakeDirectionToString(game->direction));

    if (game->direction == snakeDirection) {
        return;
    }

    if (game->direction == DIR_X_POS && snakeDirection == DIR_X_NEG) {
        return;
    }
    if (game->direction == DIR_X_NEG && snakeDirection == DIR_X_POS) {
        return;
    }

    if (game->direction == DIR_Y_POS && snakeDirection == DIR_Y_NEG) {
        return;
    }
    if (game->direction == DIR_Y_NEG && snakeDirection == DIR_Y_POS) {
        return;
    }

    if (game->direction == DIR_Z_POS && snakeDirection == DIR_Z_NEG) {
        return;
    }
    if (game->direction == DIR_Z_NEG && snakeDirection == DIR_Z_POS) {
        return;
    }

    game->direction = snakeDirection;
    printf("Changed Snake direction: %s\n", snakeDirectionToString(game->direction));
    return;
}

void game_loop_tick(struct SnakeGame *game){

    


}

SnakeGame newSnakeGame(void) {
    SnakeGame game = {0};
    game.reset = game_reset;
    game.change_direction = game_change_direction;
    game.loop_tick = game_loop_tick;
    return game;
}
