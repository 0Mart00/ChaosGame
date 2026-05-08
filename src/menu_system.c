#include "raylib.h"     // Ez kötelező a színekhez (CYAN, GRAY, BLACK, stb.)
#include "modules.h"    // A DrawButton deklarációja miatt
#include <stdbool.h>
#include <string.h>


bool DrawButton(Rectangle bounds, const char* text) {
    Vector2 mousePoint = GetMousePosition();
    bool hovering = CheckCollisionPointRec(mousePoint, bounds);
    bool clicked = false;

    if (hovering) {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            clicked = true;
        }
    }

    // Rajzolás
    DrawRectangleRec(bounds, hovering ? SKYBLUE : DARKGRAY);
    DrawRectangleLinesEx(bounds, 2.0f, WHITE);
    
    // Szöveg középre igazítása (reszponzív módon)
    int fontSize = bounds.height > 40 ? 20 : 10;
    int textWidth = MeasureText(text, fontSize);


    // Kiszámoljuk a középpontot
    float textX = bounds.x + (bounds.width / 2.0f) - (textWidth / 2.0f);
    float textY = bounds.y + (bounds.height / 2.0f) - (fontSize / 2.0f);

    DrawText(text, (int)textX, (int)textY, fontSize, WHITE);

    return clicked;
}

// Kezeli a billentyűzet bevitelt egy adott pufferbe
void HandleTextInput(char* buffer, int maxSize) {
    int key = GetCharPressed();

    while (key > 0) {
        // Csak a látható karaktereket engedjük (UTF-8 támogatással)
        if ((key >= 32) && (strlen(buffer) < maxSize )) {
            int byteSize = 0;
            const char* utf8Char = CodepointToUTF8(key, &byteSize);
            
            int len = strlen(buffer);
            for (int i = 0; i < byteSize; i++) {
                buffer[len + i] = utf8Char[i];
            }
            buffer[len + byteSize] = '\0';
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        int len = strlen(buffer);
        if (len > 0) {
            // UTF-8 biztonságos törlés (visszalépés a legutolsó kezdőbájtra)
            do {
                len--;
            } while (len > 0 && (buffer[len] & 0xC0) == 0x80);
            buffer[len] = '\0';
        }
    }
}
