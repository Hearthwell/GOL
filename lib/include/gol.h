#ifndef GOL_H
#define GOL_H

enum GOL_STATE{
    GOL_DEAD,
    GOL_ALIVE,
    GOL_NUM_STATES
};

struct GOL{
    unsigned int size;
    enum GOL_STATE *matrix1;
    enum GOL_STATE *matrix2;
    int index;
    void *opencl;
};

void gol_init(struct GOL *gol, unsigned int dim);
void gol_clean(struct GOL *gol);
void gol_set_state(struct GOL *gol, int x, int y, enum GOL_STATE state);
const enum GOL_STATE* gol_get_world(const struct GOL *gol); 
void gol_compute_state(struct GOL *gol);


#ifdef GOL_OPENCL

#include <CL/cl.h>

struct GOL_OpenCL{
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_mem buffer;
    cl_command_queue command_queue;
    cl_program program;
    cl_kernel kernel;
};

void gol_init_cl(struct GOL *gol, unsigned int dim);
void gol_clean_cl(struct GOL *gol);
void gol_compute_state_cl(struct GOL *gol);

#endif //GOL_OPENCL

#endif //GOL_H