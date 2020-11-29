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
#define MENU_NUM_OF_GAMES 2  // The current number of games
#define MENU_ICON_SIZE 60    // Size (px) of an icon, height and width
int hasEnteredSelection = 0; // Tracks whether player has stopped viewing menu
int hasChosenGame = 0;       // Tracks whether player has selected game
typedef struct {
    int xPos;   // Pixel X position
    int yPos;   // Pixel Y position
    int row;    // Row (0 indexed)
    int column; // Column (0 indexed)
    int game;   // Current selected game (used for switching)
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
        if (digitalRead(UP_BUTTON)) {
            while (digitalRead(UP_BUTTON))
                ;
            menuMoveCursorRelative(-1);
        }
        if (digitalRead(DOWN_BUTTON)) {
            while (digitalRead(DOWN_BUTTON))
                ;
            menuMoveCursorRelative(1);
        }
        if (digitalRead(LEFT_BUTTON)) {
            while (digitalRead(LEFT_BUTTON))
                ;
            menuMoveCursorRelative(-1);
        }
        if (digitalRead(RIGHT_BUTTON)) {
            while (digitalRead(RIGHT_BUTTON))
                ;
            menuMoveCursorRelative(1);
        }
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
                // TODO: Switch on cursor for choice
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

void menuMoveCursor(int row, int column) {
    // Remove selection box
    menuRemoveSelectionBox();

    // Set row and column values
    menuCursor.row = row;
    menuCursor.column = column;

    menuCursor.xPos = 20 + (row * 70); // 20px margin + 70px over for icons
    menuCursor.yPos = 20 + (column * 70);

    menuCursor.game = (3 * row) + column; // Used for switching

    // Update display on cursor move
    menuDrawSelectionBox();
}

void menuMoveCursorRelative(int position) { // -1 for left, +1 for right
    // Simplified cursor relative movement

    // Remove current selection box
    menuRemoveSelectionBox();

    // Calculate and check bounds
    if (position < 0) menuCursor.game -= 1;
    if (position > 0) menuCursor.game += 1;
    // Scroll back to other titles
    if (menuCursor.game < 0) menuCursor.game = MENU_NUM_OF_GAMES - 1;
    if (menuCursor.game >= MENU_NUM_OF_GAMES) menuCursor.game = 0;

    // Calculate row and column values
    menuCursor.row = menuCursor.game / 3;
    menuCursor.column = menuCursor.game % 3;

    // 20px margin + 70px over for icons
    menuCursor.xPos = 20 + (menuCursor.column * 70);
    menuCursor.yPos = 20 + (menuCursor.row * 70);

    // Update display on cursor move
    menuDrawSelectionBox();
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

void menuDrawGames() {
    // TODO: Handle this calculation programmatically
    battleshipDrawGameIcon(tft, 20, 20);
    testDrawGameIcon(tft, 90, 20);
}

void menuDrawSelectionScreen() {
    tft.fillScreen(TFT_BLACK);
    drawMarginBoundingBox(tft, TFT_WHITE);
    menuDrawGames();

    // Draw cursor
    menuCursor.xPos = 20;
    menuCursor.yPos = 20;
    menuMoveCursor(0, 0); // Start at top left, update cursor
}

// Same as before, but without re-initializing cursor
void menuDrawSelectionScreenAgain() {
    tft.fillScreen(TFT_BLACK);
    drawMarginBoundingBox(tft, TFT_WHITE);
    menuDrawGames();

    // Draw cursor
    menuDrawSelectionBox(); // No need to move cursor, just update visual
}

void menuRemoveSelectionBox() {
    // Replace selection box at cursor position with white box
    tft.drawRect(menuCursor.xPos, menuCursor.yPos, 60, 60, TFT_WHITE);
}

void menuDrawSelectionBox() {
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

// TODO: Create menu