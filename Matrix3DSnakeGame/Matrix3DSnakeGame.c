

#include "Matrix3DSnakeGame.h"

#include <string.h> // memset
#include <stdlib.h> // rand

// ----------- Helpers -----------

const char *snakeDirectionToString(SnakeDirection dir)
{
    switch (dir)
    {
    case DIR_X_POS:
        return "+X";
    case DIR_X_NEG:
        return "-X";
    case DIR_Y_POS:
        return "+Y";
    case DIR_Y_NEG:
        return "-Y";
    case DIR_Z_POS:
        return "+Z";
    case DIR_Z_NEG:
        return "-Z";
    default:
        return "Unknown";
    }
}

static SnakeNode dir_to_delta(SnakeDirection d)
{
    switch (d)
    {
    case DIR_X_POS:
        return (SnakeNode){1, 0, 0};
    case DIR_X_NEG:
        return (SnakeNode){-1, 0, 0};
    case DIR_Y_POS:
        return (SnakeNode){0, 1, 0};
    case DIR_Y_NEG:
        return (SnakeNode){0, -1, 0};
    case DIR_Z_POS:
        return (SnakeNode){0, 0, 1};
    case DIR_Z_NEG:
        return (SnakeNode){0, 0, -1};
    default:
        return (SnakeNode){0, 0, 0};
    }
}

static bool same_node(SnakeNode a, SnakeNode b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

static void spawn_food(SnakeGame *game)
{
    while (1)
    {
        SnakeNode n = {
            rand() % MATRIX_SIZE,
            rand() % MATRIX_SIZE,
            rand() % MATRIX_SIZE};
        bool occupied = false;
        for (int i = 0; i < game->snake.length; i++)
        {
            if (same_node(n, game->snake.positions[i]))
            {
                occupied = true;
                break;
            }
        }
        if (!occupied)
        {
            game->food = n;
            return;
        }
    }
}

// ----------- Game API -----------

void game_change_snake_direction(struct SnakeGame *game, SnakeDirection snakeDirection)
{

    // printf("Current Snake direction: %s\n", snakeDirectionToString(game->snake.direction));

    if (game->snake.direction == snakeDirection)
    {
        return;
    }

    if (game->snake.direction == DIR_X_POS && snakeDirection == DIR_X_NEG)
    {
        return;
    }
    if (game->snake.direction == DIR_X_NEG && snakeDirection == DIR_X_POS)
    {
        return;
    }

    if (game->snake.direction == DIR_Y_POS && snakeDirection == DIR_Y_NEG)
    {
        return;
    }
    if (game->snake.direction == DIR_Y_NEG && snakeDirection == DIR_Y_POS)
    {
        return;
    }

    if (game->snake.direction == DIR_Z_POS && snakeDirection == DIR_Z_NEG)
    {
        return;
    }
    if (game->snake.direction == DIR_Z_NEG && snakeDirection == DIR_Z_POS)
    {
        return;
    }

    game->snake.direction = snakeDirection;
    // printf("Changed Snake direction: %s\n", snakeDirectionToString(game->snake.direction));
    return;
}

void game_reset(SnakeGame *game)
{
    memset(game->ledMatrixColorsBuffer, 0, sizeof(game->ledMatrixColorsBuffer));

    game->snake.length = 1;
    game->snake.positions[0] = (SnakeNode){0, 0, 0};
    game->snake.direction = DIR_X_POS;

    game->score = 0;
    game->running = true;

    spawn_food(game);
}

void game_loop_tick(SnakeGame *game)
{
    if (!game->running)
    {

        return;
    }

    game->score++;

    // shift snake forward
    SnakeNode head = game->snake.positions[game->snake.length - 1];
    SnakeNode delta = dir_to_delta(game->snake.direction);

    SnakeNode newHead = {
        head.x + delta.x,
        head.y + delta.y,
        head.z + delta.z};

    // check bounds
    if (newHead.x < 0 || newHead.y < 0 || newHead.z < 0 ||
        newHead.x >= MATRIX_SIZE || newHead.y >= MATRIX_SIZE || newHead.z >= MATRIX_SIZE)
    {
        game->running = false;
        return;
    }

    // check collision with self
    for (int i = 0; i < game->snake.length; i++)
    {
        if (same_node(newHead, game->snake.positions[i]))
        {
            game->running = false;
            return;
        }
    }

    // check food
    if (same_node(newHead, game->food))
    {
        game->score += 10;
        game->snake.length++;
        game->snake.positions[game->snake.length - 1] = newHead;
        spawn_food(game);
    }
    else
    {
        // move snake body
        for (int i = 0; i < game->snake.length - 1; i++)
        {
            game->snake.positions[i] = game->snake.positions[i + 1];
        }
        game->snake.positions[game->snake.length - 1] = newHead;
    }

    // clear buffer
    memset(game->ledMatrixColorsBuffer, 0, sizeof(game->ledMatrixColorsBuffer));

    // draw snake
    for (int i = 0; i < game->snake.length; i++)
    {
        SnakeNode n = game->snake.positions[i];
        game->ledMatrixColorsBuffer[n.x][n.y][n.z] = (ColorRGB){0, 255, 0};
    }

    // draw food
    game->ledMatrixColorsBuffer[game->food.x][game->food.y][game->food.z] = (ColorRGB){255, 0, 0};
}

// Constructor-like init
SnakeGame newSnakeGame(void)
{
    SnakeGame game;
    game.reset = game_reset;
    game.change_snake_direction = game_change_snake_direction;
    game.loop_tick = game_loop_tick;
    game.reset(&game);
    return game;
}