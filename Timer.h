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
    if (elapsedTime < readFile() && win) {
        printf("Nowy rekord: %.f sekund\n", elapsedTime);
        writeFile(elapsedTime);
    }
};

#endif