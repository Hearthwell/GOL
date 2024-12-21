#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "gol.h"

void gol_init(struct GOL *gol, unsigned int dim){
    gol->size = dim;
    gol->index = 0;
    gol->matrix1 = malloc(sizeof(enum GOL_STATE) * dim * dim);
    memset(gol->matrix1, GOL_DEAD, dim * dim * sizeof(enum GOL_STATE));
    gol->matrix2 = malloc(sizeof(enum GOL_STATE) * dim * dim);
    memset(gol->matrix2, GOL_DEAD, dim * dim * sizeof(enum GOL_STATE));
}

void gol_clean(struct GOL *gol){
    free(gol->matrix1);
    free(gol->matrix2);
    gol->matrix1 = NULL;
    gol->matrix2 = NULL;
}

void gol_set_state(struct GOL *gol, int x, int y, enum GOL_STATE state){
    assert(y >= 0 && y < (int)gol->size);
    assert(x >= 0 && x < (int)gol->size);
    assert(state != GOL_NUM_STATES);
    enum GOL_STATE *matrix = (gol->index % 2 == 0) ? gol->matrix1 : gol->matrix2;
    matrix[y * gol->size + x] = state;
}

const enum GOL_STATE* gol_get_world(const struct GOL *gol){
    return (const enum GOL_STATE*)((gol->index % 2 == 0) ? gol->matrix1 : gol->matrix2);
}