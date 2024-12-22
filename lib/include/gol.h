#ifndef GOL_H
#define GOL_H

#include <stdint.h>

typedef uint32_t GOL_STATE_t; 
enum GOL_STATE{
    GOL_DEAD,
    GOL_ALIVE,
    GOL_NUM_STATES
};

struct GOL{
    unsigned int size;
    GOL_STATE_t *matrix1;
    GOL_STATE_t *matrix2;
    int index;
    void *opencl;
};

void gol_init(struct GOL *gol, unsigned int dim);
void gol_clean(struct GOL *gol);
void gol_set_state(struct GOL *gol, int x, int y, enum GOL_STATE state);
const GOL_STATE_t* gol_get_world(const struct GOL *gol); 
void gol_compute_state(struct GOL *gol);


#ifdef GOL_OPENCL

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

struct GOL_OpenCL{
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_mem buffer_f;
    cl_mem buffer_s;
    cl_command_queue command_queue;
    cl_program program;
    cl_kernel kernel;
    unsigned int iteration_idx;
};

void gol_init_cl(struct GOL *gol, unsigned int dim);
void gol_clean_cl(struct GOL *gol);
void gol_compute_state_cl(struct GOL *gol);

#endif //GOL_OPENCL

#endif //GOL_H