#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "program.h"
#include "titlebar.h"
#include "seeker.h"
#include <stdio.h>
#include <time.h>

int screenWidth = 320;
int screenHeight = 240;
bool isRunning = true;

Vector2 mousePos = {0};

Vector2 mouseOffset = {0};
bool isDragging = false;

int main(void) {
    
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST);
    InitWindow(screenWidth, screenHeight, "raylib");
    GuiLoadStyleDefault();

    InitAudioDevice();
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight-CONTROLS_HEIGHT-TITLEBAR_HEIGHT);

    srand(time(NULL));
    init();

    while(!WindowShouldClose() && isRunning == true) {

        // drag window
        mousePos = GetMousePosition();

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            CheckCollisionPointRec(mousePos, (Rectangle){0, 0, screenWidth, screenHeight-CONTROLS_HEIGHT})) {
            mouseOffset = mousePos;
            isDragging = true;
        }

        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            isDragging = false;
        }

        if(isDragging == true) {
            int dx = mousePos.x - mouseOffset.x;
            int dy = mousePos.y - mouseOffset.y;

            Vector2 wp = GetWindowPosition();
            SetWindowPosition(wp.x + dx, wp.y + dy);
        }

        update();

        BeginTextureMode(target);
        ClearBackground(BLANK);
        draw_visualization();
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLANK);
        // ClearBackground(BLACK);
        draw();
        DrawTextureRec(target.texture,
            (Rectangle){0, 0, screenWidth, -(screenHeight-CONTROLS_HEIGHT-TITLEBAR_HEIGHT)},
            (Vector2){0, 0}, WHITE
        );
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}