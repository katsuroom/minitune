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
char timeBuffer[32] = {0};

struct {
    Color color;
} titlebar;

int xPad = 6;

int titleMaxWidth = 0;

void titlebar_init(void) {
    titlebar.color = GRAY;

    Vector2 timerDim = MeasureTextEx(font, "00:00 / 00:00", fontSize, fontSpacing);
    titleMaxWidth = screenWidth-timerDim.x-20-xPad*2;
}

void titlebar_update_title(char* title) {
    textDisplay = title;
    titlebar.color = BLUE;
}

void titlebar_set_error(void) {
    textDisplay = errorMsg;
    titlebar.color = RED;
}

void titlebar_draw() {
    int yPos = screenHeight - CONTROLS_HEIGHT - TITLEBAR_HEIGHT;
    DrawRectangle(0, yPos, screenWidth, TITLEBAR_HEIGHT, (Color){220, 220, 220, 255});

    // music time
    snprintf(timeBuffer, sizeof(timeBuffer), "%02d:%02d / %02d:%02d", (int)musicTime / 60, (int)musicTime % 60, (int)musicLength / 60, (int)musicLength % 60);

    Vector2 dim = MeasureTextEx(font, timeBuffer, fontSize, fontSpacing);
    DrawTextEx(font, timeBuffer, (Vector2){screenWidth - xPad - dim.x, yPos + (TITLEBAR_HEIGHT - dim.y)/2}, fontSize, fontSpacing, BLACK);

    // playing text
    // DrawRectangle(xPad, yPos, titleMaxWidth, TITLEBAR_HEIGHT, DARKGRAY);
    DrawTextEx(font, textDisplay, (Vector2){xPad, yPos + (TITLEBAR_HEIGHT - dim.y)/2}, fontSize, fontSpacing, titlebar.color);
}