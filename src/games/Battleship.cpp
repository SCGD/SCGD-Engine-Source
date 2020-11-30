////////////////////////////////////////////////////////////////////////////////
// Module Name: Battleship
//
// Description: Contains function and calling code for the (currently
//              single-player) game Battleship
////////////////////////////////////////////////////////////////////////////////

// Imports
#include "../SCGDEngineGamesList.h"

// Globals
#define TOTAL_SHOTS 24 // The player has 24 shots to win
#define NUM_OF_SHIPS 3 // The total number of ships
typedef struct {
    int xPos;   // Contains pixel X coordinate of top left
    int yPos;   // Contains pixel Y coordinate
    int row;    // Used to calculate yPos
    int column; // Used to calculate xPos
} battleship_cursor_t;
battleship_cursor_t cursor;
const int shipPositions[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0}, // 4 represents a piece of a 4-length ship
    {0, 0, 0, 0, 0, 0, 0, 0}, // 3 represents 3, etc.
    {0, 0, 0, 0, 0, 0, 0, 0}, //
    {0, 0, 0, 4, 4, 4, 4, 0}, //
    {3, 3, 3, 0, 0, 0, 0, 0}, //
    {0, 0, 0, 0, 0, 0, 0, 0}, //
    {0, 0, 0, 0, 0, 0, 0, 0}, //
    {0, 0, 0, 0, 2, 2, 0, 0}  //
};                            // Contains ship positions
int playerShots[8][8] = {0};  // Contains shots taken
int shotsTaken;               // Number of shots taken

// ===== Initialization ========================================================

int battleshipPlayGame(TFT_eSPI tft) {
    // Start game with popup
    battleshipDrawStartGamePopup(tft);
    while (true) {
        if (digitalRead(A_BUTTON)) {
            while (digitalRead(A_BUTTON))
                ;

            battleshipResetPlayerShots();
            battleshipDrawInitializeGameBoard(tft);
            break; // Leave loop
        }
        if (digitalRead(B_BUTTON)) {
            while (digitalRead(B_BUTTON))
                ;
            return; // Go back to main screen
        }
    }

    // Enter event loop
    battleshipEventLoop(tft);
}

// ===== Game looping ==========================================================

void battleshipEventLoop(TFT_eSPI tft) {
    int score = 0;

    while (shotsTaken < TOTAL_SHOTS) {
        score += battleshipPlayPlayerTurn(tft);
        shotsTaken += 1;

        battleshipUpdateScreen(tft, score);
    }

    // Score calculation
    if (score == (4 + 3 + 2)) { // Total number of ship parts
        drawMessageBoxTwoLines(tft, TFT_WHITE,
                               // Line 1
                               "   Game win   ", TFT_WHITE,
                               // Line 2
                               "   (A) Home   ", TFT_YELLOW);
    } else {
        drawMessageBoxTwoLines(tft, TFT_WHITE,
                               // Line 1
                               "   You lost   ", TFT_RED,
                               // Line 2
                               "   (A) Home   ", TFT_YELLOW);
    }
}

// ===== Controller ============================================================

void battleshipPlayPlayerTurn(TFT_eSPI tft) { // Returns 0 if miss, 1 if hit

    // Loop for player inputs
    while (true) {
        if (digitalRead(UP_BUTTON)) {
            while (digitalRead(UP_BUTTON))
                ;
            // Bounds check
            if (cursor.row > 0) { // Top of bounding box
                battleshipMoveCursor(tft, cursor.row - 1, cursor.column);
            }
        }
        if (digitalRead(DOWN_BUTTON)) {
            while (digitalRead(DOWN_BUTTON))
                ;
            // Bounds check
            if (cursor.row < 7) { // Bottom of bounding box
                battleshipMoveCursor(tft, cursor.row + 1, cursor.column);
            }
        }
        if (digitalRead(LEFT_BUTTON)) {
            while (digitalRead(LEFT_BUTTON))
                ;
            // Bounds check
            if (cursor.column > 0) { // Left of bounding box
                battleshipMoveCursor(tft, cursor.row, cursor.column - 1);
            }
        }
        if (digitalRead(RIGHT_BUTTON)) {
            while (digitalRead(RIGHT_BUTTON))
                ;
            // Bounds check
            if (cursor.column < 7) { // Right of bounding box
                battleshipMoveCursor(tft, cursor.row, cursor.column + 1);
            }
        }

        if (digitalRead(A_BUTTON)) {
            while (digitalRead(A_BUTTON))
                ;

            if (!shotsTaken[cursor.row][cursor.column]) break; // Stop looping
        }
    }

    // Score calculation
    playerShots[cursor.row][cursor.column] = 1;
    if (shipPositions[cursor.row][cursor.column]) return 1; // We hit

    return 0; // We didn't hit
}

void battleshipResetPlayerShots() {
    int row, col;
    for (row = 0; row < 8; row++) {
        for (col = 0; col < 8; col++) {
            playerShots[row][col] = 0; // Set to zero value
        }
    }
}

// ===== Display ===============================================================

void battleshipDrawGameIcon(TFT_eSPI tft, int16_t posX, int16_t posY) {
    // Place battleship icon from flash memory and draw
    tft.drawBitmap(posX, posY, BATTLESHIPICON_PIXELDATA, 60, 60, TFT_BLUE);
    // Draw bounding box
    tft.drawRect(posX, posY, 60, 60, TFT_WHITE);
}

void battleshipDrawStartGamePopup(TFT_eSPI tft) {
    tft.fillScreen(TFT_BLACK);
    drawMessageBoxThreeLines(tft, TFT_WHITE,
                             // Line 1
                             "  Welcome to  ", TFT_WHITE,
                             // Line 2
                             "  BATTLESHIP  ", TFT_WHITE,
                             // Line 3
                             "   (A) Play   ", TFT_YELLOW);
}

void battleshipDrawInitializeGameBoard(TFT_eSPI tft) {
    int i; // Iterable tracking size of game grid
    // Start by drawing vertical lines
    for (i = 40; i <= 200; i += 20) {
        // Input params: x0, y0, x1, y1, color
        tft.drawLine(i, 80, i, 240, TFT_WHITE); // Draw line y=i
    }
    // Reuse i to draw vertical lines
    for (i = 80; i <= 240; i += 20) {
        tft.drawLine(40, i, 200, i, TFT_WHITE);
    }
}

void battleshipMoveCursor(TFT_eSPI tft, int row, int column) {
    // Remove the current selection box
    tft.drawRect(cursor.xPos, cursor.yPos, 20, 20, TFT_WHITE);
    // Set the new row and column
    cursor.row = row;
    cursor.column = column;
    // Calculate xPos and yPos
    cursor.xPos = 40 + (column * 20);
    cursor.yPos = 40 + (row * 20);
    // Update the selection box
    tft.drawRect(cursor.xPos, cursor.yPos, 20, 20, TFT_YELLOW);
}

void battleshipUpdateScreen(TFT_eSPI tft, int score) {
    int shipHit;  // Calculated hit or miss position
    int row, col; // Iterables

    // Iterate through and draw hits/misses
    for (row = 0; row < 8; row++) {
        for (col = 0; col < 8; col++) {
            shipHit = playerShots[row][col] * shipPositions[row][col];

            if (playerShots[row][col]) {
                if (shipHit) {
                    tft.fillRect(
                        // x0
                        40 + (col * 20) + 1,
                        // y0
                        40 + (row * 20) + 1,
                        // Height, width, color
                        20, 20, TFT_RED);
                } else {
                    tft.fillRect(
                        // x0
                        40 + (col * 20) + 1,
                        // y0
                        40 + (row * 20) + 1,
                        // Height, width, color
                        20, 20, TFT_BLUE);
                }
            }
        }
    }

    // Draw score and shots remaining
    tft.setCursor(40, 221);
    tft.setFreeFont(TEXT_CHOSEN_FONT);
    tft.setTextColor(TFT_WHITE);
    char scoreLine[15];
    sprintf(scoreLine, "Score: %d", score);
    tft.println(scoreLine);

    if (TOTAL_SHOTS - shotsTaken <= 3) tft.setTextColor(TFT_RED);
    char shotsLine[15];
    sprintf(shotsLine, "Shots left: %d", TOTAL_SHOTS - shotsTaken);
    tft.setCursor(40, 221 + 18 + 3); // Offset char height + baseline height
    tft.println(shotsLine);
}

// ===== Webserver =============================================================

// TODO: Implement battleship webserver