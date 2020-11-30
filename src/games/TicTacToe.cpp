////////////////////////////////////////////////////////////////////////////////
// Module Name: TicTacToe
//
// Description: Contains function and calling code for the (currently
//              single-player) game Tic-Tac-Toe
////////////////////////////////////////////////////////////////////////////////

// Imports
#include "../SCGDEngineGamesList.h"

// ===== Game looping ==========================================================
// TODO: Implement game event loop

// ===== Controller ============================================================
// TODO: Implement game logic

// ===== Display ===============================================================

void tictactoeDrawGameIcon(TFT_eSPI tft, int16_t posX, int16_t posY) {
    // Place battleship icon from flash memory and draw
    tft.drawBitmap(posX, posY, TICTACTOEICON_PIXELDATA, 60, 60, TFT_DARKGREEN);
    // Draw bounding box
    tft.drawRect(posX, posY, 60, 60, TFT_WHITE);
}

// ===== Webserver =============================================================

// TODO: Implement tictactoe webserver