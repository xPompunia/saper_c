#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <time.h>
#include "constants.h"
#include "Files.h"
#include "./Variables.h"

time_t startTime;


void startTimer() {
    time(&startTime);
}

void stopTimer() {
    time_t endTime;
    time(&endTime);
    double elapsedTime = difftime(endTime, startTime);
    double currentRecord = readFile();

    if (win && (currentRecord == 0.0 || elapsedTime < currentRecord)) {
        printf("Nowy rekord: %.f sekund\n", elapsedTime);
        writeFile(elapsedTime);
        currentRecord = elapsedTime;
    }
    else {
        printf("Czas gry: %.f sekund\n", elapsedTime);
    }

    if (!win && elapsedTime > currentRecord) {
        printf("Aktualny rekord: %.f sekund\n", currentRecord);
    }
};

#endif