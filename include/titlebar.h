// Displays song name and time

#ifndef TITLEBAR_H
#define TITLEBAR_H

#define TITLEBAR_HEIGHT 14

void titlebar_init(void);
void titlebar_update_title(const char* title);
void titlebar_set_error(void);
void titlebar_draw();

#endif