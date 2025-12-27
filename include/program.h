#ifndef PROGRAM_H
#define PROGRAM_H

#define CONTROLS_HEIGHT 20
#define MAX_TITLE_LENGTH 40

extern float musicLength, musicTime;
extern int sampleRate;
extern bool hasMusic, isPlaying, isShuffle;
extern char* title;

void init(void);
void update(void);
void draw(void);

void play_current_song(void);
void set_song_position(float target);
void draw_visualization(void);

#endif