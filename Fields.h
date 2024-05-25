#ifndef FIELDS_H
#define FIELDS_H

#include <stdio.h>
#include <stdbool.h>
#include "./constants.h"
#include "./Variables.h"


void reveal_empty_fields(struct field(*map)[MAP_SIZE], int row, int col) {

    // Sprawdzanie wszystkich sąsiadujących pól
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {

            // Sprawdzenie, czy sąsiednie pole znajduje się w obszarze planszy
            if (i < 0 || i >= MAP_SIZE || j < 0 || j >= MAP_SIZE) {
                continue;
            }

            // Sprawdzenie, czy pole nie zostało już odkryte i czy nie zawiera bomby
            if (map[i][j].is_visible || map[i][j].type == TILE_BOMB) {
                continue;
            }

            // Odkrycie pustego pola
            map[i][j].is_visible = true;

            // Jeśli odkryte pole jest puste, kontynuuj rekurencyjne odkrywanie pól wokół niego
            if (map[i][j].type == TILE_0) {
                reveal_empty_fields(map, i, j);
            }
        }
    }
}

// Funkcja do odkrywania pól dookoła cyfry
void reveal_neighbors(struct field(*map)[MAP_SIZE], int row, int col) {

    int flagged_count = 0;

    // Sprawdzamy liczbę oznaczonych flagą sąsiadów
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {

            // Sprawdzenie, czy sąsiednie pole znajduje się w obszarze planszy
            if (i < 0 || i >= MAP_SIZE || j < 0 || j >= MAP_SIZE) {
                continue;
            }

            if (map[i][j].is_flagged) {
                flagged_count++;
            }
        }
    }

    // Jeśli liczba oznaczonych flagą sąsiadów jest równa liczbie min wokół tego pola, odkrywamy wszystkie sąsiadujące nieoznaczone pola
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
                    printf("Game Over! You clicked on a bomb.\n");

                    for (int i = 0; i < MAP_SIZE; i++) {
                        for (int j = 0; j < MAP_SIZE; j++) {
                            if (map[i][j].type == TILE_BOMB) {
                                map[i][j].is_visible = true;
                            }
                        }
                    }
                    return 0;

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

// Funkcja odkrywajaca pole w grze
int click_field(struct field(*map)[MAP_SIZE], int row, int col) {


    // Odkrycie pola
    map[row][col].is_visible = true;

    //Sprawdzenie, czy to pierwszy ruch i ustawienie pola na puste
    if (first_move) {
        map[row][col].type = TILE_0;
        first_move = false;
    }

    // Je?li odkryte pole zawiera bombê, zakoñcz grê i odkryj reszte bomb
    if (map[row][col].type == TILE_BOMB) {
        game_over = true;
        map[row][col].type = TILE_RED_BOMB;
        //        stopTimer();
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
        //        stopTimer();
        return 0;
    }

    // Je?li odkryte pole jest puste, odkryj wszystkie puste pola wokó³ niego
    if (map[row][col].type == TILE_0) {
        reveal_empty_fields(map, row, col);
    }

    return 0;
};

#endif