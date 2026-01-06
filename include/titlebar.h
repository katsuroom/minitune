// Displays song name and time

#ifndef TITLEBAR_H
#define TITLEBAR_H

#define TITLEBAR_HEIGHT 16

extern int titleMaxWidth;

void titlebar_init(void);
void titlebar_update_title(char* title);
void titlebar_set_error(void);
void titlebar_draw();

#endif