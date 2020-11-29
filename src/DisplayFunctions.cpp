////////////////////////////////////////////////////////////////////////////////
// Module Name: DisplayFunctions
//
// Description: Contains functionality for display menus and other common
//              visual functionality
////////////////////////////////////////////////////////////////////////////////

// Imports
#include "../SCGDEngineGlobalLib.h"

// ===== Text ==================================================================

// ===== Helper functions ======================================================
int calculateMessageBoxOffset(unsigned int boxes) {
    int offset = DISPLAY_HEIGHT / 2;         // Start at midline
    offset -= boxes * (TEXT_BOX_HEIGHT / 2); // Move x boxes up
    offset -= STD_WORKING_MARGIN / 2;        // Add margin
    return offset;
}

int calculateVerticalTextOffset(unsigned int lines) {
    int offset = DISPLAY_HEIGHT / 2;                    // Start at midline
    offset -= ((int)lines / 2) * (TEXT_BOX_HEIGHT / 2); // Move up half # lines
    offset -= TEXT_BOX_BASELINE;                        // Add baseline
    return offset;
}

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
        calculateMessageBoxOffset(1),
        // Width
        DISPLAY_POPUP_WIDTH,
        // Height (down textBox + half margin on top and bottom)
        TEXT_BOX_HEIGHT + STD_WORKING_MARGIN,
        // Color
        boxColor);

    tft.setCursor(
        // Horizontal offset for characters
        TEXT_BOX_HORIZ_OFFSET,
        // Vertical offset
        calculateVerticalTextOffset(1));
    tft.setFreeFont(TEXT_CHOSEN_FONT);
    tft.setTextColor(textColor);
    tft.println(buffer);
}

void drawMessageBoxTwoLines(TFT_eSPI tft, uint16_t boxColor, char *line1,
                            uint16_t line1Color, char *line2,
                            uint16_t line2Color) {
    tft.drawRect(
        // x0
        DISPLAY_POPUP_HORIZ_OFFSET,
        // y0
        calculateMessageBoxOffset(2),
        // Width
        DISPLAY_POPUP_WIDTH,
        // Height
        (2 * TEXT_BOX_HEIGHT) + STD_WORKING_MARGIN,
        // Color
        boxColor);

    tft.setFreeFont(TEXT_CHOSEN_FONT);

    // Set cursor up to top of bounding box
    tft.setCursor(
        // Horizontal offset for characters
        TEXT_BOX_HORIZ_OFFSET,
        // Vertical offset
        calculateVerticalTextOffset(2));

    tft.setTextColor(line1Color);
    tft.println("This works");
    tft.print("    "); // Shift characters from left
    tft.setTextColor(line2Color);
    tft.println(line2);
}

void drawMessageBoxThreeLines(TFT_eSPI tft, uint16_t boxColor, char *line1,
                              uint16_t line1Color, char *line2,
                              uint16_t line2Color, char *line3,
                              uint16_t line3Color) {
    tft.drawRect(
        // x0
        DISPLAY_POPUP_HORIZ_OFFSET,
        // y0
        calculateMessageBoxOffset(3),
        // Width
        DISPLAY_POPUP_WIDTH,
        // Height
        (3 * TEXT_BOX_HEIGHT) + STD_WORKING_MARGIN,
        // Color
        boxColor);

    tft.setFreeFont(TEXT_CHOSEN_FONT);

    // Set cursor up to top of bounding box
    tft.setCursor(
        // Horizontal offset for characters
        TEXT_BOX_HORIZ_OFFSET,
        // Vertical offset
        calculateVerticalTextOffset(3));

    tft.setTextColor(line1Color);
    tft.println(line1);
    tft.print("    "); // Shift characters from left
    tft.setTextColor(line2Color);
    tft.println(line2);
    tft.print("    "); // Shift characters from left
    tft.setTextColor(line3Color);
    tft.println(line3);
}

void drawMessageBoxFourLines(TFT_eSPI tft, uint16_t boxColor, char *line1,
                             uint16_t line1Color, char *line2,
                             uint16_t line2Color, char *line3,
                             uint16_t line3Color, char *line4,
                             uint16_t line4Color) {
    tft.drawRect(
        // x0
        DISPLAY_POPUP_HORIZ_OFFSET,
        // y0
        calculateMessageBoxOffset(4),
        // Width
        DISPLAY_POPUP_WIDTH,
        // Height
        (4 * TEXT_BOX_HEIGHT) + STD_WORKING_MARGIN,
        // Color
        boxColor);

    tft.setFreeFont(TEXT_CHOSEN_FONT);

    // Set cursor up to top of bounding box
    tft.setCursor(
        // Horizontal offset for characters
        TEXT_BOX_HORIZ_OFFSET,
        // Vertical offset
        calculateVerticalTextOffset(4));

    tft.setTextColor(line1Color);
    tft.println(line1);
    tft.print("    "); // Shift characters from left
    tft.setTextColor(line2Color);
    tft.println(line2);
    tft.print("    "); // Shift characters from left
    tft.setTextColor(line3Color);
    tft.println(line3);
    tft.print("    "); // Shift characters from left
    tft.setTextColor(line4Color);
    tft.println(line4);
}