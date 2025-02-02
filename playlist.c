//Naor David 

#include "playlist.h"
const char* PLAYLIST_FILE = "playlist.txt";



void removePrecedingSpaces(char* str)
{
    if (!str) return;

    char* nonSpace = str;
    while (*nonSpace == ' ' && *nonSpace != '\0')
    {
        nonSpace++;
    }

    if (nonSpace != str)
    {
        memmove(str, nonSpace, strlen(nonSpace) + 1);
    }
}





void initPlaylist(Playlist* playlist)
{
    playlist->head = NULL;
    playlist->tail = NULL;
    playlist->current = NULL;
    playlist->logFile = fopen("playlist.log", "a");
}





SongNode* createSongNode(int id, const char* artist, const char* album, const char* track, int trackNo)
{
    if (strlen(artist) >= 30 || strlen(album) >= 30 || strlen(track) >= 30)
    {
        printf("Error: String length must be less than 30 characters\n");
        return NULL;
    }

    SongNode* newNode = (SongNode*)malloc(sizeof(SongNode));
    if (newNode == NULL) return NULL;

    newNode->id = id;
    strncpy(newNode->artistName, artist, 30);
    strncpy(newNode->albumName, album, 30);
    strncpy(newNode->trackName, track, 30);
    newNode->trackNo = trackNo;

    newNode->artistName[30] = '\0';
    newNode->albumName[30] = '\0';
    newNode->trackName[30] = '\0';

    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}





void printPlaylist(Playlist* playlist) 
{
    FILE* outputs[] = { stdout, playlist->logFile };
    for (int i = 0; i < 2; i++) 
    {
        fprintf(outputs[i], "\n%-5s %-30s %-30s %-10s %-30s\n", "ID", "ArtistName", "AlbumName", "TrackNo", "TrackName");

        for (int j = 0; j < 105; j++)
            fprintf(outputs[i], "-");
        fprintf(outputs[i], "\n");

        SongNode* current = playlist->head;
        while (current) 
        {
            fprintf(outputs[i], "%-5d %-30s %-30s %-10d %-30s\n",
                current->id, current->artistName, current->albumName, current->trackNo, current->trackName);
                current = current->next;
        }
        fprintf(outputs[i], "\n");
    }
}





int importData(char* path, Playlist* playlist, int sortindex)
{
    FILE* fp = fopen(path, "r");
    if (!fp)
    {
        printf("File not found!\n");
        return 0;
    }

    char h1[6], h2[31], h3[31], h4[10], h5[31];
    int header_read = fscanf(fp, "%5s%30s%30s%9s%30s", h1, h2, h3, h4, h5);
    if (header_read != 5)
    {
        printf("Error reading file header!\n");
        fclose(fp);
        return 0;
    }

    int id, trackNo;
    char artistName[31], albumName[31], trackName[31];
    int songs_read = 0;

    while (fscanf(fp, "%5d%30[^\n]%30[^\n]%5d%30[^\n]", &id, artistName, albumName, &trackNo, trackName) == 5)
    {
        removePrecedingSpaces(artistName);
        removePrecedingSpaces(albumName);
        removePrecedingSpaces(trackName);
        artistName[30] = '\0';
        albumName[30] = '\0';
        trackName[30] = '\0';

        SongNode* newNode = createSongNode(id, artistName, albumName, trackName, trackNo);
        if (!newNode) continue;

        if (playlist->tail)
        {
            newNode->prev = playlist->tail;
            playlist->tail->next = newNode;
            playlist->tail = newNode;
        }
        else
        {
            playlist->head = playlist->tail = newNode;
        }

        songs_read++;
    }

    fclose(fp);

    if (songs_read > 0)
    {
        printPlaylist(playlist);
        return 1;
    }
    else
    {
        printf("No songs were read from the file.\n");
        return 0;
    }
}





void menu(Playlist* playlist) 
{
    printf("\nPlaylist Menu:\n");
    fprintf(playlist->logFile, "\nPlaylist Menu:\n");

    printf("1. Play current song\n");
    fprintf(playlist->logFile, "1. Play current song\n");

    printf("2. Next song\n");
    fprintf(playlist->logFile, "2. Next song\n");

    printf("3. Previous song\n");
    fprintf(playlist->logFile, "3. Previous song\n");

    printf("4. Search song\n");
    fprintf(playlist->logFile, "4. Search song\n");

    printf("5. Delete song\n");
    fprintf(playlist->logFile, "5. Delete song\n");

    printf("6. Add new song\n");
    fprintf(playlist->logFile, "6. Add new song\n");

    printf("7. Sort playlist\n");
    fprintf(playlist->logFile, "7. Sort playlist\n");

    printf("0. Exit\n");
    fprintf(playlist->logFile, "0. Exit\n");

    printf("\nEnter your choice: ");
    fprintf(playlist->logFile, "\nEnter your choice: ");
}





void playCurrentSong(Playlist* playlist) 
{
    if (!playlist->current && playlist->head) 
    {
        playlist->current = playlist->head;
    }

    if (playlist->current) 
    {
        printf("Now playing:  %s- %s from album: %s\n",
            playlist->current->artistName,
            playlist->current->trackName,
            playlist->current->albumName);
        fprintf(playlist->logFile, "Now playing:  %s- %s from album: %s\n",
            playlist->current->artistName,
            playlist->current->trackName,
            playlist->current->albumName);
    }
    else 
    {
        printf("No song is currently playing.\n");
        fprintf(playlist->logFile, "No song is currently playing.\n");
    }
    printPlaylist(playlist);
}





void nextSong(Playlist* playlist) 
{
    if (playlist->current && playlist->current->next) 
    {
        playlist->current = playlist->current->next;
        playCurrentSong(playlist);
    }
    else 
    {
        printf("No next song available.\n");
        fprintf(playlist->logFile, "No next song available.\n");
    }
}





void previousSong(Playlist* playlist) 
{
    if (playlist->current && playlist->current->prev) 
    {
        playlist->current = playlist->current->prev;
        playCurrentSong(playlist);
    }
    else 
    {
        printf("No previous song available.\n");
        fprintf(playlist->logFile, "No previous song available.\n");
    }
}





SongNode* searchSong(Playlist* playlist, char* searchStr, int searchType)
{
    if (!playlist || !searchStr) return NULL;

    SongNode* current = playlist->head;
    int found = 0;

    while (current != NULL)
    {
        char* compareStr;
        switch (searchType) {
        case 1: compareStr = current->artistName; break;
        case 2: compareStr = current->albumName; break;
        case 3: compareStr = current->trackName; break;
        default: return NULL;
        }

        if (strstr(compareStr, searchStr) != NULL) 
        {
            found++;
            printf("Found match:\n");
            fprintf(playlist->logFile, "Found match:\n");
            printf("ID: %d, Artist: %s, Album: %s, Track: %s\n",
                current->id, current->artistName, current->albumName, current->trackName);
            fprintf(playlist->logFile, "ID: %d, Artist: %s, Album: %s, Track: %s\n",
                current->id, current->artistName, current->albumName, current->trackName);
            return current;
        }
        current = current->next;
    }

    printf("No matches found for: %s\n", searchStr);
    fprintf(playlist->logFile, "No matches found for: %s\n", searchStr);
    return NULL;
}





void deleteSong(Playlist* playlist, int songId) 
{
    SongNode* current = playlist->head;
    SongNode* toDelete = NULL;

    while (current) 
    {
        if (current->id == songId) 
        {
            toDelete = current;
            break;
        }
        current = current->next;
    }

   
    if (!toDelete) 
    {
        printf("Song with ID %d was not found\n", songId);
        if (playlist->logFile) 
        {
            fprintf(playlist->logFile, "Song with ID %d was not found\n", songId);
        }
        return;
    }

 
    if (toDelete == playlist->current) 
    {
        playlist->current = (toDelete->next) ? toDelete->next : toDelete->prev;
    }

    
    if (toDelete == playlist->head && toDelete == playlist->tail) 
    {
        playlist->head = playlist->tail = NULL;
    }
    else if (toDelete == playlist->head) 
    {
        playlist->head = toDelete->next;
        if (playlist->head) 
        {
            playlist->head->prev = NULL;
        }
    }
    else if (toDelete == playlist->tail) 
    {
        playlist->tail = toDelete->prev;
        if (playlist->tail) 
        {
            playlist->tail->next = NULL;
        }
    }
    else 
    {
        toDelete->prev->next = toDelete->next;
        toDelete->next->prev = toDelete->prev;
    }

    free(toDelete);

    printf("Song with ID %d was deleted\n", songId);
    if (playlist->logFile) 
    {
        fprintf(playlist->logFile, "Song with ID %d was deleted\n", songId);
    }
}





void addSong(Playlist* playlist, int id, const char* artist, const char* album, const char* track, int trackNo) 
{
    SongNode* newNode = createSongNode(id, artist, album, track, trackNo);
    if (!newNode) 
    {
        printf("Failed to create new song\n");
        fprintf(playlist->logFile, "Failed to create new song\n");
        return;
    }

    if (playlist->tail) 
    {
        newNode->prev = playlist->tail;
        playlist->tail->next = newNode;
        playlist->tail = newNode;
    }
    else 
    {
        playlist->head = playlist->tail = newNode;
    }

    printf("Song added successfully\n");
    fprintf(playlist->logFile, "Song added successfully\n");
}





void sortPlaylist(Playlist* playlist, int sortType) 
{
    if (!playlist) 
    {
        printf("Error: Playlist is NULL\n");
        return;
    }

    if (!playlist->head || !playlist->head->next) 
    {
        printf("Sort attempted on empty or single-song playlist\n");

        if (playlist->logFile) 
        {
            fprintf(playlist->logFile, "Sort attempted on empty or single-song playlist\n");
        }
        return;
    }

    char* sortTypeStr;
    switch (sortType) 
    {
    case 1: sortTypeStr = "Artist"; break;
    case 2: sortTypeStr = "Album"; break;
    case 3: sortTypeStr = "Track"; break;
    default:
        printf("Invalid sort type\n");

        if (playlist->logFile) 
        {
            fprintf(playlist->logFile, "Invalid sort type\n");
        }
        return;
    }

    printf("Started sorting by %s\n", sortTypeStr);
    if (playlist->logFile) 
    {
        fprintf(playlist->logFile, "Started sorting by %s\n", sortTypeStr);
    }

    int swapped;
    do 
    {
        swapped = 0;
        SongNode* current = playlist->head;
        while (current->next != NULL) 
        {
            int shouldSwap = 0;
            switch (sortType) 
            {
            case 1: shouldSwap = strcmp(current->artistName, current->next->artistName) > 0; break;
            case 2: shouldSwap = strcmp(current->albumName, current->next->albumName) > 0; break;
            case 3: shouldSwap = strcmp(current->trackName, current->next->trackName) > 0; break;
            }

            if (shouldSwap) 
            {
             
                char tempStr[31];
                int tempInt;

                tempInt = current->id;
                current->id = current->next->id;
                current->next->id = tempInt;

                strcpy(tempStr, current->artistName);
                strcpy(current->artistName, current->next->artistName);
                strcpy(current->next->artistName, tempStr);

                strcpy(tempStr, current->albumName);
                strcpy(current->albumName, current->next->albumName);
                strcpy(current->next->albumName, tempStr);

                strcpy(tempStr, current->trackName);
                strcpy(current->trackName, current->next->trackName);
                strcpy(current->next->trackName, tempStr);

                tempInt = current->trackNo;
                current->trackNo = current->next->trackNo;
                current->next->trackNo = tempInt;

                swapped = 1;
            }
            current = current->next;
        }
    } while (swapped);

    printf("Playlist sorted successfully\n");
    if (playlist->logFile)
    {
        fprintf(playlist->logFile, "Playlist sorted successfully\n");
    }
}





void saveToFile(const char* filename, Playlist* playlist) 
{
    FILE* fp = fopen(filename, "w");
    if (!fp) 
    {
        printf("Could not open file for writing: %s\n", filename);
        fprintf(playlist->logFile, "Could not open file for writing: %s\n", filename);
        return;
    }

    fprintf(fp, "ID    ArtistName                     AlbumName                     TrackNo   TrackName\n");

    SongNode* current = playlist->head;
    while (current) 
    {
        fprintf(fp, "%-5d %-30s %-30s %-5d %-30s\n",
            current->id, current->artistName, current->albumName, current->trackNo, current->trackName);
        current = current->next;
    }

    printf("Playlist saved to file successfully\n");
    fprintf(playlist->logFile, "Playlist saved to file successfully\n");
    fclose(fp);
}





void freePlaylist(Playlist* playlist) 
{
    if (!playlist) 
    {
        return;
    }

    if (playlist->logFile) 
    {
        fprintf(playlist->logFile, "Starting playlist cleanup\n");
    }

    SongNode* current = playlist->head;
    while (current) 
    {
        SongNode* next = current->next;

        if (playlist->logFile) 
        {
            fprintf(playlist->logFile, "Freeing song: %s - %s\n",
                current->artistName ? current->artistName : "Unknown",
                current->trackName ? current->trackName : "Unknown");
        }
        free(current);
        current = next;
    }

    if (playlist->logFile) 
    {
        fprintf(playlist->logFile, "Freeing playlist memory\n");
        fclose(playlist->logFile);
    }

    playlist->head = NULL;
    playlist->tail = NULL;
    playlist->current = NULL;
    playlist->logFile = NULL;
}





int handleUserInput(int* choice, Playlist* playlist) 
{
    if (scanf("%d", choice) != 1) 
    {
        printf("Invalid input. Please enter a number.\n");
        fprintf(playlist->logFile, "Invalid input. Please enter a number.\n");
        while (getchar() != '\n'); 
        return 0;
    }

    fprintf(playlist->logFile, "%d\n", *choice);
    return 1;
}





void handleSearchSong(Playlist* playlist) 
{
    char searchStr[31];
    int searchType;

    printf("Enter search string: ");
    fprintf(playlist->logFile, "Enter search string: ");

    while (getchar() != '\n');
    if (fgets(searchStr, sizeof(searchStr), stdin) == NULL) 
    {
        printf("Error reading search string\n");
        fprintf(playlist->logFile, "Error reading search string\n");
        return;
    }
    searchStr[strcspn(searchStr, "\n")] = 0;
    fprintf(playlist->logFile, "%s\n", searchStr);

    printf("Enter search type (1-ArtistName, 2-AlbumName, 3-TrackName): ");
    fprintf(playlist->logFile, "Enter search type (1-ArtistName, 2-AlbumName, 3-TrackName): ");

    if (scanf("%d", &searchType) != 1 || searchType < 1 || searchType > 3) 
    {
        printf("Invalid search type. Must be between 1-3\n");
        fprintf(playlist->logFile, "Invalid search type. Must be between 1-3\n");
        while (getchar() != '\n');
        return;
    }
    fprintf(playlist->logFile, "%d\n", searchType);
    searchSong(playlist, searchStr, searchType);
    printPlaylist(playlist);
}





void handleDeleteSong(Playlist* playlist, const char* filename) 
{
    int songId;
    printf("Enter song ID to delete: ");
    fprintf(playlist->logFile, "Enter song ID to delete: ");

    if (scanf("%d", &songId) != 1) 
    {
        printf("Invalid ID input\n");
        fprintf(playlist->logFile, "Invalid ID input\n");
        while (getchar() != '\n');
        return;
    }
    fprintf(playlist->logFile, "%d\n", songId);
    deleteSong(playlist, songId);
    saveToFile(filename, playlist);
    printPlaylist(playlist);
}





void handleAddSong(Playlist* playlist, const char* filename) 
{
    int id, trackNo;
    char artist[31], album[31], track[31];

    printf("Enter song details:\n");
    fprintf(playlist->logFile, "Enter song details:\n");

    printf("ID: ");
    fprintf(playlist->logFile, "ID: ");

    if (scanf("%d", &id) != 1) 
    {
        printf("Invalid ID input\n");
        fprintf(playlist->logFile, "Invalid ID input\n");
        while (getchar() != '\n');
        return;
    }
    fprintf(playlist->logFile, "%d\n", id);
    while (getchar() != '\n');
    printf("ArtistName: ");
    fprintf(playlist->logFile, "ArtistName: ");

    if (fgets(artist, sizeof(artist), stdin) == NULL) 
    {
        printf("Error reading artist name\n");
        fprintf(playlist->logFile, "Error reading artist name\n");
        return;
    }
    artist[strcspn(artist, "\n")] = 0;
    fprintf(playlist->logFile, "%s\n", artist);

    printf("AlbumName: ");
    fprintf(playlist->logFile, "AlbumName: ");

    if (fgets(album, sizeof(album), stdin) == NULL)
    {
        printf("Error reading album name\n");
        fprintf(playlist->logFile, "Error reading album name\n");
        return;
    }
    album[strcspn(album, "\n")] = 0;
    fprintf(playlist->logFile, "%s\n", album);

    printf("TrackName: ");
    fprintf(playlist->logFile, "TrackName: ");

    if (fgets(track, sizeof(track), stdin) == NULL) 
    {
        printf("Error reading Track name\n");
        fprintf(playlist->logFile, "Error reading Track name\n");
        return;
    }
    track[strcspn(track, "\n")] = 0;
    fprintf(playlist->logFile, "%s\n", track);

    printf("TrackNo: ");
    fprintf(playlist->logFile, "TrackNo: ");

    if (scanf("%d", &trackNo) != 1) 
    {
        printf("Invalid TrackNo\n");
        fprintf(playlist->logFile, "Invalid TrackNo\n");
        while (getchar() != '\n');
        return;
    }
    fprintf(playlist->logFile, "%d\n", trackNo);
    addSong(playlist, id, artist, album, track, trackNo);
    saveToFile(filename, playlist);
    printPlaylist(playlist);
}





void handleSortPlaylist(Playlist* playlist, const char* filename) 
{
    int sortType;
    printf("Enter sort type (1-ArtistName, 2-AlbumName, 3-TrackName): ");
    fprintf(playlist->logFile, "Enter sort type (1-ArtistName, 2-AlbumName, 3-TrackName): ");

    if (scanf("%d", &sortType) != 1 || sortType < 1 || sortType > 3) 
    {
        printf("Invalid sort type. Must be between 1-3\n");
        fprintf(playlist->logFile, "Invalid sort type. Must be between 1-3\n");
        while (getchar() != '\n');
        return;
    }
    fprintf(playlist->logFile, "%d\n", sortType);
    sortPlaylist(playlist, sortType);
    saveToFile(filename, playlist);
    printPlaylist(playlist);
}


