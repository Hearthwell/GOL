#include <stdio.h>
#include <stdbool.h>

#include "sdl_common.h"

int main(){
    printf("SDL View GOL\n");

    SDL_Window *window;
    if(init(&window) != 0) return 1;
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    struct GOL g;
    gol_init(&g, GOL_WORLD_SIZE);

    /* WE WANT TO INITIALIZE PATTERN */
    // _*__
    // **__
    // _**_
    gol_set_state(&g, 15, 15, GOL_ALIVE);
    gol_set_state(&g, 14, 15, GOL_ALIVE);
    gol_set_state(&g, 15, 14, GOL_ALIVE);
    gol_set_state(&g, 15, 16, GOL_ALIVE);
    gol_set_state(&g, 16, 16, GOL_ALIVE);

    bool running = true;
    while (running){
        SDL_Event e;
        Uint32 frameStart = SDL_GetTicks();
        while(SDL_PollEvent( &e ) != 0){
            if( e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) running = false;
        }

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) SDL_Delay(FRAME_DELAY - frameTime);
        
        gol_display_world(surface, &g);
        gol_compute_state(&g);

        SDL_UpdateWindowSurface(window);
    }
}