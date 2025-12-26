#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <stdio.h>

typedef struct SongEntry SongEntry;

struct SongEntry {
    char* song_path;
    char* art_path;
    char* title;
    SongEntry *prev, *next;
};

typedef struct {
    // circular doubly linked list
    SongEntry* head;
    SongEntry* tail;
    int numSongs;
    SongEntry* current;
} Playlist;

Playlist* make_playlist(FILE* file);
void free_playlist(Playlist* p);
void print_playlist(Playlist* p);

void playlist_prev(Playlist* p);
void playlist_next(Playlist* p);
void playlist_shuffle_next(Playlist* p);

#endif