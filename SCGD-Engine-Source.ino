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
#define MENU_NUM_OF_GAMES 3  // The current number of games
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
unsigned long currentTime = millis(); // Current time (unix epoch)
unsigned long previousTime = 0;       // Used for timeout calculation
String header;                        // Stores the request

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
    connectWiFi();

    menuWelcomePlayer();
}

// ===== Menu looping ==========================================================

void loop() {
    // Send WiFi data
    menuConnectWebserver(); // Earlier call due to menu event loop shenanigans
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
        delay(150);

        menuDrawSelectionScreen();
        hasEnteredSelection = 1; // Track that we've seen the label
    }
}

void handleMenuSelect() {
    while (true) {
        if (digitalRead(UP_BUTTON)) {
            while (digitalRead(UP_BUTTON))
                ;
            // TODO: Add debouncing delay
            delay(150);
            menuMoveCursorRelative(-1);
        }
        if (digitalRead(DOWN_BUTTON)) {
            while (digitalRead(DOWN_BUTTON))
                ;
            delay(150);
            menuMoveCursorRelative(1);
        }
        if (digitalRead(LEFT_BUTTON)) {
            while (digitalRead(LEFT_BUTTON))
                ;
            delay(150);
            menuMoveCursorRelative(-1);
        }
        if (digitalRead(RIGHT_BUTTON)) {
            while (digitalRead(RIGHT_BUTTON))
                ;
            delay(150);
            menuMoveCursorRelative(1);
        }
        if (digitalRead(B_BUTTON)) {
            while (digitalRead(B_BUTTON))
                ;
            delay(150);

            if (hasChosenGame) {
                hasChosenGame = 0;
                menuDrawSelectionScreenAgain();
            }
        }
        if (digitalRead(A_BUTTON)) {
            while (digitalRead(A_BUTTON))
                ;
            delay(150);

            if (!hasChosenGame) {
                switch (menuCursor.game) {
                    case BATTLESHIP_E:
                        drawMessageBoxTwoLines(tft, TFT_WHITE,
                                               // Line 1
                                               "  BATTLESHIP  ", TFT_WHITE,
                                               // Line 2
                                               "   (A) Play   ", TFT_YELLOW);
                        break;
                    case TICTACTOE_E:
                        drawMessageBoxTwoLines(tft, TFT_WHITE,
                                               // Line 1
                                               "  TIC-TAC-TOE ", TFT_WHITE,
                                               // Line 2
                                               "   (A) Play   ", TFT_YELLOW);
                        break;
                    case TEST_E:
                        drawMessageBoxTwoLines(tft, TFT_WHITE,
                                               // Line 1
                                               "     TEST     ", TFT_WHITE,
                                               // Line 2
                                               "   (A) Play   ", TFT_YELLOW);
                        break;
                }

                hasChosenGame = 1;
            } else { // Player has chosen and confirmed a game
                tft.fillScreen(TFT_BLACK);
                switch (menuCursor.game) {
                    case BATTLESHIP_E:
                        battleshipPlayGame(tft);
                        break;
                    case TICTACTOE_E:
                        tictactoePlayGame(tft);
                        break;
                    case TEST_E:
                        // TODO: Test
                        break;
                }

                hasChosenGame = 0; // Prevents issues with double pressing
                menuDrawSelectionScreenAgain();
            }
        }

        // Send WiFi data
        menuConnectWebserver();
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

    // Bounds checking
    if (position > 0 && menuCursor.game < MENU_NUM_OF_GAMES - 1) { // Right
        menuCursor.game += 1;
    }
    if (position < 0 && menuCursor.game > 0) { // Left
        menuCursor.game -= 1;
    }

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
    tictactoeDrawGameIcon(tft, 90, 20);
    testDrawGameIcon(tft, 160, 20);
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

void menuDisplayIP(IPAddress ip) { tft.println(ip.toString()); }

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

    menuDisplayIP(WiFi.localIP());
}

void menuConnectWebserver() {
    WiFiClient menuClient = server.available(); // Listen for incoming clients

    if (menuClient) {
        // Create timeout parameters
        currentTime = millis();
        previousTime = currentTime;

        Serial.println("New client.");
        String currentLine = ""; // Stores incoming client data

        // Loop while client is connected
        while (menuClient.connected() &&
               currentTime - previousTime <= TIMEOUT_TIME) {
            currentTime = millis(); // Log time

            if (menuClient.available()) {
                char c = menuClient.read(); // Read a byte from client
                Serial.write(c);            // Write on serial out
                header += c;
                if (c == '\n') { // On newline
                    // Signal for HTTP request end is \n\n, or \n and empty line
                    if (currentLine.length() == 0) {
                        // Create response header
                        menuClient.println("HTTP/1.1 200 OK");
                        menuClient.println("Content-type:text/html");
                        menuClient.println("Connection: close");
                        menuClient.println();

                        // Display HTML webpage
                        menuClient.println(
                            "<!DOCTYPE html><html> <head> "
                            "<style>body{font-family: monospace; text-align: "
                            "center; align-items: center;}h1{font-size: "
                            "5rem;}p{font-size: 2rem;}i{font-size: 1rem; "
                            "color: blue;}.lds-dual-ring{display: "
                            "inline-block; width: 80px; height: "
                            "80px;}.lds-dual-ring:after{content: \" \"; "
                            "display: block; width: 64px; height: 64px; "
                            "margin: 8px; border-radius: 50%; border: 6px "
                            "solid #000; border-color: #000 transparent #000 "
                            "transparent; animation: lds-dual-ring 1.2s linear "
                            "infinite;}@keyframes lds-dual-ring{0%{transform: "
                            "rotate(0deg);}100%{transform: "
                            "rotate(360deg);}}</style> </head> <body> "
                            "<h1>Please wait!</h1> <div "
                            "class=\"lds-dual-ring\"></div><p>Player is "
                            "currently selecting a "
                            "game.</p><p><i>State College Game Engine - Micro "
                            "Game Engine Platform</i></p></body></html>");

                        // End HTTP response
                        menuClient.println();
                        break;
                    } else {
                        currentLine = "";
                    }
                } else if (c != '\r') {
                    // Add all input excluding carriage return to currentLine
                    currentLine += c;
                }
            }
        }
        header = ""; // Reset header

        // Close connection after client connection end
        menuClient.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }
}