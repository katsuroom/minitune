#ifndef VIS_H
#define VIS_H

void process_frequency(void *buffer, unsigned int frames);
void draw_frequency(int screenWidth, int screenHeight);

void process_volume(void *buffer, unsigned int frames);
void draw_volume(int screenWidth, int screenHeight);

#endif