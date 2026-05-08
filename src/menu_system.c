#include "raylib.h"     // Ez kötelező a színekhez (CYAN, GRAY, BLACK, stb.)
#include "modules.h"    // A DrawButton deklarációja miatt
#include <stdbool.h>



bool DrawButton(const char* text, int x, int y) {
    static const Color MY_CYAN = { 0, 255, 255, 255 }; 
    static const Color MY_GRAY = { 130, 130, 130, 255 };


    // A Raylib Rectangle float típusokat vár
    Rectangle rect = { (float)x, (float)y, 200.0f, 50.0f };
    
    // Egér ütközés vizsgálata
    bool hover = CheckCollisionPointRec(GetMousePosition(), rect);
    
    // Háttér rajzolása: ha rajta van az egér, sötétszürke, egyébként fekete
    DrawRectangleRec(rect, hover ? DARKGRAY : BLACK);
   
    // Keret rajzolása: ha rajta van az egér, ciánkék, egyébként szürke
    DrawRectangleLinesEx(rect, 2.0f, hover ? MY_CYAN : MY_GRAY);
    
    // Szöveg kirajzolása a gombon belül
    DrawText(text, x + 20, y + 15, 20, hover ? WHITE : LIGHTGRAY);
    
    // Visszatérünk igazzal, ha az egér a gombon van ÉS megnyomták a bal gombot
    return hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}
