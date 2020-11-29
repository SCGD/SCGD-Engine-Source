////////////////////////////////////////////////////////////////////////////////
// Module Name: SCGD-Engine-Source
//
// Description: Contains functionality for menu systems, as well as handling for
//              all individual games
////////////////////////////////////////////////////////////////////////////////

// ===== Imports and Globals ===================================================
// Headerfile imports
#include "SCGDEngineGlobalLib.h"
#include "SCGDNetworkData.h"
#include "src/SCGDEngineGamesList.h" // Contains game prototypes

// Global variables
#define MENU_NUM_OF_GAMES 1  // The current number of games
#define MENU_ICON_SIZE 60    // Size (px) of an icon, height and width
int hasEnteredSelection = 0; // Tracks whether player has stopped viewing menu
int hasChosenGame = 0;       // Tracks whether player has selected game
typedef struct {
    int xPos; // Pixel X position
    int yPos; // Pixel Y position
    int game; // Game number (from 0 through 8)
} menu_cursor_t;
menu_cursor_t menuCursor; // Tracks current selected game (supports max 9)

// ===== Initialization ========================================================

// Creating object instances
TFT_eSPI tft = TFT_eSPI(); // Display library
WiFiServer server(80);     // TCP port 80 (reserved for HTTP protocol)

void setup() {
    // Set serial output baud rate
    Serial.begin(115200);

    Serial.println("Beginning setup");
    setPinModes();
    initializeDisplay();
    // connectWifi();

    menuWelcomePlayer();
}

// ===== Menu looping ==========================================================

void loop() {
    // First, handle the popup
    if (!hasEnteredSelection) {
        handlePopupClose();
        return; // Continue to next iteration of loop()
    }

    handleMenuSelect();
}

// ===== Controller ============================================================

void setPinModes() {
    Serial.println("Setting input pins on controller");
    pinMode(UP_BUTTON, INPUT);
    pinMode(DOWN_BUTTON, INPUT);
    pinMode(LEFT_BUTTON, INPUT);
    pinMode(RIGHT_BUTTON, INPUT);
    pinMode(B_BUTTON, INPUT);
    pinMode(A_BUTTON, INPUT);
}

void handlePopupClose() {
    if (digitalRead(A_BUTTON)) {
        // Wait for unpress
        while (digitalRead(A_BUTTON))
            ;

        menuDrawSelectionScreen();
        hasEnteredSelection = 1; // Track that we've seen the label
    }
}

void handleMenuSelect() {
    while (true) {
        // TODO: Implement selection box moving around
        // ...
        if (digitalRead(B_BUTTON)) {
            while (digitalRead(B_BUTTON))
                ;

            if (hasChosenGame) {
                hasChosenGame = 0;
                menuDrawSelectionScreenAgain();
            }
        }
        if (digitalRead(A_BUTTON)) {
            while (digitalRead(A_BUTTON))
                ;

            if (!hasChosenGame) {
                drawMessageBoxTwoLines(tft, TFT_WHITE,
                                       // Line 1
                                       "  BATTLESHIP  ", TFT_WHITE,
                                       // Line 2
                                       "   (A) Play   ", TFT_YELLOW);
                hasChosenGame = 1;
            } else {
                // TODO: Play battleship
                tft.fillScreen(TFT_BLACK);
            }
        }
    }
}

// ===== Display ===============================================================

void initializeDisplay() {
    Serial.println("Initializing display");
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
}

void menuWelcomePlayer() {
    drawMessageBoxThreeLines(tft, TFT_WHITE,
                             // Line 1
                             "   WELCOME!   ", TFT_WHITE,
                             // Line 2
                             "Select a game.", TFT_WHITE,
                             // Line 3
                             "  (Press A!)  ", TFT_YELLOW);
}

void menuDrawSelectionScreen() {
    tft.fillScreen(TFT_BLACK);
    drawMarginBoundingBox(tft, TFT_WHITE);

    // Draw individual games
    battleshipDrawGameIcon(tft, 20, 20);

    // Draw cursor
    menuCursor.game = 0; // Start at top left, on battleship
    menuDrawSelectionBox(0);
}

// Same as before, but without re-initializing cursor
void menuDrawSelectionScreenAgain() {
    tft.fillScreen(TFT_BLACK);
    drawMarginBoundingBox(tft, TFT_WHITE);

    // Draw individual games
    battleshipDrawGameIcon(tft, 20, 20);

    // Draw cursor
    menuDrawSelectionBox(menuCursor.game);
}

menu_cursor_t menuRecalcCursor(int game) {
    // Create a new cursor structure
    menu_cursor_t cursor;
    cursor.game = game;
    // Calculate X position - Adding 2 margins and icon sizing up to 3x
    cursor.xPos = 20 + (70 * (game % 3));
    // Calculate Y position - 2 margins and icon sizing up to 3x
    cursor.yPos = 20 + (70 * (game / 3));

    return cursor;
}

void menuDrawSelectionBox(int newGame) {
    menuCursor = menuRecalcCursor(menuCursor.game); // Recalculate cursor values
    // Replace selection box at cursor position with white box
    tft.drawRect(menuCursor.xPos, menuCursor.yPos, 60, 60, TFT_WHITE);

    menuCursor = menuRecalcCursor(newGame); // Create new cursor
    // Draw a new selection box at the new position
    tft.drawRect(menuCursor.xPos, menuCursor.yPos, 60, 60, TFT_YELLOW);
}

// ===== Webserver =============================================================

void connectWiFi() {
    Serial.print("Connecting to:");
    Serial.println(NETWORK_SSID);
    WiFi.begin(NETWORK_SSID, NETWORK_PASSWORD);

    // Wait until connected
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    // Display WiFi information
    Serial.println("\nWiFi connected\nIP address: ");
    Serial.print(WiFi.localIP());

    // Start webserver
    server.begin();
}