#ifndef GOL_H
#define GOL_H

enum GOL_STATE{
    GOL_DEAD,
    GOL_ALIVE,
    GOL_NUM_STATES
};

struct GOL{
    unsigned int size;
    enum GOL_STATE **matrix1;
    enum GOL_STATE **matrix2;
    int index;
};

void gol_init(struct GOL *gol, unsigned int dim);
void gol_clean(struct GOL *gol);
void gol_set_state(struct GOL *gol, int x, int y, enum GOL_STATE state);
const enum GOL_STATE** gol_get_world(const struct GOL *gol); 
void gol_compute_state(struct GOL *gol);

#endif //GOL_H