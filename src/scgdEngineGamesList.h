////////////////////////////////////////////////////////////////////////////////
// Module Name: scgdEngineGamesList.h
//
// Description: Headerfile containing imports for individual games
////////////////////////////////////////////////////////////////////////////////

#ifndef SCGDENGINEGAMESLIST
#define SCGDENGINEGAMESLIST

// ===== Icons =================================================================
#include "icons/Battleship.h"
#include "icons/Test.h"

// ===== Battleship ============================================================
// Game looping
// Controller
// Display
void battleshipDrawGameIcon(TFT_eSPI tft, int16_t posX, int16_t posY);
void testDrawGameIcon(TFT_eSPI tft, int16_t posX, int16_t posY);
// Webserver

#endif