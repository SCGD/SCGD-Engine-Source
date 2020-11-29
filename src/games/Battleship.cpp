////////////////////////////////////////////////////////////////////////////////
// Module Name: Battleship
//
// Description: Contains function and calling code for the (currently
//              single-player) game Battleship
////////////////////////////////////////////////////////////////////////////////

// Imports
#include "../SCGDEngineGamesList.h"

// ===== Game looping ==========================================================
// TODO: Implement Battleship game event loop

// ===== Controller ============================================================
// TODO: Implement Battleship game logic

// ===== Display ===============================================================

void battleshipDrawGameIcon(TFT_eSPI tft, int16_t posX, int16_t posY) {
    // Place battleship icon from flash memory and draw
    tft.drawBitmap(posX, posY, BATTLESHIPICON_PIXELDATA, 60, 60, TFT_BLUE);

    tft.drawRect(posX, posY, 60, 60, TFT_WHITE);
}

// ===== Webserver =============================================================

// TODO: Implement Battleship webserver