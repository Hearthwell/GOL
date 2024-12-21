#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "gol.h"

#define GOL_UNDER_POPULATION_TRESH 2
#define GOL_OVER_POPULATION_TRESH  3

static enum GOL_STATE gol_compute_local_state(const struct GOL *gol, unsigned int x, unsigned int y){
    const enum GOL_STATE *matrix = (const enum GOL_STATE*)((gol->index % 2 == 0) ? gol->matrix1 : gol->matrix2);
    unsigned int count = 0;
    for(int i = -1; i <= 1; i++){
        int current_x = x + i;
        if(current_x < 0 || current_x >= (int)gol->size) continue; 
        for(int k = -1; k <= 1; k++){
            int current_y = y + k;
            if(current_y < 0 || current_y >= (int)gol->size || (current_x == (int)x && current_y == (int)y)) 
                continue;
            if(matrix[current_y * gol->size + current_x] == GOL_ALIVE) count++;
        }
    }
    if(count < GOL_UNDER_POPULATION_TRESH ) return GOL_DEAD;
    if(count == GOL_UNDER_POPULATION_TRESH) return matrix[y * gol->size + x];
    if(count == GOL_OVER_POPULATION_TRESH ) return GOL_ALIVE;
    if(count > GOL_OVER_POPULATION_TRESH  ) return GOL_DEAD;
    assert("SHOULD NEVER REACH" == NULL);
    return GOL_DEAD;
}

void gol_compute_state(struct GOL *gol){
    enum GOL_STATE *output = (gol->index % 2 == 0) ? gol->matrix2 : gol->matrix1;
    for(unsigned int i = 0; i < gol->size; i++){
        for(unsigned int j = 0; j < gol->size; j++){
            enum GOL_STATE next = gol_compute_local_state(gol, j, i);
            output[i * gol->size + j] = next;
        }
    }
    gol->index = (gol->index + 1) % 2;
}