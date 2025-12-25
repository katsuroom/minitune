// Displays song name and time

#ifndef TITLEBAR_H
#define TITLEBAR_H

#define TITLEBAR_HEIGHT 14

void titlebar_init(bool* hasMusic, char** title);
void titlebar_update_title(const char* title);
void titlebar_draw();

#endif