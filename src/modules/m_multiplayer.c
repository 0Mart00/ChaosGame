#include <stdlib.h>         // Az atoi() függvényhez
#include "network_system.h" // A StartServer() és StartClient() függvényekhez
#include "raylib.h"
#include "modules.h"
#include <string.h>

typedef enum {
    MP_STATE_CHOOSE, // Választás: Join vagy Create
    MP_STATE_CREATE, // Paraméterek megadása
    MP_STATE_JOIN,   // Csatlakozás adatai
    MP_STATE_LOBBY   // Várakozás a kezdésre
} MultiState;


typedef enum { NONE, FIELD_IP, FIELD_PORT, FIELD_NAME, FIELD_PASS } ActiveField;
static ActiveField activeField = NONE;
static MultiState subState = MP_STATE_CHOOSE;

// Beviteli mezők tárolói (pufferelése)
char serverName[32] = "Chaos Server";
char ipAddr[20] = "127.0.0.1";
char portNum[12] = "12345";
char playerName[16] = "Player1";
char password[16] = "";
int lobbySize = 8;

static const Color MY_CYAN = { 0, 255, 255, 255 }; 
static const Color MY_GRAY = { 130, 130, 130, 255 };


void DrawInputField(const char* label, char* buffer, int x, int y, int width, bool active) {
    DrawText(label, x, y - 20, 15, MY_GRAY);
    Rectangle field = { (float)x, (float)y, (float)width, 35.0f };
    
    // Háttér és Neon keret
    DrawRectangleRec(field, active ? (Color){25, 25, 25, 255} : (Color){10, 10, 10, 255});
    DrawRectangleLinesEx(field, 1, active ? MY_CYAN : MY_GRAY);
    
    // Szöveg kirajzolása
    DrawText(buffer, x + 10, y + 10, 20, active ? WHITE : LIGHTGRAY);
    
    // Villogó kurzor (Kognitív visszacsatolás)
    if (active && (((int)(GetTime() * 2)) % 2 == 0)) {
        int textWidth = MeasureText(buffer, 20);
        DrawRectangle(x + 12 + textWidth, y + 8, 2, 20, MY_CYAN);
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
            
            // Fókuszkezelés: Kattintás detektálása a mezőkön
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, (Rectangle){220, 180, 360, 35})) activeField = FIELD_NAME;
                else if (CheckCollisionPointRec(mouse, (Rectangle){220, 240, 100, 35})) activeField = FIELD_PORT;
                else if (CheckCollisionPointRec(mouse, (Rectangle){220, 300, 360, 35})) activeField = FIELD_PASS;
                else activeField = NONE;
            }

            // Szövegbevitel irányítása az aktív mezőre (UTF-8 támogatással)
            if (activeField == FIELD_NAME) HandleTextInput(serverName, 31);
            if (activeField == FIELD_PORT) HandleTextInput(portNum, 5);
            if (activeField == FIELD_PASS) HandleTextInput(password, 15);

            // Mezők kirajzolása az aktuális tartalommal és aktív állapottal
            DrawInputField("SERVER NAME", serverName, 220, 180, 360, (activeField == FIELD_NAME));
            DrawInputField("PORT", portNum, 220, 240, 100, (activeField == FIELD_PORT));
            DrawInputField("PASSWORD (OPTIONAL)", password, 220, 300, 360, (activeField == FIELD_PASS));
            
            DrawText(TextFormat("MAX PLAYERS: %d", lobbySize), 220, 350, 20, MY_GRAY);
            
            if (DrawButton("LAUNCH", 300, 400)) {
                if (StartServer(atoi(portNum), lobbySize)) {
                    subState = MP_STATE_LOBBY;
                }
            }
            if (DrawButton("CANCEL", 300, 470)) {
                activeField = NONE; // Kilépéskor ürítjük a fókuszt
                subState = MP_STATE_CHOOSE;
            }
            break;

        case MP_STATE_JOIN:
            DrawText("JOIN SESSION", 280, 130, 25, LIME);
            
           if (subState == MP_STATE_JOIN) {
            // Mezők kijelölése kattintásra
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, (Rectangle){220, 180, 360, 35})) activeField = FIELD_IP;
                else if (CheckCollisionPointRec(mouse, (Rectangle){220, 240, 150, 35})) activeField = FIELD_PORT;
                else if (CheckCollisionPointRec(mouse, (Rectangle){220, 300, 360, 35})) activeField = FIELD_NAME;
                else activeField = NONE;
            }

            // Bevitel irányítása az aktív mezőre
            if (activeField == FIELD_IP) HandleTextInput(ipAddr, 15);
            if (activeField == FIELD_PORT) HandleTextInput(portNum, 5);
            if (activeField == FIELD_NAME) HandleTextInput(playerName, 15);

            // Megjelenítés (átadjuk az 'active' állapotot a keret színezéséhez)
            DrawInputField("IP ADDRESS", ipAddr, 220, 180, 360, (activeField == FIELD_IP));
            DrawInputField("PORT", portNum, 220, 240, 150, (activeField == FIELD_PORT));
            DrawInputField("PLAYER NAME", playerName, 220, 300, 360, (activeField == FIELD_NAME));            
            if (DrawButton("CONNECT", 300, 400)) {
                if (StartClient(ipAddr, atoi(portNum))) {
                    *currentState = STATE_GAMEPLAY;
                };
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
}
