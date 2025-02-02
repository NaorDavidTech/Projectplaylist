//Naor David 

#define _CRT_SECURE_NO_WARNINGS
#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct songNode 
{
    int id;
    char artistName[31];
    char albumName[31];
    char trackName[31];
    int trackNo;
    struct songNode* next;
    struct songNode* prev;
} SongNode;

typedef struct Playlist
{
    SongNode* head;
    SongNode* tail;
    SongNode* current;
    FILE* logFile;
} Playlist;


extern const char* PLAYLIST_FILE;

// Basic operations
void menu(Playlist* playlist);
int importData(char* path, Playlist* playlist, int sortindex);
void initPlaylist(Playlist* playlist);
void printPlaylist(Playlist* playlist);

// Playback operations
void playCurrentSong(Playlist* playlist);
void nextSong(Playlist* playlist);
void previousSong(Playlist* playlist);

// Playlist management
SongNode* searchSong(Playlist* playlist, char* searchStr, int searchType);
void deleteSong(Playlist* playlist, int songId);
void addSong(Playlist* playlist, int id, const char* artist, const char* album, const char* track, int trackNo);
void sortPlaylist(Playlist* playlist, int sortType);

// File operations
void saveToFile(const char* filename, Playlist* playlist);

// Memory management
void freePlaylist(Playlist* playlist);
SongNode* createSongNode(int id, const char* artist, const char* album, const char* track, int trackNo);


void removePrecedingSpaces(char* str);


int handleUserInput(int* choice, Playlist* playlist);
void handleSearchSong(Playlist* playlist);
void handleDeleteSong(Playlist* playlist, const char* filename);
void handleAddSong(Playlist* playlist, const char* filename);
void handleSortPlaylist(Playlist* playlist, const char* filename);

#endif



