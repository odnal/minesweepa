#include "./raylib/raylib.h"

#define SCREEN_WIDTH    500
#define SCREEN_HEIGHT   500

#define ROWS 9
#define COLS 9

#define CELL_WIDTH  (SCREEN_WIDTH/COLS)
#define CELL_HEIGHT (SCREEN_HEIGHT/ROWS)

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

typedef struct {
    Cell_Kind kind;
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
            DrawRectangleLines(j * CELL_WIDTH, i * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, GRAY);
        }
    }
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mine Sweepa");

    while(!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        Render_Grid();

        EndDrawing();

    }

    CloseWindow();
    return 0;
}
