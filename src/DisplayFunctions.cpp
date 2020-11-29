////////////////////////////////////////////////////////////////////////////////
// Module Name: DisplayFunctions
//
// Description: Contains functionality for display menus and other common
//              visual functionality
////////////////////////////////////////////////////////////////////////////////

// Imports
#include "../SCGDEngineGlobalLib.h"

// ===== Text ==================================================================

// ===== Display Boxes =========================================================
void drawMarginBoundingBox(TFT_eSPI tft, uint16_t color) {
    tft.drawRect(
        // x0 offset from left border
        STD_WORKING_MARGIN,
        // y0 offset from top border
        STD_WORKING_MARGIN,
        // Width
        DISPLAY_WIDTH - (2 * STD_WORKING_MARGIN),
        // Height
        DISPLAY_HEIGHT - (2 * STD_WORKING_MARGIN),
        // Color
        color);
}

void drawMessageBoxOneLine(TFT_eSPI tft, uint16_t boxColor, char *buffer,
                           uint16_t textColor) {
    tft.drawRect(
        // x0 - One text box height up from center
        DISPLAY_POPUP_HORIZ_OFFSET,
        // y0 - Down half height, then up textBox + half margin
        (DISPLAY_HEIGHT / 2) - (TEXT_BOX_HEIGHT / 2) - (STD_WORKING_MARGIN / 2),
        // Width
        DISPLAY_POPUP_WIDTH,
        // Height (down textBox + half margin on top and bottom)
        TEXT_BOX_HEIGHT + (STD_WORKING_MARGIN),
        // Color
        boxColor);

    tft.setCursor(
        // Horizontal offset for characters
        TEXT_BOX_HORIZ_OFFSET,
        // Vertical offset (down half, then move baseline down)
        (DISPLAY_HEIGHT / 2) + TEXT_BOX_BASELINE);
    tft.setFreeFont(TEXT_CHOSEN_FONT);
    tft.setTextColor(textColor);
    tft.println("01234567890123");
}