#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "./constants.h"
#include <time.h>


#define ARRAY_SIZE (MAP_SIZE*MAP_SIZE)

int mines = NR_OF_MINES;
bool game_over = false;


struct field{
    char hidden_state;
    int value;
    bool is_visible;
    bool flag;
    bool is_bomb;
};

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
    SDL_Surface* tile_map_surface = SDL_LoadBMP(".\\img\\saper_sprites2.bmp");
    SDL_Texture* tile_texture = SDL_CreateTextureFromSurface(renderer, tile_map_surface);
    SDL_FreeSurface(tile_map_surface);

    SDL_Rect tile[MAP_SIZE][MAP_SIZE];
    for (int x = 0; x < MAP_SIZE; x++) {
        for (int y = 0; y < MAP_SIZE; y++) {
            tile[x][y].x = x * 32;
            tile[x][y].y = y * 32;
            tile[x][y].w = 32;
            tile[x][y].h = 32;
        }
    }

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

    bool gameIsRunning = true;
    while (gameIsRunning) {
        handle_mouse_events(map);

        SDL_RenderClear(renderer);

        for (int x = 0;x < MAP_SIZE;x++) {
            for (int y = 0;y < MAP_SIZE;y++) {
                if (map[x][y].is_visible) {
                    switch (map[x][y].value) {
                    // TODO: use enums instead random values
                    case 1:
                        SDL_RenderCopy(renderer, tile_texture, &select_tile_1, &tile[x][y]);
                        break;
                    case 2:
                        SDL_RenderCopy(renderer, tile_texture, &select_tile_2, &tile[x][y]);
                        break;
                    case 3:
                        SDL_RenderCopy(renderer, tile_texture, &select_tile_3, &tile[x][y]);
                        break;
                    case 4:
                        SDL_RenderCopy(renderer, tile_texture, &select_tile_4, &tile[x][y]);
                        break;
                    case 5:
                        SDL_RenderCopy(renderer, tile_texture, &select_tile_5, &tile[x][y]);
                        break;
                    case 6:
                        SDL_RenderCopy(renderer, tile_texture, &select_tile_6, &tile[x][y]);
                        break;
                    case 7:
                        SDL_RenderCopy(renderer, tile_texture, &select_tile_7, &tile[x][y]);
                        break;
                    case 8:
                        SDL_RenderCopy(renderer, tile_texture, &select_tile_8, &tile[x][y]);
                        break;
                    case 10:
                        SDL_RenderCopy(renderer, tile_texture, &select_tile_10, &tile[x][y]);
                        break;
                    case 0:
                        SDL_RenderCopy(renderer, tile_texture, &select_tile_blank, &tile[x][y]);
                        break;
                    }
                    continue;
                }
                if (map[x][y].flag) {
                    SDL_RenderCopy(renderer, tile_texture, &select_tile_flag, &tile[x][y]);
                }
                else {
                    SDL_RenderCopy(renderer, tile_texture, &select_tile_hidden, &tile[x][y]);
                }

            }
        }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(tile_texture);

    return 0;
}
        

int handle_mouse_events(struct field(*map)[MAP_SIZE]) {
    SDL_Event event;
    bool gameIsRunning = true;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gameIsRunning = false;
        }
       
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                printf("%s\n", "Left mouse button clicked");
                int mouse_x, mouse_y, tile_x, tile_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                tile_x = mouse_x / 32;
                tile_y = mouse_y / 32;
                click_field(map, tile_x, tile_y); // Wywo³anie funkcji click_field
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


// Funkcja rekurencyjna do odkrywania pustych pól wokó³ danego pola
void reveal_empty_fields(struct field (*map)[MAP_SIZE], int row, int col) {
    // Sprawdzanie wszystkich s¹siaduj¹cych pól
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            // Sprawdzenie, czy s¹siaduj¹ce pole znajduje siê w obszarze planszy
            if (i >= 0 && i < MAP_SIZE && j >= 0 && j < MAP_SIZE) {
                // Sprawdzenie, czy pole nie zosta³o ju¿ odkryte i czy nie zawiera bomby
                if (!map[i][j].is_visible && !map[i][j].is_bomb) {
                    // Odkrycie pustego pola
                    map[i][j].is_visible = true;
                    // Jeœli odkryte pole jest puste, kontynuuj rekurencyjne odkrywanie pól wokó³ niego
                    if (map[i][j].value == 0) {
                        reveal_empty_fields(map, i, j);
                    }
                }
            }
        }
    }
}

// Funkcja odkrywaj¹ca pole w grze
int click_field(struct field(*map)[MAP_SIZE], int row, int col) {

    if (game_over) {
        return 0;
    }

    // Odkrycie pola
    map[row][col].is_visible = true;
    // Jeœli odkryte pole jest puste, odkryj wszystkie puste pola wokó³ niego
    if (map[row][col].value == 0) {
        reveal_empty_fields(map, row, col);
    }
    
    // Jeœli odkryte pole zawiera bombê, zakoñcz grê
    if (map[row][col].value == 10) {
        map[row][col].is_bomb = true;
        game_over = true;
        printf("Game Over! You clicked on a bomb.\n");
        
    }
    return 0;
}


int main(int argc, char* args[]) {

    // Inicjalizacja mapy
    struct field map[MAP_SIZE][MAP_SIZE];
    long int arr[ARRAY_SIZE] = { 0 };
    srand(time(NULL));

    // Umieszczenie min na mapie
    for (int i = 0; i < NR_OF_MINES; i++) {
        int index;
        do {
            index = rand() % (ARRAY_SIZE);
        } while (arr[index] != 0);
        arr[index] = 10;
    }

    // Ustawienie wartoœci pól na mapie
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            map[i][j].value = arr[i * MAP_SIZE + j];
            map[i][j].hidden_state = '#';
            map[i][j].is_visible = false;
            map[i][j].flag = false;
            map[i][j].is_bomb = false;
        }
    }

    // Obliczenie liczby min w s¹siedztwie ka¿dego pola
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (map[i][j].value == 10) {
                // Sprawdzenie s¹siednich pól
                for (int x = i - 1; x <= i + 1; x++) {
                    for (int y = j - 1; y <= j + 1; y++) {
                        // Jeœli pole jest na mapie i nie jest min¹, zwiêksz wartoœæ pola o 1
                        if (x >= 0 && x < MAP_SIZE && y >= 0 && y < MAP_SIZE && map[x][y].value != 10) {
                            map[x][y].value += 1;
                        }
                    }
                }
            }
        }
    }

    // Wyœwietlenie mapy przed pierwszym ruchem
    printf("Generated map:\n");
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            printf("%2c ", map[i][j].hidden_state);
        }
        printf("\n");
    }

    printf("\n");
    for (int i = 0;i < MAP_SIZE;i++) {
        for (int j = 0;j < MAP_SIZE;j++) {
            printf("%2d ", map[i][j].value);
        }
        printf("\n");
    }

    // Wyœwietlenie mapy po pierwszym ruchu
    printf("\nMap after first move:\n");
    display_texture(map, MAP_SIZE, MAP_SIZE);

    return 0;
}

