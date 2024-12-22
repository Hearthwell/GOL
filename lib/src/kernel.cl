#define GOL_DEAD 0
#define GOL_ALIVE 1

#define GOL_UNDER_POPULATION_TRESH 2
#define GOL_OVER_POPULATION_TRESH  3

__kernel void gol_compute(__global int *buffer_f, __global int *buffer_s, const unsigned int dim, const unsigned int time){
    size_t x = get_global_id(0);
    size_t y = get_global_id(1);
    __global int *input  = (time == 0) ? buffer_f : buffer_s;
    __global int *output = (time == 0) ? buffer_s : buffer_f;

    const int idx = y * dim + x;
    const int uBound = dim * dim;

    unsigned int count = 0;
    for(int i = 0; i < 3; i++){
        int innerBound = idx + (i - 1) * dim;
        for(int j = 0; j < 3; j++){
            int currentBound = innerBound + (j - 1);
            if(currentBound < 0 || currentBound >= uBound || (i == 1 && j == 1)) continue;
            if(input[currentBound] == GOL_ALIVE) count++;
        }
    }

    if(count < GOL_UNDER_POPULATION_TRESH     ) output[idx] = GOL_DEAD;
    else if(count == GOL_OVER_POPULATION_TRESH) output[idx] = GOL_ALIVE;
    else if(count > GOL_OVER_POPULATION_TRESH ) output[idx] = GOL_DEAD;
    else output[idx] = input[idx];
}