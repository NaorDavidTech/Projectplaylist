//Naor David 

#define _CRT_SECURE_NO_WARNINGS
#include "playlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) 
{
    // Initialize file path
    char* filePath = (argc < 2) ? PLAYLIST_FILE : argv[1];

    // Initialize playlist
    Playlist playlist;
    initPlaylist(&playlist);

    // Import data from file with optional sorting
    int sortIndex = (argc >= 3) ? atoi(argv[2]) : 0;
    if (!importData(filePath, &playlist, sortIndex)) 
    {
        printf("Failed to import data from file\n");
        fprintf(playlist.logFile, "Failed to import data from file\n");
        freePlaylist(&playlist);
        return 1;
    }

    // Main menu loop
    int choice;
    do 
    {
        menu(&playlist);
        if (!handleUserInput(&choice, &playlist)) 
        {
            continue;
        }

        switch (choice) 
        {
        case 1:
            playCurrentSong(&playlist);
            break;
        case 2:
            nextSong(&playlist);
            break;
        case 3:
            previousSong(&playlist);
            break;
        case 4:
            handleSearchSong(&playlist);
            break;
        case 5:
            handleDeleteSong(&playlist, filePath);
            break;
        case 6:
            handleAddSong(&playlist, filePath);
            break;
        case 7:
            handleSortPlaylist(&playlist, filePath);
            break;
        case 0:
            printf("Saving and exiting...\n");
            fprintf(playlist.logFile, "Saving and exiting...\n");
            saveToFile(filePath, &playlist);
            break;
        default:
            printf("Invalid choice, please try again\n");
            fprintf(playlist.logFile, "Invalid choice, please try again\n");
        }
    } while (choice != 0);

    freePlaylist(&playlist);
    return 0;
}



