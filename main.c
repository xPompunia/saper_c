#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "./constants.h"
#include <time.h>


bool game_over = false;
bool first_move = true;
bool gameIsRunning = true;
time_t startTime;

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


struct field{
    int value;
    bool is_visible;
    bool is_flagged;
    enum TileType type;
};

void startTimer() {
    time(&startTime);
}

void stopTimer() {
    time_t endTime;
    time(&endTime);
    double elapsedTime = difftime(endTime, startTime);
    printf("Czas gry: %.f sekund\n", elapsedTime);
}


// Funkcja rekurencyjna do odkrywania pustych pól wokol danego pola
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
    if (flagged_count == map[row][col].value) {

        for (int i = row - 1; i <= row + 1; i++) {
            for (int j = col - 1; j <= col + 1; j++) {

                if (i < 0 || i >= MAP_SIZE || j < 0 || j >= MAP_SIZE) {
                    continue;
                }

                if (!(i == row && j == col) && !map[i][j].is_visible && !map[i][j].is_flagged) {
                    map[i][j].is_visible = true;

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
        stopTimer();
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
                /*int mouse_x, mouse_y, tile_x, tile_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                tile_x = mouse_x / TILE_SIZE;
                tile_y = mouse_y / TILE_SIZE;*/
                if (!map[tile_x][tile_y].is_flagged) {
                    click_field(map, tile_x, tile_y);
                    reveal_neighbors(map, tile_x, tile_y);
                }
            }
            if (event.button.button == SDL_BUTTON_RIGHT) {
                /*int mouse_x, mouse_y, tile_x, tile_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                tile_x = mouse_x / TILE_SIZE;
                tile_y = mouse_y / TILE_SIZE;*/
                
                if (map[tile_x][tile_y].is_visible && !map[tile_x][tile_y].is_flagged) {
                    continue;
                }

                map[tile_x][tile_y].is_flagged = !map[tile_x][tile_y].is_flagged;

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
    SDL_Surface* tile_map_surface = SDL_LoadBMP(".\\img\\saper_sprites2.bmp");
    SDL_Texture* tile_texture = SDL_CreateTextureFromSurface(renderer, tile_map_surface);
    SDL_FreeSurface(tile_map_surface);

    SDL_Surface* game_over_surface = SDL_LoadBMP(".\\img\\game_over.bmp") ;
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

    SDL_Rect game_over_tile = { .x = 0, .y = 0, .w = 512, .h = 128 };
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


        for (int x = 0;x < MAP_SIZE;x++) {
            for (int y = 0;y < MAP_SIZE;y++) {
                if(game_over){
                    SDL_RenderCopy(renderer, game_over_texture, &game_over_tile, &(SDL_Rect){game_over_x, game_over_y, game_over_tile.w, game_over_tile.h});
                }

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
        SDL_RenderPresent(renderer);
        SDL_Delay(GAME_LOOP_DELAY);
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

    return 0;
}