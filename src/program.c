#include "raylib.h"
#include "raygui.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "program.h"
#include "titlebar.h"
#include "seeker.h"

#include "vis/volume.h"
#include "vis/frequency.h"

#define BTN_SZ 20   // button size

// memory allocated objects
char* pathBuffer = NULL;
Music music = {0};

float musicLength = 0;
float musicTime = 0;
int sampleRate = 0;
bool hasMusic = false;
bool isLooping = false;
bool isPaused = true;

void set_song_position(float pos) {
    SeekMusicStream(music, pos);
    musicTime = pos;
}

void init(void) {
    seeker_init(BTN_SZ*3 - 3, screenHeight - CONTROLS_HEIGHT, screenWidth-(BTN_SZ*6)+6, CONTROLS_HEIGHT);
    titlebar_init(&hasMusic, &pathBuffer);
}

void update(void) {

    seeker_update();

    // arrow keys for seeking
    // 3 second interval
    if(hasMusic == true) {
        if(IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)) {
            int t = musicTime - 3;
            t = (t > 0) ? t : 0;
            set_song_position(t);
        }
        if(IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) {
            int t = musicTime + 3;
            if(t > musicLength)
                t = musicLength;
            set_song_position(t);
        }
    }

    // update music stream
    if(isPaused == false) {
        UpdateMusicStream(music);
        musicTime = GetMusicTimePlayed(music);

        // song ended
        if(IsMusicStreamPlaying(music) == false) {
            isPaused = true;
        }
    }

    // read dropped files
    if(IsFileDropped()) {
        FilePathList files = LoadDroppedFiles();

        // only take first file
        char* path = files.paths[0];

        // start music
        if(hasMusic) {
            UnloadMusicStream(music);
        }
        music = LoadMusicStream(path);
        
        // rellocate path buffer
        if(pathBuffer != NULL) {
            free(pathBuffer);
        }
        const char* filename = GetFileName(path);
        pathBuffer = malloc(strlen(filename) + 1);
        strcpy(pathBuffer, filename);
        UnloadDroppedFiles(files);

        hasMusic = IsMusicValid(music);
        if(hasMusic) {
            musicLength = GetMusicTimeLength(music);
            PlayMusicStream(music);
            isPaused = false;
            music.looping = isLooping;
            sampleRate = music.stream.sampleRate;

            // redirect sample data
            AttachAudioStreamProcessor(music.stream, process_frequency);

            titlebar_update_title(filename);
        }
        else {
            isPaused = true;
        }
    }
}

void draw_visualization(void) {
    if(IsMusicStreamPlaying(music) == false)
        return;

    // draw_volume(screenWidth, screenHeight-BTN_SZ-TITLEBAR_HEIGHT);
    draw_frequency(screenWidth, screenHeight-BTN_SZ-TITLEBAR_HEIGHT);
}

void draw(void) {

    titlebar_draw();
    
    // ICON_PLAYER_PREVIOUS
    GuiButton((Rectangle){0, screenHeight-BTN_SZ, BTN_SZ, BTN_SZ}, "#129#");

    // ICON_PLAYER_PLAY
    // ICON_PLAYER_PAUSE
    if(GuiButton((Rectangle){BTN_SZ-1, screenHeight-BTN_SZ, BTN_SZ, BTN_SZ}, isPaused ? "#131#" : "#132#")) {
        if(hasMusic == true) {
            // restart finished song
            if(IsMusicStreamPlaying(music) == false)
                PlayMusicStream(music);

            isPaused = !isPaused;
        }
    }

    // ICON_PLAYER_NEXT
    GuiButton((Rectangle){BTN_SZ*2-2, screenHeight-BTN_SZ, BTN_SZ, BTN_SZ}, "#134#");

    // Seeker
    seeker_draw();

    // ICON_GEAR_BIG
    GuiButton((Rectangle){screenWidth-(BTN_SZ*3-2), screenHeight-BTN_SZ, BTN_SZ, BTN_SZ}, "#142#");

    // ICON_AUDIO
    GuiButton((Rectangle){screenWidth-(BTN_SZ*2-1), screenHeight-BTN_SZ, BTN_SZ, BTN_SZ}, "#122#");

    // ICON_CROSS
    if(GuiButton((Rectangle){screenWidth-BTN_SZ, screenHeight-BTN_SZ, BTN_SZ, BTN_SZ}, "#113#")) {
        isRunning = false;
    }
}