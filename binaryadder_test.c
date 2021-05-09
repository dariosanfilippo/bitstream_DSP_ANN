#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "osc.c"
#include "dsm.c"
#include "auxiliary.c"

int main() {

    FILE* fptr;
    fptr = fopen("binaryadder.csv", "w+");

    const size_t samples = 65536;
    const size_t SR = 3072000;
    double** ba_ins = twod_alloc(2, samples);
    double* ba_out = malloc(samples * sizeof(double));
    
    /* calculate the first input */
    sine(SR, .25, 999.0, ba_ins[0], samples);
    dsm3(ba_ins[0], ba_ins[0], samples);

    /* calculate the second input */
    sine(SR, .25, 1999.0, ba_ins[1], samples);
    dsm3(ba_ins[1], ba_ins[1], samples);

    /* sum the inputs and print output */
    binaryadder(ba_ins, ba_out, samples);
    for (size_t i = 0; i < samples; i++) {
        fprintf(fptr, "%f\n", ba_out[i]);
    }

    return EXIT_SUCCESS;
}
