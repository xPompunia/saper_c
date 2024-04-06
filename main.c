#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "./constants.h"
#include <time.h>


bool game_over = false;
bool first_move = true;
bool gameIsRunning = true;

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
    TILE_BOMB,
    TILE_HIDDEN,
    TILE_RED_BOMB
};


struct field{
    int value;
    bool is_visible;
    bool flag;
    enum TileType type;
};

// Funkcja rekurencyjna do odkrywania pustych pól wokol danego pola
void reveal_empty_fields(struct field (*map)[MAP_SIZE], int row, int col) {
    // Sprawdzanie wszystkich sasiadujacych pól
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            // Sprawdzenie, czy sasiadujace pole znajduje sie w obszarze planszy
            if (i >= 0 && i < MAP_SIZE && j >= 0 && j < MAP_SIZE) {
                // Sprawdzenie, czy pole nie zosta³o ju¿ odkryte i czy nie zawiera bomby
                if (!map[i][j].is_visible && map[i][j].type != TILE_BOMB) {
                    // Odkrycie pustego pola
                    map[i][j].is_visible = true;
                    // Jesli odkryte pole jest puste, kontynuuj rekurencyjne odkrywanie pól wokol niego
                    if (map[i][j].type == TILE_0) {
                        reveal_empty_fields(map, i, j);
                    }
                }
            }
        }
    }
}

// Funkcja odkrywajaca pole w grze
int click_field(struct field(*map)[MAP_SIZE], int row, int col) {

    if (game_over) {
        return 0;
    }

    // Odkrycie pola
    map[row][col].is_visible = true;

    //Sprawdzenie, czy to pierwszy ruch i ustawienie pola na puste
    if (first_move) {
        map[row][col].type = TILE_0;
        first_move = false;
    }

    // Je?li odkryte pole jest puste, odkryj wszystkie puste pola wokó³ niego
    if (map[row][col].type == TILE_0) {
        reveal_empty_fields(map, row, col);
    }

    // Je?li odkryte pole zawiera bombê, zakoñcz grê i odkryj reszte bomb
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
    return 0;
}

// Funkcja do obslugi klikniec myszy
int handle_mouse_events(struct field(*map)[MAP_SIZE]) {
    SDL_Event event;
//    bool gameIsRunning = true;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gameIsRunning = false;
            SDL_Quit();
            return 0;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                printf("%s\n", "Left mouse button clicked");
                int mouse_x, mouse_y, tile_x, tile_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                tile_x = mouse_x / 32;
                tile_y = mouse_y / 32;
                click_field(map, tile_x, tile_y);
            }
            if (event.button.button == SDL_BUTTON_RIGHT) {
                int mouse_x, mouse_y, tile_x, tile_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                tile_x = mouse_x / 32;
                tile_y = mouse_y / 32;
                if (!map[tile_x][tile_y].flag) {
                    map[tile_x][tile_y].flag = true;
                }
                else {
                    map[tile_x][tile_y].flag = false;
                }
            }
        }
    }
}

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
    SDL_Surface* tile_map_surface = SDL_LoadBMP("D:/low_level_programming_c/game/img/saper_sprites2.bmp"); // (w clion musze miec absolte path)
    SDL_Texture* tile_texture = SDL_CreateTextureFromSurface(renderer, tile_map_surface);
    SDL_FreeSurface(tile_map_surface);

    SDL_Surface* game_over_surface = SDL_LoadBMP("D:/low_level_programming_c/game/img/game_over.bmp") ;
    SDL_Texture* game_over_texture = SDL_CreateTextureFromSurface(renderer, game_over_surface);
    SDL_FreeSurface(game_over_surface);

    SDL_Rect tile[MAP_SIZE][MAP_SIZE];
    for (int x = 0; x < MAP_SIZE; x++) {
        for (int y = 0; y < MAP_SIZE; y++) {
            tile[x][y].x = x * 32;
            tile[x][y].y = y * 32;
            tile[x][y].w = 32;
            tile[x][y].h = 32;
        }
    }



    SDL_Rect game_over_tile;
    game_over_tile.x = 0;
    game_over_tile.y = 0;
    game_over_tile.w = 512;
    game_over_tile.h = 128;

    int game_over_x = (WINDOW_WIDTH - game_over_tile.w) / 2; // Center horizontally
    int game_over_y = (WINDOW_HEIGHT - game_over_tile.h) / 2; // Center vertically

    SDL_Rect select_tile_hidden;
    select_tile_hidden.x = 0;
    select_tile_hidden.y = 0;
    select_tile_hidden.w = 32;
    select_tile_hidden.h = 32;

    SDL_Rect select_tile_blank;
    select_tile_blank.x = 32;
    select_tile_blank.y = 0;
    select_tile_blank.w = 32;
    select_tile_blank.h = 32;

    SDL_Rect select_tile_1;
    select_tile_1.x = 0;
    select_tile_1.y = 32;
    select_tile_1.w = 32;
    select_tile_1.h = 32;

    SDL_Rect select_tile_2;
    select_tile_2.x = 32;
    select_tile_2.y = 32;
    select_tile_2.w = 32;
    select_tile_2.h = 32;

    SDL_Rect select_tile_3;
    select_tile_3.x = 64;
    select_tile_3.y = 32;
    select_tile_3.w = 32;
    select_tile_3.h = 32;

    SDL_Rect select_tile_4;
    select_tile_4.x = 96;
    select_tile_4.y = 32;
    select_tile_4.w = 32;
    select_tile_4.h = 32;

    SDL_Rect select_tile_5;
    select_tile_5.x = 128;
    select_tile_5.y = 32;
    select_tile_5.w = 32;
    select_tile_5.h = 32;

    SDL_Rect select_tile_6;
    select_tile_6.x = 160;
    select_tile_6.y = 32;
    select_tile_6.w = 32;
    select_tile_6.h = 32;

    SDL_Rect select_tile_7;
    select_tile_7.x = 192;
    select_tile_7.y = 32;
    select_tile_7.w = 32;
    select_tile_7.h = 32;

    SDL_Rect select_tile_8;
    select_tile_8.x = 224;
    select_tile_8.y = 32;
    select_tile_8.w = 32;
    select_tile_8.h = 32;

    SDL_Rect select_tile_10;
    select_tile_10.x = 160;
    select_tile_10.y = 0;
    select_tile_10.w = 32;
    select_tile_10.h = 32;

    SDL_Rect select_tile_flag;
    select_tile_flag.x = 96;
    select_tile_flag.y = 0;
    select_tile_flag.w = 32;
    select_tile_flag.h = 32;

    SDL_Rect select_tile_red_bomb;
    select_tile_red_bomb.x = 192;
    select_tile_red_bomb.y = 0;
    select_tile_red_bomb.w = 32;
    select_tile_red_bomb.h = 32;

//    bool gameIsRunning = true;
    while (gameIsRunning) {
        handle_mouse_events(map);
        SDL_RenderClear(renderer);


        for (int x = 0;x < MAP_SIZE;x++) {
            for (int y = 0;y < MAP_SIZE;y++) {
                if (map[x][y].is_visible) {

                    switch (map[x][y].type) {
                        case TILE_0:
                            SDL_RenderCopy(renderer, tile_texture, &select_tile_blank, &tile[x][y]);
                            break;
                        case TILE_1:
                            SDL_RenderCopy(renderer, tile_texture, &select_tile_1, &tile[x][y]);
                            break;
                        case TILE_2:
                            SDL_RenderCopy(renderer, tile_texture, &select_tile_2, &tile[x][y]);
                            break;
                        case TILE_3:
                            SDL_RenderCopy(renderer, tile_texture, &select_tile_3, &tile[x][y]);
                            break;
                        case TILE_4:
                            SDL_RenderCopy(renderer, tile_texture, &select_tile_4, &tile[x][y]);
                            break;
                        case TILE_5:
                            SDL_RenderCopy(renderer, tile_texture, &select_tile_5, &tile[x][y]);
                            break;
                        case TILE_6:
                            SDL_RenderCopy(renderer, tile_texture, &select_tile_6, &tile[x][y]);
                            break;
                        case TILE_7:
                            SDL_RenderCopy(renderer, tile_texture, &select_tile_7, &tile[x][y]);
                            break;
                        case TILE_8:
                            SDL_RenderCopy(renderer, tile_texture, &select_tile_8, &tile[x][y]);
                            break;
                        case TILE_BOMB:
                            SDL_RenderCopy(renderer, tile_texture, &select_tile_10, &tile[x][y]);
                            break;
                        case TILE_RED_BOMB:
                            SDL_RenderCopy(renderer, tile_texture, &select_tile_red_bomb, &tile[x][y]);
                            SDL_RenderCopy(renderer, game_over_texture, &game_over_tile, &(SDL_Rect){game_over_x, game_over_y, game_over_tile.w, game_over_tile.h});
                            break;
                    }
                    continue;
                }
                if (map[x][y].flag && gameIsRunning) {
                    SDL_RenderCopy(renderer, tile_texture, &select_tile_flag, &tile[x][y]);
                }
                else if (gameIsRunning) {
                    SDL_RenderCopy(renderer, tile_texture, &select_tile_hidden, &tile[x][y]);
                }
            }
        }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(tile_texture);
    return 0;
}

// Przypisywanie polu typu
void setTileType(struct field(*map)[MAP_SIZE], int row, int col, int value) {
    if (map[row][col].type != TILE_BOMB && map[row][col].type != TILE_HIDDEN){
        map[row][col].type = value;
    }
}

int main(int argc, char* args[]) {


    // Ustawienie wszystkich pól na domy?lny typ
    struct field map[MAP_SIZE][MAP_SIZE];

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            map[i][j].is_visible = false;
            map[i][j].flag = false;
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
                        if (x >= 0 && x < MAP_SIZE && y >= 0 && y < MAP_SIZE && map[x][y].type != TILE_BOMB) {
                            map[x][y].value += 1;
                        }
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

    // Wyswietlenie mapy po pierwszym ruchu
    printf("\nMap after first move:\n");
    display_texture(map, MAP_SIZE, MAP_SIZE);

    return 0;
}