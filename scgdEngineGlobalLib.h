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

// Display sizing constants
#define DISPLAY_FULL_WIDTH 240
#define DISPLAY_FULL_HEIGHT 320
#define STD_WORKING_MARGIN 10 // Generally, keep a 10px margin
#define DISPLAY_MARGINAL_WIDTH 220
#define DISPLAY_MARGINAL_HEIGHT 300

// ===== Webserver =============================================================
#include <WiFi.h>

#endif