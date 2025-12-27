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

    Vector2 dim = MeasureTextEx(GetFontDefault(), "0", 10, 0);
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
    
    DrawText(textDisplay, 4, screenHeight - CONTROLS_HEIGHT - TITLEBAR_HEIGHT + (TITLEBAR_HEIGHT - textHeight)/2, 10, titlebar.color);

    // music time
    snprintf(timeBuffer, sizeof(timeBuffer), "%02d:%02d / %02d:%02d", (int)musicTime / 60, (int)musicTime % 60, (int)musicLength / 60, (int)musicLength % 60);

    int width = MeasureText(timeBuffer, 10);
    DrawText(timeBuffer, screenWidth - 4 - width, screenHeight - CONTROLS_HEIGHT - TITLEBAR_HEIGHT + (TITLEBAR_HEIGHT - textHeight)/2, 10, BLACK);
}