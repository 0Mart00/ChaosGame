#include "overlay.h"
#include "modules.h"

void DrawTechnicalOverlay(int entityCount) {
    // Felső bal sarok, sötétített technikai panel
    DrawRectangle(5, 5, 180, 60, (Color){0, 0, 0, 150});
    DrawRectangleLines(5, 5, 180, 60, MY_CYAN);
    
    DrawFPS(15, 12);
    DrawText(TextFormat("ENTITIES: %d", entityCount), 15, 35, 18, LIME);
}
