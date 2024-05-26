#include <SDL.h>
#include "./constants.h"
#include "Mouse.h"
#include "Timer.h"
#include "./Variables.h"

int display_texture(struct field(*map)[MAP_SIZE], int rows, int cols) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("MineSweeper", 20, 20, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = NULL;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* tile_map_surface = SDL_LoadBMP("./img/saper_sprites2.bmp");
    SDL_Texture* tile_texture = SDL_CreateTextureFromSurface(renderer, tile_map_surface);
    SDL_FreeSurface(tile_map_surface);

    SDL_Surface* game_over_surface = SDL_LoadBMP("./img/game_over2.bmp");
    SDL_Texture* game_over_texture = SDL_CreateTextureFromSurface(renderer, game_over_surface);
    SDL_FreeSurface(game_over_surface);

    SDL_Surface* win_surface = SDL_LoadBMP("./img/win.bmp");
    SDL_Texture* win_texture = SDL_CreateTextureFromSurface(renderer, win_surface);
    SDL_FreeSurface(win_surface);

    SDL_Rect tile[MAP_SIZE][MAP_SIZE];
    for (int x = 0; x < MAP_SIZE; x++) {
        for (int y = 0; y < MAP_SIZE; y++) {
            tile[x][y].x = x * 32;
            tile[x][y].y = y * 32;
            tile[x][y].w = 32;
            tile[x][y].h = 32;
        }
    }

    SDL_Rect game_over_tile = { .x = 0, .y = 0, .w = 512, .h = 128 };
    SDL_Rect win_tile = { .x = 0, .y = 0, .w = 512, .h = 128 };
    SDL_Rect select_tile_hidden = { .x = 0, .y = 0, .w = 32, .h = 32 };
    SDL_Rect select_tile_blank = { .x = 32, .y = 0, .w = 32, .h = 32 };
    SDL_Rect select_tile_1 = { .x = 0, .y = 32, .w = 32, .h = 32 };
    SDL_Rect select_tile_2 = { .x = 32, .y = 32, .w = 32, .h = 32 };
    SDL_Rect select_tile_3 = { .x = 64, .y = 32, .w = 32, .h = 32 };
    SDL_Rect select_tile_4 = { .x = 96, .y = 32, .w = 32, .h = 32 };
    SDL_Rect select_tile_5 = { .x = 128, .y = 32, .w = 32, .h = 32 };
    SDL_Rect select_tile_6 = { .x = 160, .y = 32, .w = 32, .h = 32 };
    SDL_Rect select_tile_7 = { .x = 192, .y = 32, .w = 32, .h = 32 };
    SDL_Rect select_tile_8 = { .x = 224, .y = 32, .w = 32, .h = 32 };
    SDL_Rect select_tile_bomb = { .x = 160, .y = 0, .w = 32, .h = 32 };
    SDL_Rect select_tile_flag = { .x = 96, .y = 0, .w = 32, .h = 32 };
    SDL_Rect select_tile_red_bomb = { .x = 192, .y = 0, .w = 32, .h = 32 };

    int game_over_x = (WINDOW_WIDTH - game_over_tile.w) / 2; // Center horizontally
    int game_over_y = (WINDOW_HEIGHT - game_over_tile.h) / 2; // Center vertically


    SDL_Rect fields[] = { select_tile_blank, select_tile_1, select_tile_2, select_tile_3, select_tile_4, select_tile_5,
                select_tile_6, select_tile_7, select_tile_8, select_tile_hidden, select_tile_flag, select_tile_bomb, select_tile_red_bomb };

    while (gameIsRunning) {
        handle_mouse_events(map);
        SDL_RenderClear(renderer);

        if (!game_over && check_win(map)) {
            win = true;
            stopTimer();
            SDL_RenderCopy(renderer, win_texture, &win_tile, &(SDL_Rect){game_over_x, game_over_y, win_tile.w, win_tile.h});
            printf("You've won!\n");
        }

        for (int x = 0;x < MAP_SIZE;x++) {
            for (int y = 0;y < MAP_SIZE;y++) {

                if (map[x][y].is_flagged) {
                    SDL_RenderCopy(renderer, tile_texture, &select_tile_flag, &tile[x][y]);
                }
                else if (map[x][y].is_visible) {
                    SDL_Rect field = fields[map[x][y].type];
                    SDL_RenderCopy(renderer, tile_texture, &field, &tile[x][y]);
                }
                else {
                    SDL_RenderCopy(renderer, tile_texture, &select_tile_hidden, &tile[x][y]);
                }

            }
        }

        if (game_over) {
            SDL_RenderCopy(renderer, game_over_texture, &game_over_tile, &(SDL_Rect){game_over_x, game_over_y, game_over_tile.w, game_over_tile.h});
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(GAME_LOOP_DELAY);
    }
    SDL_DestroyTexture(tile_texture);
    return 0;
};