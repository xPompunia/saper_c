#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include"constants.h"
#include <stdlib.h>
#include "./Variables.h"



void writeFile(double gameTime) {
    errno_t err;

    // Use fopen_s instead of fopen
    err = fopen_s(&fp, "game_stats.txt", "w");
    if (err != 0)
    {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Write to the file as before
        fprintf(fp, "%f", gameTime);
        printf("File 'game_stats.txt' created and written successfully.\n");
        fclose(fp);
    }
}

double readFile() {
    errno_t err;

    // Use fopen_s instead of fopen
    //err = fopen_s(&fp, "C:/Users/Alicja/source/repos/Saper - projekt/Saper/game_stats.txt", "r");
    err = fopen_s(&fp, "./game_stats.txt", "r");
    if (err != 0)
    {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE); // Use a consistent exit strategy (EXIT_FAILURE is more standard than 1)
    }
    else
    {
        if (fgets(read, 100, fp) != NULL) {
            // Optional: Print the time read from the file
            // printf("Time from game_stats.txt: %s\n", read);
        }
        fclose(fp);
        double time = strtod(read, NULL);
        return time;
    }
};
#endif
