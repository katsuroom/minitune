#include "raylib.h"
#include "program.h"
#include "playlist.h"
#include <stdlib.h>
#include <string.h>

#define PATH_BUFFER_SIZE 512

void add_link(Playlist** p, SongEntry** s) {
    // list is empty
    if((*p)->head == NULL) {
        (*p)->head = *s;
        (*p)->tail = *s;
        (*s)->prev = *s;
        (*s)->next = *s;
    }
    else {
        (*s)->prev = (*p)->tail;
        (*s)->next = (*p)->head;
        
        (*p)->tail->next = *s;
        (*p)->head->prev = *s;

        (*p)->tail = *s;
    }
}

void playlist_prev(Playlist* p) {
    p->current = p->current->prev;
}

void playlist_next(Playlist* p) {
    p->current = p->current->next;
}

void playlist_shuffle_next(Playlist* p) {
    int next = (rand() % p->numSongs) + 1;
    for(int i = 0; i < next; ++i) {
        p->current = p->current->next;
    }
}

Playlist* make_playlist(FILE* file) {

    char buffer[PATH_BUFFER_SIZE];

    Playlist* playlist = malloc(sizeof(Playlist));
    playlist->head = NULL;
    playlist->tail = NULL;
    playlist->numSongs = 0;

    while(fgets(buffer, PATH_BUFFER_SIZE, file) != 0) {

        // remove newline at end of buffer
        int length = strlen(buffer);
        if(buffer[length-1] == '\n') {
            buffer[length-1] = '\0';
            length -= 1;
        }

        if(IsPathFile(buffer) == false) {
            continue;
        }

        SongEntry* entry = malloc(sizeof(SongEntry));
        entry->art_path = NULL;
        entry->song_path = NULL;
        entry->title = NULL;

        // set song path
        entry->song_path = malloc(length + 1);
        strcpy(entry->song_path, buffer);

        // set default title
        const char* title = GetFileName(buffer);
        entry->title = malloc(strlen(title) + 1);
        strcpy(entry->title, title);

        add_link(&playlist, &entry);
        playlist->numSongs++;
    }

    playlist->current = playlist->head;

    return playlist;
}

void free_playlist(Playlist* p) {
    SongEntry* s = p->head;
    SongEntry* next = NULL;

    if(s != NULL) {
        next = s->next;
    }

    for(int i = 0; i < p->numSongs; ++i) {
        if(s->song_path) free(s->song_path);
        if(s->art_path) free(s->art_path);
        if(s->title) free(s->title);

        free(s);
        s = next;
        next = s->next;
    }

    // free playlist
    free(p);
}

void print_playlist(Playlist* p) {
    printf("Playlist: %d songs\n", p->numSongs);
    if(p->head == NULL) {
        return;
    }

    int songIdx = 0;
    SongEntry* s = p->head;
    do {
        printf("[%d] %s\n", songIdx, s->title);
        s = s->next;
        songIdx++;
    } while(s != p->head);
}