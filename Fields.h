#ifndef FIELDS_H
#define FIELDS_H

#include <stdio.h>
#include <stdbool.h>
#include "./constants.h"
#include "./Variables.h"
#include "Timer.h"


void reveal_empty_fields(struct field(*map)[MAP_SIZE], int row, int col) {

    // Check all neighbouring fields
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {

            // Check that the neighbouring field is within the board
            if (i < 0 || i >= MAP_SIZE || j < 0 || j >= MAP_SIZE) {
                continue;
            }

            // Check that the field is not already revealed and holds no bomb
            if (map[i][j].is_visible || map[i][j].type == TILE_BOMB) {
                continue;
            }

            // Reveal the empty field
            map[i][j].is_visible = true;

            // If the revealed field is empty, keep revealing the fields around it recursively
            if (map[i][j].type == TILE_0) {
                reveal_empty_fields(map, i, j);
            }
        }
    }
}

// Reveals the fields around a digit
void reveal_neighbors(struct field(*map)[MAP_SIZE], int row, int col) {

    int flagged_count = 0;

    // Count the flagged neighbours
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {

            // Check that the neighbouring field is within the board
            if (i < 0 || i >= MAP_SIZE || j < 0 || j >= MAP_SIZE) {
                continue;
            }

            if (map[i][j].is_flagged) {
                flagged_count++;
            }
        }
    }

    // If the number of flagged neighbours equals the mine count around this field, reveal all unflagged neighbours
    if (flagged_count != map[row][col].value) return;


    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {

            if (i < 0 || i >= MAP_SIZE || j < 0 || j >= MAP_SIZE) {
                continue;
            }

            if (!map[i][j].is_flagged) {
                map[i][j].is_visible = true;

                if (map[i][j].type == TILE_BOMB) {
                    game_over = true;
                    map[i][j].type = TILE_RED_BOMB;

                    for (int i = 0; i < MAP_SIZE; i++) {
                        for (int j = 0; j < MAP_SIZE; j++) {
                            if (map[i][j].type == TILE_BOMB) {
                                map[i][j].is_visible = true;
                            }
                        }
                    }
                    return;

                }
            }
        }
    }
}


int check_win(struct field(*map)[MAP_SIZE]) {
    int visible_fields = 0;

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (map[i][j].is_visible) {
                visible_fields++;
            }
        }
    }
    return visible_fields >= all_fields;
}

// Reveals a field in the game
int click_field(struct field(*map)[MAP_SIZE], int row, int col) {


    // Reveal the field
    map[row][col].is_visible = true;

    // If this is the first move, make the field empty
    if (first_move) {
        map[row][col].type = TILE_0;
        first_move = false;
    }

    // If the revealed field holds a bomb, end the game and reveal the remaining bombs
    if (map[row][col].type == TILE_BOMB) {
        game_over = true;
        map[row][col].type = TILE_RED_BOMB;
        printf("Game Over! You clicked on a bomb.\n");

        for (int i = 0; i < MAP_SIZE; i++) {
            for (int j = 0; j < MAP_SIZE; j++) {
                if (map[i][j].type == TILE_BOMB) {
                    map[i][j].is_visible = true;
                }
            }
        }

    }
    if (game_over || win) {
        stopTimer();
        return 0;
    }

    // If the revealed field is empty, reveal all empty fields around it
    if (map[row][col].type == TILE_0) {
        reveal_empty_fields(map, row, col);
    }

    return 0;
};

#endif