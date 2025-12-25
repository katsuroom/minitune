#include "raylib.h"
#include "program.h"
#include "util.h"
#include "titlebar.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_TITLE_LENGTH 40

char titleBuffer[MAX_TITLE_LENGTH+1] = {0};

struct {
    bool* hasMusic;
    char** title;
} titlebar;

int textHeight;

void titlebar_init(bool* hasMusic, char** title) {
    titlebar.hasMusic = hasMusic;
    titlebar.title = title;

    Vector2 dim = MeasureTextEx(GetFontDefault(), "0", 10, 0);
    textHeight = dim.y;
}

char timeBuffer[16] = {0};

void titlebar_update_title(const char* title) {

    int length = strlen(title);
    if(length <= MAX_TITLE_LENGTH) {
        strcpy(titleBuffer, title);
    }
    else {
        strncpy(titleBuffer, title, MAX_TITLE_LENGTH);
        for(int i = 0; i < 3; ++i)
            titleBuffer[MAX_TITLE_LENGTH-i] = '.';
    }

}

void titlebar_draw() {
    // playing text
    DrawRectangle(0, screenHeight - CONTROLS_HEIGHT - TITLEBAR_HEIGHT, screenWidth, TITLEBAR_HEIGHT, (Color){220, 220, 220, 255});
    
    if(*titlebar.hasMusic == true) {
        DrawText(titleBuffer, 4, screenHeight - CONTROLS_HEIGHT - TITLEBAR_HEIGHT + (TITLEBAR_HEIGHT - textHeight)/2, 10, BLUE);
    }
    else {
        DrawText("(No song)", 4, screenHeight - CONTROLS_HEIGHT - TITLEBAR_HEIGHT + (TITLEBAR_HEIGHT - textHeight)/2, 10, GRAY);
    }

    // music time
    sprintf(timeBuffer, "%02d:%02d / %02d:%02d", (int)musicTime / 60, (int)musicTime % 60, (int)musicLength / 60, (int)musicLength % 60);

    int width = MeasureText(timeBuffer, 10);
    DrawText(timeBuffer, screenWidth - 4 - width, screenHeight - CONTROLS_HEIGHT - TITLEBAR_HEIGHT + (TITLEBAR_HEIGHT - textHeight)/2, 10, BLACK);
}