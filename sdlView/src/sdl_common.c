#include "sdl_common.h"

int init(SDL_Window **window){
    /* initialize SDL */
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return 1;
    }
    *window = SDL_CreateWindow("Basic", 0, 0, DISPLAY_SIZE, DISPLAY_SIZE, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if(*window == NULL){
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return 1;
    }
    return 0;
}

void gol_display_world(SDL_Surface *surface, const struct GOL *g){
    const GOL_STATE_t *matrix = gol_get_world(g);
    const float width = (float)DISPLAY_SIZE / g->size;
    for(unsigned int i = 0; i < g->size; i++){
        for(unsigned int j = 0; j < g->size; j++){
            SDL_Rect rect = {.x = (int)(j * width), .y = (int)(i * width), .w = (int)width, .h = (int)width};
            uint32_t color = (matrix[i * g->size + j] == (GOL_STATE_t)GOL_ALIVE) ? 0xFFFFFFFF : 0x00000000;
            SDL_FillRect(surface, &rect, color);
        }
    }
}