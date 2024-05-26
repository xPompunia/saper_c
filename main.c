#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include "./constants.h"
#include <time.h>
#include "Files.h"
#include "Mouse.h"
#include "Fields.h"
#include "Timer.h"
#include "Texture.c"
#include "./Variables.h"


int main(int argc, char* args[]) {

    readFile();
    // Ustawienie wszystkich pól na domy?lny typ
    struct field map[MAP_SIZE][MAP_SIZE];

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            map[i][j].is_visible = false;
            map[i][j].is_flagged = false;
            map[i][j].type = TILE_HIDDEN;
            map[i][j].value = 0;
        }
    }

    // Inicjalizacja bomb
    srand(time(NULL));
    int bombs_placed = 0;
    while (bombs_placed < NR_OF_MINES) {
        int i = rand() % MAP_SIZE;
        int j = rand() % MAP_SIZE;
        if (map[i][j].type != TILE_BOMB) {
            map[i][j].type = TILE_BOMB;
            bombs_placed++;
        }
    }

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (map[i][j].type == TILE_BOMB) {

                // Sprawdzenie s¹siednich pól
                for (int x = i - 1; x <= i + 1; x++) {
                    for (int y = j - 1; y <= j + 1; y++) {

                        // Je?li pole jest na mapie i nie jest min¹, zwiêksz warto?æ pola o 1
                        if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE || map[x][y].type == TILE_BOMB) {
                            continue;
                        }
                        map[x][y].value += 1;
                    }
                }
            }
        }
    }

    // Przypisanie typu
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (map[i][j].type != TILE_BOMB) {
                if (map[i][j].value <= TILE_8) {
                    map[i][j].type = (enum TileType)map[i][j].value;
                }
                else {
                    map[i][j].type = TILE_HIDDEN;
                }
            }
        }
    }


    // Wy?wietlenie mapy przed pierwszym ruchem
    printf("Generated map:\n");
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            printf("%2c ", map[i][j].type);
        }
        printf("\n");
    }

    printf("\n");
    for (int i = 0;i < MAP_SIZE;i++) {
        for (int j = 0;j < MAP_SIZE;j++) {
            printf("%2d ", map[i][j].type);
        }
        printf("\n");
    }

    startTimer();

    // Wyswietlenie mapy po pierwszym ruchu
    printf("\nMap after first move:\n");
    display_texture(map, MAP_SIZE, MAP_SIZE);

    if (!game_over && check_win(map)) {
        win = true;
        stopTimer();
        printf("You've won!\n");
    }

    return 0;
}