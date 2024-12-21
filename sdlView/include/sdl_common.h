#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "gol.h"

#define DISPLAY_SIZE   720
#define GOL_WORLD_SIZE 30
#define FRAME_DELAY    33

int init(SDL_Window **window);
void gol_display_world(SDL_Surface *surface, const struct GOL *g);