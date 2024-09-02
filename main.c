#include <stdio.h>
#include <assert.h>
#include "./raylib/raylib.h"

#define SCREEN_WIDTH    500
#define SCREEN_HEIGHT   500

#define ROWS 9
#define COLS 9

#define CELL_WIDTH  ((float)SCREEN_WIDTH/COLS)
#define CELL_HEIGHT ((float)SCREEN_HEIGHT/ROWS)

typedef enum {
    CELL_BLANK = 0,
    CELL_MINE
} Cell_Kind;

typedef enum {
    UNOPENED = 0,
    OPENED,
    FLAGGED
} Cell_State;

typedef struct { 
    Cell_Kind kind;
    Cell_State state;
    int nearby_mines;
} Cell;

Cell grid[ROWS][COLS];

typedef enum {
    STATE_PLAYING = 0,
    STATE_GAME_OVER,
} game_states;

game_states game_state;

int count_mines(int r, int c)
{
    int num_mines = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i != 0 && j != 0) continue; // skip diags
            int nrow = r + i;
            int ncol = c + j;
            if (nrow >= 0 && nrow < ROWS && ncol >= 0 && ncol < COLS) {
                if (grid[nrow][ncol].kind == CELL_MINE) {
                    num_mines++;
                }
            }
        }
    }
    return num_mines;
}

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

    // cell reveal cases
    // 1. Blank (nothing to worry about and at some point should implement all non-mined adjacent cells will be opened)
    // 2. Mine == game over
    switch (grid[row][col].kind) {
        case CELL_BLANK:
            game_state = STATE_PLAYING;
            break;
        case CELL_MINE:
            game_state = STATE_GAME_OVER;
            break;
    }
}

void Render_Cell(Cell cell, Vector2 pos, Vector2 size)
{
    if (cell.state == OPENED) {
        if (cell.kind == CELL_MINE) {
            DrawRectangleV(pos, size, YELLOW);
        } else if (cell.nearby_mines > 0) {
            // Check if there is a better way to center text within a given cell
            DrawRectangleV(pos, size, CLITERAL(Color){ 120, 120, 120, 255 }); // balanced gray color i prefer
            DrawText(TextFormat("%d", cell.nearby_mines), pos.x + 24, pos.y + 18, 24, BLACK);
        } 
    } else if (cell.state == FLAGGED) {
        DrawRectangleV(pos, size, GREEN);
    } else if (cell.kind == CELL_MINE) { // DELETE ME at some point
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
            grid[i][j].kind = CELL_BLANK;
            grid[i][j].nearby_mines = 0;
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
    // count near by mines for the opened cell
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            grid[i][j].nearby_mines = count_mines(i, j);
        }
    }
}

void Game_Render()
{
    // TODO: implement a main menu state that displays a legend of how to play and when to start a game
    switch(game_state) {
        case STATE_PLAYING:
            // API for left click:
            // 1. Blank (nothing to worry about and at some point should implement all non-mined adjacent cells will be opened)
            // 2. Mine == game over
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

                if (isCellValid(col, row) && grid[row][col].state != OPENED) {
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
            //assert(0 && "GAME OVER!! - not implemented yet");
            // TODO: Display some UI indicators for game over and how to restart the game or go to the main menu
            if (IsKeyPressed(KEY_SPACE)) {
                Reset_Grid();
                game_state = STATE_PLAYING;
            }
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
