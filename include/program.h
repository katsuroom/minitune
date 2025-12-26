#ifndef PROGRAM_H
#define PROGRAM_H

#define CONTROLS_HEIGHT 20

extern float musicLength, musicTime;
extern int sampleRate;
extern bool hasMusic, isPlaying, isShuffle;

void init(void);
void update(void);
void draw(void);

void set_song_position(float target);
void draw_visualization(void);

#endif