#include "raylib.h"
#include "raygui.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "program.h"
#include "titlebar.h"
#include "seeker.h"
#include "playlist.h"
#include "vis.h"

#define BTN_SZ 20   // button size

// memory allocated objects
char* titleBuffer = NULL;
Music music = {0};

float musicLength = 0;
float musicTime = 0;
int sampleRate = 0;
bool hasMusic = false;
bool isLooping = false;
bool isPlaying = false;
bool isShuffle = false;

float volume = 1.0;
float volumeIncrement = 0.1;

// playlist object
Playlist* playlist = NULL;

// function pointers
void (*vis_callback)(void *buffer, unsigned int frames) = process_frequency;
void (*vis_draw)(int screenWidth, int screenHeight) = draw_frequency;

void init(void) {
    seeker_init(BTN_SZ*3 - 3, screenHeight - CONTROLS_HEIGHT, screenWidth-(BTN_SZ*6)+6, CONTROLS_HEIGHT);
    titlebar_init();
}

void set_song_position(float pos) {
    SeekMusicStream(music, pos);
    musicTime = pos;
}

void load_music_file(char* path) {
    // start music
    if(hasMusic) {
        UnloadMusicStream(music);
    }

    music = LoadMusicStream(path);
    
    // rellocate path buffer
    if(titleBuffer != NULL) {
        free(titleBuffer);
    }
    const char* filename = GetFileName(path);
    titleBuffer = malloc(strlen(filename) + 1);
    strcpy(titleBuffer, filename);
    
    hasMusic = IsMusicValid(music);
    if(hasMusic) {
        musicLength = GetMusicTimeLength(music);
        PlayMusicStream(music);
        isPlaying = true;
        music.looping = isLooping;
        sampleRate = music.stream.sampleRate;

        // redirect sample data
        AttachAudioStreamProcessor(music.stream, vis_callback);

        titlebar_update_title(filename);
    }
    else {
        isPlaying = false;
        musicTime = 0;
        musicLength = 0;
        titlebar_set_error();
    }
}

void load_playlist(char* path) {
    FILE* file = fopen(path, "r");
    playlist = make_playlist(file);
    fclose(file);

    // print_playlist(playlist);

    // playlist has at least 1 song
    if(playlist->current != NULL) {
        if(isShuffle)
            playlist_shuffle_next(playlist);
        load_music_file(playlist->current->song_path);
    }
    else {
        load_music_file("");
    }
}

void prev_song() {
    if(playlist && playlist->current) {
        playlist_prev(playlist);
        load_music_file(playlist->current->song_path);
    }
}

void next_song() {
    if(playlist && playlist->current) {
        if(isShuffle == true) {
            playlist_shuffle_next(playlist);
        }
        else {
            playlist_next(playlist);
        }
        load_music_file(playlist->current->song_path);
    }
}

void update(void) {

    seeker_update();

    // toggle shuffle
    if(IsKeyPressed(KEY_S)) {
        isShuffle = !isShuffle;
    }

    // volume
    if(IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN)) {
        volume -= volumeIncrement;
        if(volume < 0)
            volume = 0;
        SetMusicVolume(music, volume);
    }
    if(IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP)) {
        volume += volumeIncrement;
        if(volume > 1)
            volume = 1;
        SetMusicVolume(music, volume);
    }

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
    if(isPlaying == true) {
        UpdateMusicStream(music);
        musicTime = GetMusicTimePlayed(music);

        // song ended
        if(IsMusicStreamPlaying(music) == false) {
            isPlaying = false;
            next_song();
        }
    }

    // read dropped files
    if(IsFileDropped()) {
        FilePathList files = LoadDroppedFiles();

        // only take first file
        char* path = files.paths[0];

        if(playlist != NULL) {
            free_playlist(playlist);
            playlist = NULL;
        }

        // check extension of file
        const char* ext = GetFileExtension(path);
        if(strcmp(ext, ".txt") == 0) {
            load_playlist(path);
        }
        else {
            load_music_file(path);
        }
        UnloadDroppedFiles(files);
    }
}

void draw_visualization(void) {
    if(IsMusicStreamPlaying(music) == false)
        return;

    vis_draw(screenWidth, screenHeight-BTN_SZ-TITLEBAR_HEIGHT);
}

void draw(void) {

    titlebar_draw();
    
    // ICON_PLAYER_PREVIOUS
    if(GuiButton((Rectangle){0, screenHeight-BTN_SZ, BTN_SZ, BTN_SZ}, "#129#")) {
        prev_song();
    }

    // ICON_PLAYER_PLAY
    // ICON_PLAYER_PAUSE
    if(GuiButton((Rectangle){BTN_SZ-1, screenHeight-BTN_SZ, BTN_SZ, BTN_SZ}, isPlaying ? "#132#" : "#131#")) {
        if(hasMusic == true) {
            // restart finished song
            if(IsMusicStreamPlaying(music) == false)
                PlayMusicStream(music);
            else
                PauseMusicStream(music);

            isPlaying = !isPlaying;
        }
    }

    // ICON_PLAYER_NEXT
    // ICON_SHUFFLE_FILL
    if(GuiButton((Rectangle){BTN_SZ*2-2, screenHeight-BTN_SZ, BTN_SZ, BTN_SZ}, isShuffle ? "#78#" : "#134#")) {
        next_song();
    }

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