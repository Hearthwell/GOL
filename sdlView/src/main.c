#include <stdio.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "gol.h"

#define DISPLAY_SIZE   720
#define GOL_WORLD_SIZE 30
#define FRAME_DELAY    33

/* TODO, REMOVE IF NOT USED */
static int init_gl_context(SDL_Window *window, SDL_GLContext *context){
    *context = SDL_GL_CreateContext(window);
    if(!*context){
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return 1;
    }
    glewExperimental = GL_TRUE; 
    GLenum glewError = glewInit();
    if(glewError != GLEW_OK){
        printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
        return 1;
    }
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("OPENGL VERSION: %d.%d\n", major, minor);
    return 0;
}

static int init(SDL_Window **window){
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

static void gol_display_world(SDL_Surface *surface, const struct GOL *g){
    const enum GOL_STATE **matrix = gol_get_world(g);
    const float width = (float)DISPLAY_SIZE / g->size;
    for(unsigned int i = 0; i < g->size; i++){
        for(unsigned int j = 0; j < g->size; j++){
            SDL_Rect rect = {.x = (int)(j * width), .y = (int)(i * width), .w = (int)width, .h = (int)width};
            uint32_t color = (matrix[i][j] == GOL_ALIVE) ? 0xFFFFFFFF : 0x00000000;
            SDL_FillRect(surface, &rect, color);
        }
    }
}

int main(){
    printf("SDL View GOL\n");

    SDL_Window *window;
    //SDL_GLContext context;
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