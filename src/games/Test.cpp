////////////////////////////////////////////////////////////////////////////////
// Module Name: Test
//
// Description: Contains function and calling code for various testing purposes
////////////////////////////////////////////////////////////////////////////////

// Imports
#include "../SCGDEngineGamesList.h"

// ===== Event looping =========================================================

// ===== Controller ============================================================

// ===== Display ===============================================================

void testDrawGameIcon(TFT_eSPI tft, int16_t posX, int16_t posY) {
    // Place test icon from flash memory and draw
    tft.drawBitmap(posX, posY, TESTICON_PIXELDATA, 60, 60, TFT_BROWN);
    // Draw bounding box
    tft.drawRect(posX, posY, 60, 60, TFT_WHITE);
}

// ===== Webserver =============================================================