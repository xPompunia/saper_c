#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "./constants.h"
#include <time.h>


#define ARRAY_SIZE (MAP_SIZE*MAP_SIZE)

int mines = NR_OF_MINES;


struct field{
    char hidden_state;
    int value;
    bool is_visible;
    bool flag;
};

int display_texture(struct field (*map)[MAP_SIZE], int rows, int cols){
    // Inicjalizacja SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Utworzenie okna
    SDL_Window* window = SDL_CreateWindow("MineSweeper", 20, 20, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer* renderer = NULL;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* tile_map_surface = SDL_LoadBMP(".\\saper_sprites2.bmp");
    //SDL_Surface* tile_map_surface = SDL_LoadBMP("D:\\low_level_programming_c\\game\\img\\saper_sprites2.bmp");
    SDL_Texture* tile_texture = SDL_CreateTextureFromSurface(renderer, tile_map_surface);
    SDL_FreeSurface(tile_map_surface);
    int tile_map[MAP_SIZE][MAP_SIZE] = {1};
    SDL_Rect tile[MAP_SIZE][MAP_SIZE];
    for(int x=0;x<MAP_SIZE;x++){
        for(int y=0;y<MAP_SIZE;y++){
            tile[x][y].x = x*32;
            tile[x][y].y = y*32;
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
    select_tile_flag.x=96;
    select_tile_flag.y=0;
    select_tile_flag.w=32;
    select_tile_flag.h=32;



    bool left_click = false;
    bool gameIsRunning = true;
    while(gameIsRunning){
        SDL_Event event;

        int mouse_x, mouse_y, tile_x, tile_y;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                gameIsRunning = false;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN){
                if(event.button.button == SDL_BUTTON_LEFT){
                    printf("%s\n", "Left mouse button clicked");
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    printf("%d %d\n", mouse_x, mouse_y);
                    tile_x = mouse_x/32; tile_y=mouse_y/32;
                    map[tile_x][tile_y].is_visible = true;
                    printf("%d %d\n", tile_x, tile_y);
                    printf("%d\n", map[tile_x][tile_y].value);
                    left_click = true;
                }
                if(event.button.button == SDL_BUTTON_RIGHT){
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    tile_x = mouse_x/32; tile_y=mouse_y/32;
                    if(!map[tile_x][tile_y].flag){
                        map[tile_x][tile_y].flag = true;
                    }
                    else{
                        map[tile_x][tile_y].flag = false;
                    }

                }
            }
        }
//        SDL_SetRenderDrawColor(renderer,0x66,0x66,0xBB,0xFF);
        SDL_RenderClear(renderer);
//        SDL_Delay(20);

        for(int x=0;x<MAP_SIZE;x++){
            for(int y=0;y<MAP_SIZE;y++){
                if(map[x][y].is_visible){
                    switch (map[x][y].value) {
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
                if(map[x][y].flag){
                    SDL_RenderCopy(renderer, tile_texture, &select_tile_flag, &tile[x][y]);
                }
                else{
                    SDL_RenderCopy(renderer, tile_texture, &select_tile_hidden, &tile[x][y]);
                }

            }
        }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(tile_texture);

    return 0;
}


int main(int argc, char* args[]) {

    struct field map[MAP_SIZE][MAP_SIZE];
    long int arr[ARRAY_SIZE] = {0};
    srand(time(NULL));

    for(int i=0;i<NR_OF_MINES;i++){
        int index;
        do {
            index = rand() % (ARRAY_SIZE);
        } while(arr[index] != 0);
        arr[index] = 10;
    }

    for (int i = 0; i < MAP_SIZE; i++) {
        for(int j = 0; j < MAP_SIZE; j++){
            map[i][j].value = arr[i * MAP_SIZE + j];
            map[i][j].hidden_state = '#';
            map[i][j].is_visible = false;
            map[i][j].flag = false;
        }
    }

    for (int i = 0; i < MAP_SIZE; i++) {
        for(int j = 0; j < MAP_SIZE; j++){
            if(map[i][j].value == 10){
                if(i+1<16 && map[i+1][j].value != 10){
                    map[i+1][j].value += 1;
                }
                if(i-1>=0 && map[i-1][j].value != 10){
                    map[i-1][j].value += 1;
                }
                if(j+1<16 && map[i][j+1].value != 10){
                    map[i][j+1].value += 1;
                }
                if(j-1>=0 && map[i][j-1].value != 10){
                    map[i][j-1].value += 1;
                }

                if(i+1<16 && j+1<16 && map[i+1][j+1].value != 10){
                    map[i+1][j+1].value+=1;
                }
                if(i-1>=0 && j+1<16 && map[i-1][j+1].value != 10){
                    map[i-1][j+1].value+=1;
                }
                if(i+1<16 && j-1>=0 && map[i+1][j-1].value != 10){
                    map[i+1][j-1].value+=1;
                }
                if(i-1>=0 && j-1>=0 && map[i-1][j-1].value != 10){
                    map[i-1][j-1].value+=1;
                }
            }
        }
    }

    printf("Generated map:\n");
    for(int i=0;i<MAP_SIZE;i++){
        for(int j=0;j<MAP_SIZE;j++){
            printf("%2c ",map[i][j].hidden_state);
        }
        printf("\n");
    }
    printf("\n");
    for(int i=0;i<MAP_SIZE;i++){
        for(int j=0;j<MAP_SIZE;j++){
            printf("%2d ",map[i][j].value);
        }
        printf("\n");
    }

    printf("\n");
    for(int i=0;i<MAP_SIZE;i++){
        for(int j=0;j<MAP_SIZE;j++){
            printf(" %2d,%2d ",i, j);
        }
        printf("\n");
    }
    display_texture(map, MAP_SIZE, MAP_SIZE);
    return 0;
}