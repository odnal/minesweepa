#include <stdio.h>
#include <assert.h>
#include "./raylib/raylib.h"

#define SCREEN_WIDTH    500
#define SCREEN_HEIGHT   500

#define ROWS 9
#define COLS 9

#define CELL_WIDTH  ((float)SCREEN_WIDTH/COLS)
#define CELL_HEIGHT ((float)SCREEN_HEIGHT/ROWS)

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
    CELL_MINE
} Cell_Kind;

typedef enum {
    UNOPENED,
    OPENED,
    FLAGGED
} Cell_State;

typedef struct { 
    Cell_Kind kind;
    Cell_State state;
} Cell;

Cell grid[ROWS][COLS];

typedef enum {
    STATE_PLAYING = 0,
    STATE_GAME_OVER,
} game_states;

game_states game_state;

int isCellValid(int col, int row)
{
    return col >= 0 && col < COLS && row >= 0 && row < ROWS;
}

void Reveal_Cell(int col, int row)
{
    if (grid[row][col].state == OPENED || grid[row][col].state == FLAGGED) {
        return;
    }

    grid[row][col].state = OPENED;

    // TODO: implement the cell reveal cases
    // 1. Blank (nothing to worry about and at some point should implement all non-mined adjacent cells will be opened)
    // 2. Number (representing a mine in a location adjacent or diagonal to the current cell)
    // 3. Mine == game over
    switch (grid[row][col].kind) {
        case CELL_MINE:
            game_state = STATE_GAME_OVER;
            break;
    }
}

void Render_Cell(Cell cell, Vector2 pos, Vector2 size)
{
    if (cell.kind == CELL_MINE) {
        DrawRectangleV(pos, size, YELLOW);
    }
    DrawRectangleLinesEx((Rectangle) { pos.x, pos.y, size.x, size.y }, 1, GRAY);
}

void Render_Grid()
{
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            Vector2 position = { j * CELL_WIDTH, i * CELL_HEIGHT };
            Vector2 size = { CELL_WIDTH, CELL_HEIGHT };
            Render_Cell(grid[i][j], position, size);
        }
    }
}

void Reset_Grid()
{
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            grid[i][j].state = UNOPENED;
        }
    }

    // place mines
    int num_mines = (int) (ROWS * COLS * .125f);
    while (num_mines > 0) {
        int col = GetRandomValue(0, COLS-1);
        int row = GetRandomValue(0, ROWS-1);
        printf("num_mines: %d (%d, %d): \n", num_mines, row, col);
        if (grid[row][col].kind != CELL_MINE) {
            grid[row][col].kind = CELL_MINE;
            num_mines--;
        }
    }
    // TODO: next
    // count near mines for the opened cell
}

void Game_Render()
{
    switch(game_state) {
        case STATE_PLAYING:
            // API for left click:
            // 1. Blank (nothing to worry about and at some point should implement all non-mined adjacent cells will be opened)
            // 2. Number (representing a mine in a location adjacent or diagonal to the current cell)
            // 3. Mine == game over
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mouse_pos = GetMousePosition();
                const int col = (int) (mouse_pos.x / CELL_WIDTH);
                const int row = (int) (mouse_pos.y / CELL_HEIGHT);

                if (isCellValid(col, row)) {
                    if (grid[row][col].state == UNOPENED) {
                        printf("TRUE\n");
                    }
                    Reveal_Cell(col, row);
                }
            }
            // API for right click:
            // Flag (player can flag a cell if the think a mine is under it)
            else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                Vector2 mouse_pos = GetMousePosition();
                const int col = (int) (mouse_pos.x / CELL_WIDTH);
                const int row = (int) (mouse_pos.y / CELL_HEIGHT);

                if (isCellValid(col, row)) {
                    if (grid[row][col].state == UNOPENED) {
                        grid[row][col].state = FLAGGED;
                        printf("FLAGGED\n");
                    } else {
                        grid[row][col].state = UNOPENED;
                        printf("UNFLAGGED\n");
                    }
                }
            }
            break;
        case STATE_GAME_OVER:
            assert(0 && "GAME OVER!! - not implemented yet");
            break;

    }
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mine Sweepa");
    // initialize the grid
    Reset_Grid();

    while(!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        Render_Grid();
        Game_Render();

        EndDrawing();

    }

    CloseWindow();
    return 0;
}
