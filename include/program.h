#ifndef PROGRAM_H
#define PROGRAM_H

#define CONTROLS_HEIGHT 20
#define MAX_TITLE_LENGTH 40
#define ART_SZ 100   // artwork size when ART_DISPLAY_SIDE

extern float musicLength, musicTime;
extern int sampleRate;
extern bool hasMusic, isPlaying, isShuffle;
extern char* title;

enum {
    ART_DISPLAY_SIDE,   // small image on left side
    ART_DISPLAY_FULL    // span full width
};
extern int artDisplayMode;

extern Font font;
extern int fontSize;

void init(void);
void update(void);
void draw(void);

void load_file(char* path);

void play_current_song(void);
void set_song_position(float target);
void draw_visualization(void);

#endif