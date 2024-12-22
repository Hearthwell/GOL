#include <stdio.h>

#define _STRINGIZE(x) #x
#define STRINGIZE(x) _STRINGIZE(x)

#ifdef GOL_OPENCL

#ifndef OPEN_CL_PROGRAM_SRC
#error OPENCL KERNEL IMPLEMENTATION NOT PROVIDED 
#endif //OPEN_CL_PROGRAM_SRC

#define KERNEL_NAME "gol_compute"

#include "gol.h"

#define TOKEN_STR(x) case x: return #x;
static const char *opencl_errstr(cl_int err)
{
    switch (err)
    {
        TOKEN_STR(CL_SUCCESS                        )                                  
        TOKEN_STR(CL_DEVICE_NOT_FOUND               )
        TOKEN_STR(CL_DEVICE_NOT_AVAILABLE           )
        TOKEN_STR(CL_COMPILER_NOT_AVAILABLE         ) 
        TOKEN_STR(CL_MEM_OBJECT_ALLOCATION_FAILURE  )
        TOKEN_STR(CL_OUT_OF_RESOURCES               )
        TOKEN_STR(CL_OUT_OF_HOST_MEMORY             )
        TOKEN_STR(CL_PROFILING_INFO_NOT_AVAILABLE   )
        TOKEN_STR(CL_MEM_COPY_OVERLAP               )
        TOKEN_STR(CL_IMAGE_FORMAT_MISMATCH          )
        TOKEN_STR(CL_IMAGE_FORMAT_NOT_SUPPORTED     )
        TOKEN_STR(CL_BUILD_PROGRAM_FAILURE          )
        TOKEN_STR(CL_MAP_FAILURE                    )
        TOKEN_STR(CL_MISALIGNED_SUB_BUFFER_OFFSET   )
        TOKEN_STR(CL_COMPILE_PROGRAM_FAILURE        )
        TOKEN_STR(CL_LINKER_NOT_AVAILABLE           )
        TOKEN_STR(CL_LINK_PROGRAM_FAILURE           )
        TOKEN_STR(CL_DEVICE_PARTITION_FAILED        )
        TOKEN_STR(CL_KERNEL_ARG_INFO_NOT_AVAILABLE  )
        TOKEN_STR(CL_INVALID_VALUE                  )
        TOKEN_STR(CL_INVALID_DEVICE_TYPE            )
        TOKEN_STR(CL_INVALID_PLATFORM               )
        TOKEN_STR(CL_INVALID_DEVICE                 )
        TOKEN_STR(CL_INVALID_CONTEXT                )
        TOKEN_STR(CL_INVALID_QUEUE_PROPERTIES       )
        TOKEN_STR(CL_INVALID_COMMAND_QUEUE          )
        TOKEN_STR(CL_INVALID_HOST_PTR               )
        TOKEN_STR(CL_INVALID_MEM_OBJECT             )
        TOKEN_STR(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR)
        TOKEN_STR(CL_INVALID_IMAGE_SIZE             )
        TOKEN_STR(CL_INVALID_SAMPLER                )
        TOKEN_STR(CL_INVALID_BINARY                 )
        TOKEN_STR(CL_INVALID_BUILD_OPTIONS          )
        TOKEN_STR(CL_INVALID_PROGRAM                )
        TOKEN_STR(CL_INVALID_PROGRAM_EXECUTABLE     )
        TOKEN_STR(CL_INVALID_KERNEL_NAME            )
        TOKEN_STR(CL_INVALID_KERNEL_DEFINITION      )
        TOKEN_STR(CL_INVALID_KERNEL                 )
        TOKEN_STR(CL_INVALID_ARG_INDEX              )
        TOKEN_STR(CL_INVALID_ARG_VALUE              )
        TOKEN_STR(CL_INVALID_ARG_SIZE               )
        TOKEN_STR(CL_INVALID_KERNEL_ARGS            )
        TOKEN_STR(CL_INVALID_WORK_DIMENSION         )
        TOKEN_STR(CL_INVALID_WORK_GROUP_SIZE        )
        TOKEN_STR(CL_INVALID_WORK_ITEM_SIZE         )
        TOKEN_STR(CL_INVALID_GLOBAL_OFFSET          )
        TOKEN_STR(CL_INVALID_EVENT_WAIT_LIST        )
        TOKEN_STR(CL_INVALID_EVENT                  )
        TOKEN_STR(CL_INVALID_OPERATION              )
        TOKEN_STR(CL_INVALID_GL_OBJECT              )
        TOKEN_STR(CL_INVALID_BUFFER_SIZE            )
        TOKEN_STR(CL_INVALID_MIP_LEVEL              )
        TOKEN_STR(CL_INVALID_GLOBAL_WORK_SIZE       )
        TOKEN_STR(CL_INVALID_PROPERTY               )
        TOKEN_STR(CL_INVALID_IMAGE_DESCRIPTOR       )
        TOKEN_STR(CL_INVALID_COMPILER_OPTIONS       )
        TOKEN_STR(CL_INVALID_LINKER_OPTIONS         )
        TOKEN_STR(CL_INVALID_DEVICE_PARTITION_COUNT )
        default: return "Unknown OpenCL error code";
    }
}

void gol_init_cl(struct GOL *gol, unsigned int dim){
    gol_init(gol, dim);

    /* INIT OPENCL STRUCTS */
    struct GOL_OpenCL *gol_opencl = malloc(sizeof(struct GOL_OpenCL));

    cl_int status;
    status = clGetPlatformIDs(1, &gol_opencl->platform, NULL);
    if(status != CL_SUCCESS){
        printf("ERROR: %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    } 
    status = clGetDeviceIDs(gol_opencl->platform, CL_DEVICE_TYPE_GPU, 1, &gol_opencl->device, NULL);
    if(status != CL_SUCCESS){
        printf("ERROR: %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    } 
    gol_opencl->context = clCreateContext(NULL, 1, &gol_opencl->device, NULL, NULL, &status);
    if(status != CL_SUCCESS){
        printf("ERROR: %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    } 
    gol_opencl->buffer_f = clCreateBuffer(gol_opencl->context, CL_MEM_READ_WRITE, dim * dim * sizeof(GOL_STATE_t), NULL, &status);
    if(status != CL_SUCCESS){
        printf("ERROR: %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    } 
    gol_opencl->buffer_s = clCreateBuffer(gol_opencl->context, CL_MEM_READ_WRITE, dim * dim * sizeof(GOL_STATE_t), NULL, &status);
    if(status != CL_SUCCESS){
        printf("ERROR: %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    } 
    gol_opencl->command_queue = clCreateCommandQueueWithProperties(gol_opencl->context, gol_opencl->device, NULL, &status);
    if(status != CL_SUCCESS){
        printf("ERROR: %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    } 

    const size_t size[]        = {sizeof(STRINGIZE(OPEN_CL_PROGRAM_SRC)) - 1};
    const char  *kernel_srcs[] = {STRINGIZE(OPEN_CL_PROGRAM_SRC)}; 
    gol_opencl->program = clCreateProgramWithSource(gol_opencl->context, 1, kernel_srcs, size, &status);
    if(status != CL_SUCCESS){
        printf("ERROR: %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    }
    status = clBuildProgram(gol_opencl->program, 1, &gol_opencl->device, NULL, NULL, NULL);
    if(status != CL_SUCCESS){
        size_t log_length;
        clGetProgramBuildInfo(gol_opencl->program, gol_opencl->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_length);
        char log_buffer[log_length + 1];
        log_buffer[log_length] = '\0';
        clGetProgramBuildInfo(gol_opencl->program, gol_opencl->device, CL_PROGRAM_BUILD_LOG, log_length, log_buffer, &log_length);
        printf("BUILD LOG: %s\n", log_buffer);
        return;
    }

    gol_opencl->kernel = clCreateKernel(gol_opencl->program, KERNEL_NAME, &status);
    if(status != CL_SUCCESS){
        printf("ERROR: %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    }
    status = clSetKernelArg(gol_opencl->kernel, 0, sizeof(cl_mem), &gol_opencl->buffer_f);
    status = clSetKernelArg(gol_opencl->kernel, 1, sizeof(cl_mem), &gol_opencl->buffer_s);
    status = clSetKernelArg(gol_opencl->kernel, 2, sizeof(unsigned int), &gol->size);
    if(status != CL_SUCCESS){
        printf("ERROR: %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    }
    gol_opencl->iteration_idx = 0;
    gol->opencl = gol_opencl;
}

void gol_clean_cl(struct GOL *gol){
    struct GOL_OpenCL *gol_opencl = (struct GOL_OpenCL *)gol->opencl;
    cl_int status;
    status = clReleaseProgram(gol_opencl->program);
    if(status != CL_SUCCESS){
        printf("ERROR: %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    } 
    status = clReleaseKernel(gol_opencl->kernel);
    if(status != CL_SUCCESS){
        printf("ERROR: %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    } 
    status = clReleaseCommandQueue(gol_opencl->command_queue);
    if(status != CL_SUCCESS){
        printf("ERROR: %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    } 
    status = clReleaseMemObject(gol_opencl->buffer_f);
    if(status != CL_SUCCESS){
        printf("ERROR: %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    } 
    status = clReleaseMemObject(gol_opencl->buffer_s);
    if(status != CL_SUCCESS){
        printf("ERROR: %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    } 
    status = clReleaseContext(gol_opencl->context);
    if(status != CL_SUCCESS){
        printf("ERROR: %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    } 
    status = clReleaseDevice(gol_opencl->device);
    if(status != CL_SUCCESS){
        printf("ERROR: %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    } 
    free(gol_opencl);
    gol->opencl = NULL;
}

void gol_compute_state_cl(struct GOL *gol){
    cl_int status;
    struct GOL_OpenCL *gol_opencl = (struct GOL_OpenCL *)gol->opencl;
    if(gol_opencl->iteration_idx == 0){
        status = clEnqueueWriteBuffer(gol_opencl->command_queue, gol_opencl->buffer_f, 
            CL_TRUE, 0, gol->size * gol->size * sizeof(GOL_STATE_t), gol->matrix1, 0, NULL, NULL);
        if(status != CL_SUCCESS){
            printf("ERROR %d, %s\n", __LINE__, opencl_errstr(status));
            return;
        }
    }
    status = clSetKernelArg(gol_opencl->kernel, 3, sizeof(unsigned int), &gol->index);
    status = clEnqueueNDRangeKernel(gol_opencl->command_queue, gol_opencl->kernel, 2, 
        NULL, (const size_t[]){gol->size, gol->size}, NULL, 0, NULL, NULL);
    if(status != CL_SUCCESS){
        printf("ERROR %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    }

    GOL_STATE_t *output = (gol->index % 2 == 0) ? gol->matrix2 : gol->matrix1;
    cl_mem source = (gol->index % 2 == 0) ? gol_opencl->buffer_s : gol_opencl->buffer_f;
    status = clEnqueueReadBuffer(gol_opencl->command_queue, source, CL_TRUE, 0, gol->size * gol->size * sizeof(GOL_STATE_t), output, 0, NULL, NULL);
    if(status != CL_SUCCESS){
        printf("ERROR %d, %s\n", __LINE__, opencl_errstr(status));
        return;
    }

    gol->index = (gol->index + 1) % 2;
    gol_opencl->iteration_idx++;
}

#endif //GOL_OPENCL