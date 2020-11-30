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
#include "icons/TicTacToe.h"

// ===== Games =================================================================
typedef enum { BATTLESHIP_E = 0, TICTACTOE_E = 1, TEST_E = 2 } GAME_E;

// ===== Battleship ============================================================
// Game looping
// Controller
// Display
void battleshipDrawGameIcon(TFT_eSPI tft, int16_t posX, int16_t posY);
// Webserver

// ===== TicTacToe =============================================================
// Game looping
// Controller
// Display
void tictactoeDrawGameIcon(TFT_eSPI tft, int16_t posX, int16_t posY);
// Webserver

// ===== Test ==================================================================
// Game looping
// Controller
// Display
void testDrawGameIcon(TFT_eSPI tft, int16_t posX, int16_t posY);
// Webserver

#endif