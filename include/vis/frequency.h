#include "raylib.h"
#include "fft.h"
#include "program.h"
#include <math.h>
#include <complex.h>
#include <float.h>

#define NUM_BANDS (59+1)
#define BAND_INTERVAL 0.16

// bands 0 - 10
double bands[NUM_BANDS] = {0};

void process_frequency(void *buffer, unsigned int frames) {

    if(isPlaying == false || frames == 0)
        return;

    float* samples = (float*)buffer;

    // subtract samples[i] by mean
    double mean = 0;
    for(int i = 0; i < frames; ++i) {
        mean += samples[i];
    }
    mean /= frames;

    // next exponent of 2
    int n = frames;
    int a = log2(n);
    if(pow(2, a) != n) {
        n = pow(2, a+1);
    }
    int numFrames = n;

    double complex in[numFrames];
    
    for(int i = 0; i < frames; ++i) {
        in[i] = samples[i] - mean;  // subtract mean here
    }

    // pad remaining with 0
    for(int i = frames; i < n; ++i) {
        in[i] = 0;
    }

    // compute fft
    fft(in, n);
    
    // compute bands
    double bin_size = (double)sampleRate / numFrames;

    for(int i = 0; i < NUM_BANDS; ++i) {
        // exp = [4, BAND_INTERVAL*(NUM_BANDS-1)]
        double exp = 4 + i*BAND_INTERVAL;
        double target_freq = pow(2, exp);

        // skip bin 0
        int bin_idx = i+1;

        // if target_freq < frequency_bin[i], use index i+1
        // if target_freq is larger, jump to target frequency bin
        if(target_freq > bin_idx*bin_size) {
            bin_idx = target_freq / bin_size;
        }

        // bin index too high
        if(bin_idx > numFrames-1) {
            bin_idx = numFrames-1;
        }

        double complex val = in[bin_idx];

        // actual frequency
        double freq = bin_idx * bin_size;

        double magnitude = sqrt(creal(val) * creal(val) + cimag(val) * cimag(val));

        // divide by number of spanned semitones
        int semitones = 12 * log2(1 + (bin_size / freq));
        magnitude /= fmax(semitones, 1);

        double decibels = 20 * log10(magnitude);

        double delta = fabs(decibels - bands[i]);
        if(decibels < bands[i]) {
            // fall by 1 dB
            bands[i] -= fmin(1, delta);
        }
        else {
            // rise by delta/2 or 1 dB
            if(delta/2 < 1) {
                bands[i] = decibels;
            }
            else {
                bands[i] += fmax(delta / 2, 1);
            }
        }
    }
}

void draw_frequency(int screenWidth, int screenHeight) {

    float barWidth = (float)screenWidth / NUM_BANDS;

    for(int i = 0; i < NUM_BANDS; ++i) {
        double val = bands[i] * 2;

        DrawRectangle(i * barWidth, screenHeight - val, barWidth-2, val, (Color){255, 220, 240, 255});
    }
}