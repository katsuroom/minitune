#include "raylib.h"
#include "util.h"
#include "program.h"

#define BAR_HEIGHT 10

// seeker ui element
struct {
    int x, y, width, height;
} sRect;

// seeker bar
struct {
    int x, y, width, height, padding;
} sBar;

// seeker dot
struct {
    int x, y;
} sDot;

int offset = 12;
int seekerRadius = 4;

static bool isDragging = false;

void seeker_init(int x, int y, int width, int height) {
    sRect.x = x;
    sRect.y = y;
    sRect.width = width;
    sRect.height = height;

    sBar.width = sRect.width - 2*offset; 
    sBar.height = 6;
    sBar.x = sRect.x + offset;
    sBar.y = sRect.y + (sRect.height - sBar.height)/2;
    sBar.padding = 4;

    sDot.y = sRect.y + sRect.height/2;
}

void seeker_update(void) {

    if(hasMusic && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // if clicked sBar
        if(CheckCollisionPointRec(mousePos, (Rectangle){sBar.x-sBar.padding, sBar.y-sBar.padding, sBar.width+2*sBar.padding, sBar.height+2*sBar.padding})) {
            isDragging = true;
        }
    }
    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if(isDragging == true) {
            // update song position
            float pos = (float)(sDot.x - sBar.x) / sBar.width;

            set_song_position(pos * musicLength);
        }
        isDragging = false;
    }

    if(isDragging) {
        sDot.x = mousePos.x;

        // bounds checking
        if(sDot.x < sBar.x)
            sDot.x = sBar.x;
        else if(sDot.x > sBar.x + sBar.width)
            sDot.x = sBar.x + sBar.width;
    }

    // update sDot based on current music progress
    else {
        double ratio = 0;
        if(musicLength > 0)
            ratio = musicTime / musicLength;
        sDot.x = sBar.x + ratio * sBar.width;
    }
}

void seeker_draw(void) {
    DrawRectangle(sRect.x, sRect.y, sRect.width, sRect.height, LIGHTGRAY);
    DrawRectangleLinesEx((Rectangle){sRect.x, sRect.y, sRect.width, sRect.height}, 2, GRAY);

    DrawRectangle(sBar.x, sBar.y, sBar.width, sBar.height, GRAY);
    DrawRectangle(sBar.x, sBar.y, sDot.x - sBar.x, sBar.height, (Color){180, 60, 120, 255});
    
    DrawCircle(sDot.x, sDot.y, seekerRadius, BLACK);
}