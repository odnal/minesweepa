#include <stdio.h>
#include "./raylib/raylib.h"

#define SCREEN_WIDTH    500
#define SCREEN_HEIGHT   500

#define ROWS 9
#define COLS 9

#define CELL_WIDTH  (SCREEN_WIDTH/COLS)
#define CELL_HEIGHT (SCREEN_HEIGHT/ROWS)

#define GRID_WIDTH  (CELL_WIDTH * COLS)
#define GRID_HEIGHT (CELL_HEIGHT * ROWS)

#define PADDING_X ((SCREEN_WIDTH - GRID_WIDTH) / 2)
#define PADDING_Y ((SCREEN_HEIGHT - GRID_HEIGHT) / 2)

// maybe use enum for the state of a given cell via how the game is controlled
typedef enum {
    CELL_BLANK = 0,
    CELL_ONE,
    CELL_TWO,
    CELL_THREE,
    CELL_FOUR,
    CELL_FIVE,
    CELL_SIX,
    CELL_SEVEN,
    CELL_EIGHT,
    CELL_MINE,
    CELL_FLAG
} Cell_Kind;

typedef struct { Cell_Kind kind;
} Cell;

Cell grid[ROWS][COLS] = {0};

typedef enum {
    STATE_PLAYING = 0
} game_states;

game_states game_state;

void Render_Grid()
{
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            DrawRectangleLines(PADDING_X + j * CELL_WIDTH, PADDING_Y + i * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, GRAY);
        }
    }
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mine Sweepa");

    printf("CELL_WIDTH: %d\n", CELL_WIDTH);
    printf("CELL_HEIGHT: %d\n", CELL_HEIGHT);
    printf("GRID_WIDTH: %d\n", GRID_WIDTH);
    printf("GRID_WIDTH: %d\n", GRID_WIDTH);
    printf("PADDING_X: %d\n", PADDING_X);
    printf("PADDING_Y: %d\n", PADDING_Y);

    while(!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        Render_Grid();

        EndDrawing();

    }

    CloseWindow();
    return 0;
}
