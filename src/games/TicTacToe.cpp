////////////////////////////////////////////////////////////////////////////////
// Module Name: TicTacToe
//
// Description: Contains function and calling code for the (currently
//              single-player) game Tic-Tac-Toe
////////////////////////////////////////////////////////////////////////////////

// Imports
#include "../SCGDEngineGamesList.h"

// Globals
#define DISPLAYBLOCKSIZE 60 // Sizing of display
typedef struct {
    int xPos;   // Contains pixel X coordinate of top left
    int yPos;   // Contains pixel Y coordinate
    int row;    // Used to calculate yPos
    int column; // Used to calculate xPos
} tictactoe_cursor_t;
tictactoe_cursor_t tictactoeCursor;

int gameGrid[3][3] = {0};
int tictactoeFinish = 0; // Indicator for completion of game
const char CPUWins[15] = "   CPU Wins!  ";
const char UserWins[15] = "  User Wins!  ";
const char drawWins[15] = "    Draw!     ";

// ===== Initialization ========================================================

void tictactoePlayGame(TFT_eSPI tft) {
    // Start game display function
    tictactoeStartDisplay(tft);

    // waiting for A button input
    while (true) {
        if (digitalRead(A_BUTTON)) {
            // stall until button is unpressed
            while (digitalRead(A_BUTTON))
                ;
            // leave waiting loop
            break;
        }
        if (digitalRead(B_BUTTON)) {
            while (digitalRead(B_BUTTON))
                ;
            return; // Go back to main screen
        }
    }

    tictactoeResetGrid();

    // set cursor to first block
    tictactoeCursor.x = 0;
    tictactoeCursor.y = 0;

    // draws game grid
    tictactoeDrawGameGrid(tft);

    // reset indicator bits to 0
    tictactoeFinish = 0;
}

// ===== Game looping ==========================================================

void tictactoeEventLoop(TFT_eSPI tft) {
    int gameChecker;
    while (true) {
        // if the game has not been finished
        if (!tictactoeFinish) {
            // have player take a turn
            tictactoeplayPlayerTurn(tft);
            gameChecker = tictactoeFilled() + tictactoeWinner();
            // check if the game is finished before having CPU play
            if (!tictactoeFinish) {
                delay(1000);
                tictactoeplayCPUTurn(tft);
            }
        } else {
            // if it is a draw
            if (tictactoeFilled() && !tictactoeWinner()) {
                drawMessageBoxTwoLines(tft, TFT_WHITE,
                                       // line 1
                                       drawWins, TFT_WHITE,
                                       // line 2
                                       "   (A) Home   ", TFT_YELLOW);
            }
            // if user wins
            if (tictactoeWinner() == 1) {
                drawMessageBoxTwoLines(tft, TFT_WHITE,
                                       // line 1
                                       UserWins, TFT_WHITE,
                                       // line 2
                                       "   (A) Home   ", TFT_YELLOW);
            }
            // if cpu wins
            if (tictactoeWinner() == 2) {
                drawMessageBoxTwoLines(tft, TFT_WHITE,
                                       // line 1
                                       CPUWins, TFT_WHITE,
                                       // line 2
                                       "   (A) Home   ", TFT_YELLOW);
            }
            return; // This will be exiting the game
        }
    }
}

// ===== Controller ============================================================

void tictactoeResetGrid() {
    int i;
    int j;
    // iterates through gameGrid and resets everything
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            gameGrid[i][j] = 0;
        }
    }
}

// tictactoe grid filled checker
int tictactoeFilled() {
    int i, j;
    // iterate through entire gameGrid
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            // if any index = 0
            if (!gameGrid[i][j]) {
                return 0;
            }
        }
    }
    // else return 1
    tictactoeFinish = 1;
    return 1;
}
// adds the win functions together to determine the winner
int tictactoeWinner() {
    if (horizontalWin() + verticalWin() + diagonalWin()) {
        tictactoeFinish = 1;
    }
    return (horizontalWin() + verticalWin() + diagonalWin());
}

// outputs 1 for User Win 2 for CPU win else 0
int horizontalWin() {
    int i;
    // iterate through each row checking if the row is equal
    for (i = 0; i < 3; i++) {
        if (gameGrid[i][0] == gameGrid[i][1] == gameGrid[i][2]) {
            return i;
        }
    }
    return 0;
}

// outputs 1 for User Win 2 for CPU win else 0
int verticalWin() {
    int i;
    // iterate through each column checking if the column is equal
    for (i = 0; i < 3; i++) {
        if (gameGrid[0][i] == gameGrid[1][i] == gameGrid[2][i]) {
            return i;
        }
    }
    return 0;
}

// outputs 1 for User Win 2 for CPU win else 0
int diagonalWin() {
    // left to right diagonal
    if (gameGrid[0][0] == gameGrid[1][1] == gameGrid[2][2]) {
        return gameGrid[0][0];
    }
    // right to left diagonal
    if (gameGrid[0][2] == gameGrid[1][1] == gameGrid[2][0]) {
        return gameGrid[0][2];
    }
    return 0;
}

// ===== Display ===============================================================

void tictactoeDrawGameIcon(TFT_eSPI tft, int16_t posX, int16_t posY) {
    // Place tictactoe icon from flash memory and draw
    tft.drawBitmap(posX, posY, TICTACTOEICON_PIXELDATA, 60, 60, TFT_DARKGREEN);
    // Draw bounding box
    tft.drawRect(posX, posY, 60, 60, TFT_WHITE);
}

void tictactoeStartDisplay(TFT_eSPI tft) {
    tft.fillScreen(TFT_BLACK);
    drawMessageBoxThreeLines(tft, TFT_WHITE,
                             // line 1
                             "  Welcome to  ", TFT_WHITE,
                             // line 2
                             "  TIC-TAC-TOE ", TFT_WHITE,
                             // line 3
                             "   (A) Play   ", TFT_YELLOW);
}

void tictactoeDrawGameGrid(TFT_eSPI tft) {
    // Display is x240 by y320
    tft.fillScreen(TFT_BLACK);

    int i; // i will increment 60 pixels down each iteration
    for (i = 30; i <= 210; i += 60) { // iterates to draw the vertical lines
        // Input params: x0, y0, x1, y1, color
        tft.drawLine(i, 70, i, 250, TFT_WHITE);
        // drawing the horizontal line at y=i
    }
    // now we will use i to increment through the horizontal lines by
    // incrementing i vertically
    for (i = 70; i <= 250; i += 60) {
        tft.drawLine(30, i, 210, i, TFT_WHITE);
    }
}
// ===== Webserver =============================================================

// TODO: Implement tictactoe webserver