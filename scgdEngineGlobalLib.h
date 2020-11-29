////////////////////////////////////////////////////////////////////////////////
// Module Name: SCGDEngineGlobalLib.h
//
// Description: Headerfile containing global data for all modules
////////////////////////////////////////////////////////////////////////////////

#ifndef SCGDENGINEGLOBALLIB
#define SCGDENGINEGLOBALLIB

#include "src/FreeFonts.h" // Adding fonts for display output

// ===== Controller ============================================================
// Controller/circuit button pin configuration
#define UP_BUTTON 27
#define DOWN_BUTTON 13
#define LEFT_BUTTON 14
#define RIGHT_BUTTON 12
#define B_BUTTON 26
#define A_BUTTON 25

// ===== Graphics ==============================================================
// Imports
#include <SPI.h>      // Serial Peripheral Interface main library
#include <TFT_eSPI.h> // Graphics/fonts library for ILI9341 driver

// Display Function Prototypes
void drawMarginBoundingBox(TFT_eSPI tft, uint16_t color);
void drawMessageBoxOneLine(TFT_eSPI tft, uint16_t boxColor, char *buffer,
                           uint16_t textColor);

// Display sizing constants
#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 320
#define STD_WORKING_MARGIN 10 // Generally, keep a 10px margin
#define DISPLAY_POPUP_HORIZ_OFFSET 40
#define DISPLAY_POPUP_WIDTH 160

// Text sizing constants
#define TEXT_BOX_HEIGHT 18  // Full size of a line in println() calls
#define TEXT_BOX_BASELINE 6 // The println cursor is 6px up from bottom
#define TEXT_CHOSEN_FONT &FreeMono9pt7b
#define TEXT_BOX_CHAR_WIDTH 14   // Can only fit 14 characters per line in popup
#define TEXT_BOX_HORIZ_OFFSET 43 // Comfortable text positioning offset

// ===== Webserver =============================================================
#include <WiFi.h>

#endif