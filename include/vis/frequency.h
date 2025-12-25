#include "raylib.h"
#include <math.h>
#include "fft.h"
#include <complex.h>
#include "program.h"

double complex *in = NULL;
int numFrames = 0;

void process_frequency(void *buffer, unsigned int frames) {

    float* samples = (float*)buffer;

    if(frames == 0) {
        in = NULL;
        return;
    }

    // next exponent of 2
    int n = frames;
    int a = log2(n);
    if(pow(2, a) != n) {
        n = pow(2, a+1);
    }

    if(in != NULL) {
        free(in);
        numFrames = 0;
    }
    
    in = malloc(n * sizeof(double complex));
    for(int i = 0; i < frames; ++i) {
        in[i] = samples[i];
    }

    for(int i = frames; i < n; ++i) {
        in[i] = 0;
    }

    fft(in, n);
    numFrames = n;
}

void draw_frequency(int screenWidth, int screenHeight) {

    int barWidth = 2;

    for(int i = 0; i < numFrames/2; ++i) {
        int magnitude = sqrt(creal(in[i]) * creal(in[i]) + cimag(in[i]) * cimag(in[i]));
        int decibels = 20 * log10(magnitude);

        float freq = sampleRate / (float)numFrames * i;

        DrawRectangle(i * barWidth, screenHeight-decibels, barWidth, decibels, BLACK);
    }
}