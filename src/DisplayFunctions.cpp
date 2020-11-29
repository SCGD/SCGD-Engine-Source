////////////////////////////////////////////////////////////////////////////////
// Module Name: DisplayFunctions
//
// Description: Contains functionality for display menus and other common
//              visual functionality
////////////////////////////////////////////////////////////////////////////////

// Imports
#include "../SCGDEngineGlobalLib.h"

void drawMarginBoundingBox(TFT_eSPI tft, uint16_t color) {
    tft.drawRect(
        // x0 offset from left border
        STD_WORKING_MARGIN,
        // y0 offset from top border
        STD_WORKING_MARGIN,
        // Width
        DISPLAY_MARGINAL_WIDTH,
        // Height
        DISPLAY_MARGINAL_HEIGHT,
        // Color
        color);
}