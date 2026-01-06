#include "raylib.h"
#include "program.h"
#include "playlist.h"
#include "titlebar.h"
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

char* alloc_title(const char* title) {
    int length = strlen(title);
    bool trimmed = false;
    if(length > MAX_TITLE_LENGTH) {
        trimmed = true;
        length = MAX_TITLE_LENGTH;
    }

    char* titleBuffer = malloc(length+1);
    strncpy(titleBuffer, title, length);
    titleBuffer[length] = 0;

    // remove characters if longer than titleMaxWidth
    Vector2 dim = MeasureTextEx(font, titleBuffer, fontSize, fontSpacing);
    unsigned char c = 0;
    while(dim.x > titleMaxWidth || (c >> 6) == 0b10) {
        // erase characters
        trimmed = true;
        c = titleBuffer[length-1];
        length--;
        titleBuffer[length] = 0;
        dim = MeasureTextEx(font, titleBuffer, fontSize, fontSpacing);
    }

    if(trimmed == true) {
        for(int i = 0; i < 3; ++i)
            titleBuffer[length-1-i] = '.';
    }

    return titleBuffer;
}

bool check_metadata(char** dest, char* substr, const char* match) {
    int len = strlen(match);
    if(strncmp(substr, match, len) == 0) {
        // modify last entry
        if(*dest != NULL)
            free(*dest);
        *dest = malloc(strlen(substr)-len+1);
        strcpy(*dest, substr+len);
        return true;
    }
    return false;
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

    SongEntry* lastEntry = NULL;

    while(fgets(buffer, PATH_BUFFER_SIZE, file) != 0) {

        // remove newline at end of buffer
        int length = strlen(buffer);
        if(buffer[length-1] == '\n') {
            buffer[length-1] = '\0';
            length -= 1;
        }

        if(length == 0)
            continue;

        // skip leading whitespace
        char* substr = buffer;
        while(substr[0] == ' ' || substr[0] == '\t') {
            substr++;
            length--;
        }

        // is config line
        if(lastEntry != NULL && length >= 1 && substr[0] == '-') {
            substr++;
            length--;
            while(substr[0] == ' ' || substr[0] == '\t') {
                substr++;
                length--;
            }

            bool res = check_metadata(&lastEntry->title, substr, "title:");
            if(!res) res = check_metadata(&lastEntry->art_path, substr, "art:");
        }
        else {
            // check if file
            if(IsPathFile(substr) == false) {
                printf("Error: %s\n", substr);
                continue;
            }

            SongEntry* entry = malloc(sizeof(SongEntry));
            entry->art_path = NULL;
            entry->song_path = NULL;
            entry->title = NULL;

            // set song path
            entry->song_path = malloc(length + 1);
            strcpy(entry->song_path, substr);

            // set default title
            const char* title = GetFileName(substr);
            entry->title = alloc_title(title);

            add_link(&playlist, &entry);
            playlist->numSongs++;

            lastEntry = entry;
        }
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