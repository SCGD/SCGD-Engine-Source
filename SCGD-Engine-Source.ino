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
#define NUM_OF_GAMES = 1
int hasEnteredSelection = 0; // Tracks whether player has stopped viewing menu

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
    // First, handle the
    if (!hasEnteredSelection) {
        handlePopupClose();
        return; // Continue to next iteration of loop()
    }
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
    drawMarginBoundingBox();

    // Iterate through and draw our individual games
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