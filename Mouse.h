#ifndef MOUSE_H
#define MOUSE_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "./constants.h"
#include "Fields.h"
#include "./Variables.h"

int handle_mouse_events(struct field(*map)[MAP_SIZE]) {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gameIsRunning = false;
            SDL_Quit();
            return 0;
        }

        int mouse_x, mouse_y, tile_x, tile_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        tile_x = mouse_x / TILE_SIZE;
        tile_y = mouse_y / TILE_SIZE;

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                printf("%s\n", "Left mouse button clicked");

                if (!map[tile_x][tile_y].is_flagged) {
                    click_field(map, tile_x, tile_y);
                    if (map[tile_x][tile_y].is_visible) {
                        reveal_neighbors(map, tile_x, tile_y);
                    }
                }
            }
            if (event.button.button == SDL_BUTTON_RIGHT) {

                if (map[tile_x][tile_y].is_visible && !map[tile_x][tile_y].is_flagged) {
                    continue;
                }

                map[tile_x][tile_y].is_flagged = !map[tile_x][tile_y].is_flagged;

            }
        }
    }
};
#endif