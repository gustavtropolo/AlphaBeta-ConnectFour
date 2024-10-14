
/*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com  */

#include "raylib.h"
#include "ConnectFour.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdint>
#include <unordered_set>

#define FRAMECOLOR CLITERAL(Color){166, 165, 201, 255}
#define LINESCOLOR CLITERAL(Color){106, 105, 141, 255}
// #if defined(PLATFORM_WEB)
//     #include <emscripten/emscripten.h>
// #endif

void drawFrameLines();
void drawCircles(int mouseX, int mouseY);
void drawFrame();
void makeMove(int mouseX, int mouseY);
void checkForWin();
void makeAImove();
void endScreen();

const int FRAME_SIZE = 15;
const int screenWidth = 1000;
const int screenHeight = 600;
const int frameWidth = FRAME_SIZE * 42;
const int frameHeight = FRAME_SIZE * 36;
const int frameX = (screenWidth - frameWidth) / 2;
const int frameY = (screenHeight - frameHeight) / 2;
Rectangle frame = Rectangle({frameX, frameY, frameWidth, frameHeight});
static bool isPlayerTurn = true;
static bool gameOver = false;
ConnectFour game;

int main() {
    //RUN TESTS

    InitWindow(screenWidth, screenHeight, "Connect 4");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        checkForWin();
        if (!gameOver) {
            if (isPlayerTurn && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                makeMove(mouseX, mouseY);
                game.printBoard();
                std::cout << std::endl;
            } else if (!isPlayerTurn) {
                game.makeMove(game.getAImove(), true);
                game.printBoard();
                isPlayerTurn = true;
            }
        } else { //game is over
            DrawText("Press R to Restart or ESC to Exit", screenWidth / 2 - 150, screenHeight / 2, 20, DARKGRAY);
            if (IsKeyPressed(KEY_R)) {
                // Reset the game state to restart
                gameOver = false;
                isPlayerTurn = true;
                //game.reset();
            }
        }
     
        drawCircles(mouseX, mouseY);
        drawFrame();
    }
    // De-Initialization
    CloseWindow();                  // Close window and OpenGL context
    return 0;
}

void drawFrameLines() {
    int inc = (int)frameWidth / 7;
    for (int i = 1; i < 7; i++) { //draw vertical lines
        int x = frameX + (inc * i);
        DrawLine(x, frameY, x, frameY + frameHeight, LINESCOLOR);
    }
    for (int i = 1; i < 6; i++) {
        int y = frameY + (inc * i);
        DrawLine(frameX, y, frameX + frameWidth, y, LINESCOLOR);
    }
}

void drawFrame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectangleRounded(frame, 0.008 * FRAME_SIZE, 1000, FRAMECOLOR);
    drawCircles(GetMouseX(), GetMouseY());
    drawFrameLines();
    EndDrawing();
}

void drawCircles(int mouseX, int mouseY) {
    int inc = frameWidth / 7;
    int size = frameWidth / 17;
    int buffer = inc / 2;
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 7; col++) {
            int x = frameX + (inc * col);
            int y = frameY + (inc * row);
            if (game.board[row][col] == 'X') {
                DrawCircle(x + buffer, y + buffer, size, RED);
            } else if (game.board[row][col] == 'O') {
                DrawCircle(x + buffer, y + buffer, size, YELLOW);
            } else if (mouseX >= x && mouseX <= x + inc && row == game.rowArray[col] && isPlayerTurn) {
                DrawCircle(x + buffer, y + buffer, size, LIGHTGRAY);
            } else {
                DrawCircle(x + buffer, y + buffer, size, WHITE);
            }
        }
    }
}

void makeMove(int mouseX, int mouseY) {
    int column;
    int inc = frameWidth / 7;
    for (int col = 0; col < 7; col++) {
            int xPos = frameX + (inc * col);
            if (mouseX >= xPos && mouseX <= xPos + inc) {
                column = col;
            }
        }
    if (game.isLegalMove(column)) {
        game.makeMove(column, false);
        isPlayerTurn = false;
    }
}

void checkForWin() {
    int size = 16;
    int posY = 
    frameY / 2 - 5;
    if (game.isWin('X')) {
        int posX = (screenWidth - MeasureText("Red Player Wins!", size)) / 2;
        DrawText("Red Player Wins!", posX, posY, size, DARKGRAY);
        gameOver = true;
    } else if (game.isWin('O')) {
        int posX = (screenWidth - MeasureText("Yellow Player Wins!", size)) / 2;
        DrawText("Yellow Player Wins!", posX, posY, size, DARKGRAY);
        gameOver = true;
    } else if (game.isTie()) {
        int posX = (screenWidth - MeasureText("Womp womp, the game is a tie.", size)) / 2;
        DrawText("Womp womp, the game is a tie.", posX, posY, size, DARKGRAY);
        gameOver = true;
    }

    if (gameOver) {
        int posX = (screenWidth - MeasureText("Press R to restart or ESC to exit.", size)) / 2;
        posY = (screenHeight + frameY + frameHeight) / 2 - 5;
        DrawText("Press R to restart or ESC to exit.", posX, posY, size, DARKGRAY);
    }
}


////////////// TESTS \\\\\\\\\\\\\\\\\\\\

// void isWinTest() {
//     UINT64_C pos = 
// }
