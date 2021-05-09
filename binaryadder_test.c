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
    double val0 = .3;
    double val1 = -.7;
    double avg0 = 0.0;
    double avg1 = 0.0;
    double avg2 = 0.0;

    /* calculate the first input */
    //sine(SR, .25, 999.0, ba_ins[0], samples);
    for (size_t i = 0; i < samples; i++) {
        ba_ins[0][i] = val0;
    }
    dsm3(ba_ins[0], ba_ins[0], samples);

    /* calculate the second input */
    //sine(SR, .25, 1999.0, ba_ins[1], samples);
    for (size_t i = 0; i < samples; i++) {
        ba_ins[1][i] = val1;
    }
    dsm3(ba_ins[1], ba_ins[1], samples);

    /* sum the inputs and print output */
    binaryadder(ba_ins, ba_out, samples);
    //for (size_t i = 0; i < samples; i++) {
    //    fprintf(fptr, "%f\n", ba_out[i]);
    //}
    for (size_t i = 0; i < samples; i++) {
        avg0 += ba_ins[0][i];
        avg1 += ba_ins[1][i];
        avg2 += ba_out[i];
    }

    avg0 = avg0 / (double) samples;
    avg1 = avg1 / (double) samples;
    avg2 = avg2 / (double) samples;

    printf("%f, %f, %f\n", avg0, avg1, avg2);

    twod_free(ba_ins, 2);
    free(ba_out);

    return EXIT_SUCCESS;
}
