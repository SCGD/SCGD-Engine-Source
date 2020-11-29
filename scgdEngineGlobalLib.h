////////////////////////////////////////////////////////////////////////////////
// Module Name: SCGDEngineGlobalLib.h
//
// Description: Headerfile containing global display data for all games
////////////////////////////////////////////////////////////////////////////////

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
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip

// Library instance
TFT_eSPI tft = TFT_eSPI();

// Display sizing constants
#define DISPLAY_FULL_WIDTH 240
#define DISPLAY_FULL_HEIGHT 320
#define STD_WORKING_MARGIN 10 // Generally, keep a 10px margin
#define DISPLAY_MARGINAL_WIDTH 220
#define DISPLAY_MARGINAL_HEIGHT 300

// ===== Webserver =============================================================
#include <WiFi.h>

// Webserver definition
WiFiServer server(80); // TCP port 80 (reserved for HTTP protocol)