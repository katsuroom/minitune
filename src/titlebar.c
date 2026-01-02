#include "raylib.h"
#include "program.h"
#include "util.h"
#include "titlebar.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char appName[] = "minitune";
char errorMsg[] = "Error: file could not be opened.";

char* textDisplay = appName;

struct {
    Color color;
} titlebar;

int textHeight;

void titlebar_init(void) {
    titlebar.color = GRAY;

    // Vector2 dim = MeasureTextEx(GetFontDefault(), "0", 10, 0);
    Vector2 dim = MeasureTextEx(font, appName, fontSize, 1);
    textHeight = dim.y;
}

char timeBuffer[32] = {0};

void titlebar_update_title(char* title) {
    textDisplay = title;
    titlebar.color = BLUE;
}

void titlebar_set_error(void) {
    textDisplay = errorMsg;
    titlebar.color = RED;
}

void titlebar_draw() {
    // playing text
    DrawRectangle(0, screenHeight - CONTROLS_HEIGHT - TITLEBAR_HEIGHT, screenWidth, TITLEBAR_HEIGHT, (Color){220, 220, 220, 255});
    
    DrawTextEx(font, textDisplay, (Vector2){4, screenHeight - CONTROLS_HEIGHT - TITLEBAR_HEIGHT + (TITLEBAR_HEIGHT - textHeight)/2}, fontSize, 1, titlebar.color);

    // music time
    snprintf(timeBuffer, sizeof(timeBuffer), "%02d:%02d / %02d:%02d", (int)musicTime / 60, (int)musicTime % 60, (int)musicLength / 60, (int)musicLength % 60);

    Vector2 dim = MeasureTextEx(font, timeBuffer, fontSize, 1);
    DrawTextEx(font, timeBuffer, (Vector2){screenWidth - 4 - dim.x, screenHeight - CONTROLS_HEIGHT - TITLEBAR_HEIGHT + (TITLEBAR_HEIGHT - textHeight)/2}, fontSize, 1, BLACK);
}