#ifndef VARIABLES_H
#define	VARIABLES_H

#include <stdio.h>
#include <stdbool.h>
#include "./constants.h"

enum TileType {
    TILE_0,
    TILE_1,
    TILE_2,
    TILE_3,
    TILE_4,
    TILE_5,
    TILE_6,
    TILE_7,
    TILE_8,
    TILE_HIDDEN,
    TILE_FLAG,
    TILE_BOMB,
    TILE_RED_BOMB
};

struct field {
    int value;
    bool is_visible;
    bool is_flagged;
    enum TileType type;
};

bool game_over = false;
bool win = false;
bool first_move = true;
bool gameIsRunning = true;

int all_fields = MAP_SIZE * MAP_SIZE - NR_OF_MINES;

FILE* fp;
char write[100];
char read[100];

#endif