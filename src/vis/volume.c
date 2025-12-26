#include "raylib.h"
#include <math.h>

#define DB_MINIMUM -40      // minimum displayed dB volume

static double volume = DB_MINIMUM;

void process_volume(void *buffer, unsigned int frames) {

    float* samples = (float*)buffer;

    // root mean square of audio data
    double rms = 0;

    for(int i = 0; i < frames; ++i) {
        rms += samples[i] * samples[i];
    }

    rms = sqrt(rms / frames);

    // convert to decibels
    volume = (rms == 0) ? DB_MINIMUM : 20 * log10(rms);
}

void draw_volume(int screenWidth, int screenHeight) {
    // int rectHeight = (1-(volume/DB_MINIMUM)) * screenHeight;
    // DrawRectangle(0, screenHeight-rectHeight, 20, rectHeight, (Color){255, 0, 0, 255});

    int rectWidth = (1-(volume/DB_MINIMUM)) * screenWidth;
    DrawRectangle(0, screenHeight - 20, rectWidth, 20, (Color){255, 0, 0, 255});

    // DrawCircleLines(screenWidth/2, screenHeight/2, (1-(volume/DB_MINIMUM)) * screenHeight/2, GRAY);
}