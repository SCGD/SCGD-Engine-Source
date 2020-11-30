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
// Initialization
void battleshipPlayGame(TFT_eSPI tft);
// Game looping
void battleshipEventLoop(TFT_eSPI tft);
// Controller
int battleshipPlayPlayerTurn(TFT_eSPI tft);
void battleshipResetPlayerShots();
// Display
void battleshipDrawGameIcon(TFT_eSPI tft, int16_t posX, int16_t posY);
void battleshipDrawStartGamePopup(TFT_eSPI tft);
void battleshipDrawInitializeGameBoard(TFT_eSPI tft);
void battleshipMoveCursor(TFT_eSPI tft, int row, int column);
void battleshipUpdateScreen(TFT_eSPI tft, int score);

// ===== TicTacToe =============================================================
// Initialization
void tictactoePlayGame(TFT_eSPI tft, WiFiServer server);
// Game looping
void tictactoeEventLoop(TFT_eSPI tft, WiFiServer server);
// Controller
void tictactoePlayPlayerTurn(TFT_eSPI tft);
void tictactoePlayCPUTurn(TFT_eSPI tft);
void tictactoeResetGrid();
int tictactoeFilled();
int tictactoeWinner();
int horizontalWin();
int verticalWin();
int diagonalWin();
// Display
void tictactoeDrawGameIcon(TFT_eSPI tft, int16_t posX, int16_t posY);
void tictactoeStartDisplay(TFT_eSPI tft);
void tictactoeDrawGameGrid(TFT_eSPI tft);
void tictactoeDisplayUserPlay(TFT_eSPI tft);
void tictactoeDisplayCPUPlay(TFT_eSPI tft);
void tictactoeDrawCursor(TFT_eSPI tft);
void tictactoeEraseCursor(TFT_eSPI tft);
// Webserver
void tictactoeConnectWebserver(WiFiServer server);

// ===== Test ==================================================================
void testDrawGameIcon(TFT_eSPI tft, int16_t posX, int16_t posY);

#endif