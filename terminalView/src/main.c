#include <stdio.h>

#include "gol.h"

#define GOL_WORLD_SIZE 10

static void gol_display_terminal(const struct GOL *g){
    const enum GOL_STATE **matrix = gol_get_world(g);
    for(unsigned int i = 0; i < g->size; i++){
        for(unsigned int j = 0; j < g->size; j++)
            printf("%s ", ((matrix[i][j] == GOL_ALIVE) ? "*" : "_"));
        printf("\n");
    }
    printf("#########################\n");
}

int main(){
    printf("TERMINAL GOL\n");

    struct GOL g;
    gol_init(&g, GOL_WORLD_SIZE);

    /* WE WANT TO INITIALIZE PATTERN */
    // _*__
    // **__
    // _**_
    gol_set_state(&g, 5, 5, GOL_ALIVE);
    gol_set_state(&g, 4, 5, GOL_ALIVE);
    gol_set_state(&g, 5, 4, GOL_ALIVE);
    gol_set_state(&g, 5, 6, GOL_ALIVE);
    gol_set_state(&g, 6, 6, GOL_ALIVE);

    const unsigned int epochs = 10;
    for(unsigned int i = 0; i < epochs; i++){
        gol_display_terminal(&g);
        gol_compute_state(&g);
    }

    gol_clean(&g);
    return 0;
}