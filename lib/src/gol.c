#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "gol.h"

#define GOL_UNDER_POPULATION_TRESH 2
#define GOL_OVER_POPULATION_TRESH  3

static enum GOL_STATE gol_compute_local_state(const struct GOL *gol, unsigned int x, unsigned int y){
    const enum GOL_STATE **matrix = (const enum GOL_STATE**)((gol->index % 2 == 0) ? gol->matrix1 : gol->matrix2);
    unsigned int count = 0;
    for(int i = -1; i <= 1; i++){
        int current_x = x + i;
        if(current_x < 0 || current_x >= (int)gol->size) continue; 
        for(int k = -1; k <= 1; k++){
            int current_y = y + k;
            if(current_y < 0 || current_y >= (int)gol->size || (current_x == (int)x && current_y == (int)y)) 
                continue;
            if(matrix[current_y][current_x] == GOL_ALIVE) count++;
        }
    }
    if(count < GOL_UNDER_POPULATION_TRESH ) return GOL_DEAD;
    if(count == GOL_UNDER_POPULATION_TRESH) return matrix[y][x];
    if(count == GOL_OVER_POPULATION_TRESH ) return GOL_ALIVE;
    if(count > GOL_OVER_POPULATION_TRESH  ) return GOL_DEAD;
    assert("SHOULD NEVER REACH" == NULL);
    return GOL_DEAD;
}

void gol_init(struct GOL *gol, unsigned int dim){
    gol->size = dim;
    gol->index = 0;
    gol->matrix1 = malloc(sizeof(enum GOL_STATE*) * dim);
    gol->matrix2 = malloc(sizeof(enum GOL_STATE*) * dim);
    for(unsigned int i = 0; i < dim; i++){
        gol->matrix1[i] = malloc(sizeof(enum GOL_STATE) * dim);
        memset(gol->matrix1[i], GOL_DEAD, dim * sizeof(enum GOL_STATE));
        gol->matrix2[i] = malloc(sizeof(enum GOL_STATE) * dim);
        memset(gol->matrix2[i], GOL_DEAD, dim * sizeof(enum GOL_STATE));
    }
}

void gol_clean(struct GOL *gol){
    for(unsigned int i = 0; i < gol->size; i++){
        free(gol->matrix1[i]);
        free(gol->matrix2[i]);
    }
    free(gol->matrix1);
    free(gol->matrix2);
}

void gol_set_state(struct GOL *gol, int x, int y, enum GOL_STATE state){
    assert(y >= 0 && y < (int)gol->size);
    assert(x >= 0 && x < (int)gol->size);
    assert(state != GOL_NUM_STATES);
    enum GOL_STATE **matrix = (gol->index % 2 == 0) ? gol->matrix1 : gol->matrix2;
    matrix[y][x] = state;
}

const enum GOL_STATE** gol_get_world(const struct GOL *gol){
    return (const enum GOL_STATE**)((gol->index % 2 == 0) ? gol->matrix1 : gol->matrix2);
}

void gol_compute_state(struct GOL *gol){
    enum GOL_STATE **output = (gol->index % 2 == 0) ? gol->matrix2 : gol->matrix1;
    for(unsigned int i = 0; i < gol->size; i++){
        for(unsigned int j = 0; j < gol->size; j++){
            enum GOL_STATE next = gol_compute_local_state(gol, j, i);
            output[i][j] = next;
        }
    }
    gol->index = (gol->index + 1) % 2;
}