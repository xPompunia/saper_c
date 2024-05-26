#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include"constants.h"
#include <stdlib.h>
#include "./Variables.h"



void writeFile(double gameTime) {
    errno_t err;

    // Use fopen_s instead of fopen
    err = fopen_s(&fp, "./game_stats.txt", "a");
    if (err != 0)
    {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Write to the file as before
        fprintf(fp, "%f\n", gameTime);
        printf("File 'game_stats.txt' updated successfully.\n");
        fclose(fp);
    }
}

double readFile() {
    errno_t err;
    double minTime = 100000;
    double currentTime;

    // Use fopen_s instead of fopen
    err = fopen_s(&fp, "./game_stats.txt", "r");
    if (err != 0)
    {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE); // Use a consistent exit strategy (EXIT_FAILURE is more standard than 1)
    }
    else
    {
        while (fgets(read, 100, fp) != NULL) {
            currentTime = strtod(read, NULL);
            if (currentTime < minTime) {
                minTime = currentTime;
            }
        }
        fclose(fp);

        if (minTime == 100000) {
            minTime = 0.0;
        }

        double time = strtod(read, NULL);
        return time;
    }
};
#endif
