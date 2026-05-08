#include "raylib.h"
#include "modules.h"
#include <string.h>

typedef enum {
    MP_STATE_CHOOSE, // Választás: Join vagy Create
    MP_STATE_CREATE, // Paraméterek megadása
    MP_STATE_JOIN,   // Csatlakozás adatai
    MP_STATE_LOBBY   // Várakozás a kezdésre
} MultiState;

static MultiState subState = MP_STATE_CHOOSE;

// Beviteli mezők tárolói (pufferelése)
char serverName[32] = "Chaos Server";
char ipAddr[16] = "127.0.0.1";
char portNum[6] = "12345";
char playerName[16] = "Player1";
char password[16] = "";
int lobbySize = 8;

static const Color MY_CYAN = { 0, 255, 255, 255 }; 
static const Color MY_GRAY = { 130, 130, 130, 255 };


// Segédfüggvény egy stílusos beviteli mezőhöz
void DrawInputField(const char* label, char* buffer, int x, int y, int width, bool active) {
    DrawText(label, x, y - 20, 15, MY_GRAY);
    Rectangle field = { (float)x, (float)y, (float)width, 35 };
    DrawRectangleRec(field, active ? (Color){30, 30, 30, 255} : (Color){15, 15, 15, 255});
    DrawRectangleLinesEx(field, 1, active ? MY_CYAN : DARKGRAY);
    DrawText(buffer, x + 10, y + 10, 20, active ? WHITE : LIGHTGRAY);
    if (active && (GetTime() * 2) > (int)(GetTime() * 2)) { // Villogó kurzor
        int tw = MeasureText(buffer, 20);
        DrawLine(x + 15 + tw, y + 8, x + 15 + tw, y + 27, MY_CYAN);
    }
}

void Module_Multiplayer_Draw(GameState *currentState) {
    // Háttér sötétítése
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 200});
    
    Rectangle windowArea = { 200, 100, 400, 450 };
    DrawRectangleRec(windowArea, (Color){10, 10, 10, 255});
    DrawRectangleLinesEx(windowArea, 2, MY_CYAN);

    switch (subState) {
        case MP_STATE_CHOOSE:
            DrawText("MULTIPLAYER", 280, 130, 30, RAYWHITE);
            if (DrawButton("CREATE SERVER", 300, 220)) subState = MP_STATE_CREATE;
            if (DrawButton("JOIN SERVER", 300, 300)) subState = MP_STATE_JOIN;
            if (DrawButton("BACK", 300, 450)) *currentState = STATE_MAIN_MENU;
            break;

        case MP_STATE_CREATE:
            DrawText("CREATE SESSION", 280, 130, 25, MY_CYAN);
           
            DrawInputField("SERVER NAME", serverName, 220, 180, 360, false);
            DrawInputField("PORT", portNum, 220, 240, 100, false);
            DrawInputField("PASSWORD (OPTIONAL)", password, 220, 300, 360, false);
            DrawText(TextFormat("MAX PLAYERS: %d", lobbySize), 220, 350, 20, MY_GRAY);
            
            if (DrawButton("LAUNCH", 300, 400)) {
                // Itt indulna az ENet szerver inicializálása
                subState = MP_STATE_LOBBY;
            }
            if (DrawButton("CANCEL", 300, 470)) subState = MP_STATE_CHOOSE;
            break;

        case MP_STATE_JOIN:
            DrawText("JOIN SESSION", 280, 130, 25, LIME);
            
            DrawInputField("IP ADDRESS", ipAddr, 220, 180, 360, true);
            DrawInputField("PORT", portNum, 220, 240, 150, false);
            DrawInputField("PLAYER NAME", playerName, 220, 300, 360, false);
            
            if (DrawButton("CONNECT", 300, 400)) {
                // Itt indulna az ENet kliens csatlakozása
                *currentState = STATE_GAMEPLAY;
            }
            if (DrawButton("CANCEL", 300, 470)) subState = MP_STATE_CHOOSE;
            break;

        case MP_STATE_LOBBY:
            DrawText("WAITING FOR PLAYERS...", 250, 200, 20, GOLD);
            DrawText(TextFormat("Server: %s", serverName), 220, 250, 15, MY_GRAY);
            if (DrawButton("START GAME", 300, 400)) *currentState = STATE_GAMEPLAY;
            if (DrawButton("ABANDON", 300, 470)) subState = MP_STATE_CHOOSE;
            break;
    }
}
