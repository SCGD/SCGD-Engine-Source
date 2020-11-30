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
    int x;      // Contains pixel X coordinate of top left
    int y;      // Contains pixel Y coordinate
    int row;    // Used to calculate yPos
    int column; // Used to calculate xPos
} tictactoe_cursor_t;
tictactoe_cursor_t tictactoeCursor;
tictactoe_cursor_t tictactoeCPUCursor;

int gameGrid[3][3] = {0};
int tictactoeFinish = 0; // Indicator for completion of game
char CPUWins[15] = "   CPU Wins!  ";
char UserWins[15] = "  User Wins!  ";
char drawWins[15] = "    Draw!     ";

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
    // draw cursor
    tictactoeDrawCursor(tft);

    // reset indicator bits to 0
    tictactoeFinish = 0;
    tictactoeEventLoop(tft);
}

// ===== Game looping ==========================================================

void tictactoeEventLoop(TFT_eSPI tft) {
    int gameChecker;
    while (true) {
        // if the game has not been finished
        if (!tictactoeFinish) {
            // have player take a turn
            tictactoePlayPlayerTurn(tft);
            gameChecker = tictactoeFilled() + tictactoeWinner();
            // check if the game is finished before having CPU play
            if (!tictactoeFinish) {
                delay(1000);
                tictactoePlayCPUTurn(tft);
                gameChecker = tictactoeFilled() + tictactoeWinner();
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

// * Turns
void tictactoePlayPlayerTurn(TFT_eSPI tft) {
    // waiting for button input
    while (true) {
        if (digitalRead(UP_BUTTON)) {
            while (digitalRead(UP_BUTTON))
                ;
            // bounds check
            if (tictactoeCursor.y > 0) {
                // erase previous cursor
                tictactoeEraseCursor(tft);
                tictactoeCursor.y -= 1;
                // draw new cursor
                tictactoeDrawCursor(tft);
            }
        }
        if (digitalRead(DOWN_BUTTON)) {
            while (digitalRead(DOWN_BUTTON))
                ;
            if (tictactoeCursor.y < 2) { // Bottom of bounding box
                // erase previous cursor
                tictactoeEraseCursor(tft);
                tictactoeCursor.y += 1;
                // draw new cursor
                tictactoeDrawCursor(tft);
            }
        }
        if (digitalRead(LEFT_BUTTON)) {
            while (digitalRead(LEFT_BUTTON))
                ;
            if (tictactoeCursor.x > 0) { // Left of bounding box
                // erase previous cursor
                tictactoeEraseCursor(tft);
                tictactoeCursor.x -= 1;
                // draw new cursor
                tictactoeDrawCursor(tft);
            }
        }
        if (digitalRead(RIGHT_BUTTON)) {
            while (digitalRead(RIGHT_BUTTON))
                ;
            if (tictactoeCursor.x < 2) { // Right of bounding box
                // erase previous cursor
                tictactoeEraseCursor(tft);
                tictactoeCursor.x += 1;
                // draw new cursor
                tictactoeDrawCursor(tft);
            }
        }
        if (digitalRead(A_BUTTON)) {
            // stall until button is unpressed
            while (digitalRead(A_BUTTON) == 1)
                ;
            // if the space is empty
            if (!(gameGrid[tictactoeCursor.y][tictactoeCursor.x])) {
                // 1 represents user placement of an X
                gameGrid[tictactoeCursor.y][tictactoeCursor.x] = 1;
                // display user play
                tictactoeDisplayUserPlay(tft);
                break; // stop looping functionality
            }
        }
    }
}

void tictactoePlayCPUTurn(TFT_eSPI tft) {
    // find where last play for user was
    int lastPlayX = tictactoeCursor.x;
    int lastPlayY = tictactoeCursor.y;
    int playMade = 0;
    int i, j;
    // check to the left for free space
    if (lastPlayX && !playMade) {
        if (!(gameGrid[lastPlayY][lastPlayX - 1])) {
            tictactoeCPUCursor.y = lastPlayY;
            tictactoeCPUCursor.x = lastPlayX - 1;
            tictactoeDisplayCPUPlay(tft);
            gameGrid[tictactoeCPUCursor.y][tictactoeCPUCursor.x] = 2;
            playMade = 1;
        }
    }
    // check to the right for free space
    if ((lastPlayX < 2) && !playMade) {
        if (!(gameGrid[lastPlayY][lastPlayX + 1])) {
            tictactoeCPUCursor.y = lastPlayY;
            tictactoeCPUCursor.x = lastPlayX + 1;
            tictactoeDisplayCPUPlay(tft);
            gameGrid[tictactoeCPUCursor.y][tictactoeCPUCursor.x] = 2;
            playMade = 1;
        }
    }
    // check down
    if (lastPlayY && !playMade) {
        if (!(gameGrid[lastPlayY - 1][lastPlayX])) {
            tictactoeCPUCursor.y = lastPlayY - 1;
            tictactoeCPUCursor.x = lastPlayX;
            tictactoeDisplayCPUPlay(tft);
            gameGrid[tictactoeCPUCursor.y][tictactoeCPUCursor.x] = 2;
            playMade = 1;
        }
    }
    // check up
    if ((lastPlayY < 2) && !playMade) {
        if (!(gameGrid[lastPlayY + 1][lastPlayX])) {
            tictactoeCPUCursor.y = lastPlayY + 1;
            tictactoeCPUCursor.x = lastPlayX;
            tictactoeDisplayCPUPlay(tft);
            gameGrid[tictactoeCPUCursor.y][tictactoeCPUCursor.x] = 2;
            playMade = 1;
        }
    } else {
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                // if the slot is empty fill for CPU
                if (gameGrid[i][j] == 0) {
                    // 2 represents a CPU placement of an O
                    tictactoeCPUCursor.y = i;
                    tictactoeCPUCursor.x = j;
                    tictactoeDisplayCPUPlay(tft);
                    gameGrid[i][j] = 2;
                    break; // exit the for loops
                }
            }
        }
    }
}
// * General helpers
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

// * Game completion checkers
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

// * X and O display
void tictactoeDisplayUserPlay(TFT_eSPI tft) {
    // left to right up to down line
    tft.drawLine(
        // x0 coordinate left side of block right 5
        30 + DISPLAYBLOCKSIZE * tictactoeCursor.x + 5,
        // y0 coordinate top of block down 5
        70 + DISPLAYBLOCKSIZE * tictactoeCursor.y + 5,
        // x final coordinate right side of block left 5
        90 + DISPLAYBLOCKSIZE * tictactoeCursor.x - 5,
        // y final coordinate bottom of block up 5
        130 + DISPLAYBLOCKSIZE * tictactoeCursor.y - 5,
        // color
        TFT_BLUE);

    // right to left up to down line
    tft.drawLine(
        // x0 coordinate right side of block left 5
        90 + DISPLAYBLOCKSIZE * tictactoeCursor.x - 5,
        // y0 coordinate top of block down 5
        70 + DISPLAYBLOCKSIZE * tictactoeCursor.y + 5,
        // x final coordinate left side of block right 5
        30 + DISPLAYBLOCKSIZE * tictactoeCursor.x + 5,
        // y final coordinate bottom of block up 5
        130 + DISPLAYBLOCKSIZE * tictactoeCursor.y - 5,
        // color
        TFT_BLUE);
}

void tictactoeDisplayCPUPlay(TFT_eSPI tft) {
    tft.drawCircle(
        // center x coordinate left side of block + 30 right to center
        30 + 30 + DISPLAYBLOCKSIZE * tictactoeCPUCursor.x,
        // center y coordinate top of block + 30 down to center
        70 + 30 + DISPLAYBLOCKSIZE * tictactoeCPUCursor.y,
        // radius
        25,
        // color
        TFT_RED);
}

// * Cursor display
void tictactoeDrawCursor(TFT_eSPI tft) {
    // cursor indicator similar to battleship but with new block size
    tft.drawRect(
        // x coordinate
        30 + DISPLAYBLOCKSIZE * tictactoeCursor.x + 1,
        // y coordinate
        70 + DISPLAYBLOCKSIZE * tictactoeCursor.y + 1,
        // height
        DISPLAYBLOCKSIZE - 1,
        // width
        DISPLAYBLOCKSIZE - 1,
        // color
        TFT_GREEN);
}

void tictactoeEraseCursor(TFT_eSPI tft) {
    // draw over a previous cursor with black to "erase it"
    tft.drawRect(
        // x coordinate
        30 + DISPLAYBLOCKSIZE * tictactoeCursor.x + 1,
        // y coordinate
        70 + DISPLAYBLOCKSIZE * tictactoeCursor.y + 1,
        // height
        DISPLAYBLOCKSIZE - 1,
        // width
        DISPLAYBLOCKSIZE - 1,
        // color
        TFT_BLACK);
}

// ===== Webserver =============================================================

// TODO: Implement tictactoe webserver