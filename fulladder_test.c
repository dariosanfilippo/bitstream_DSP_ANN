#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "osc.c"
#include "dsm.c"
#include "auxiliary.c"

int main() {

    const size_t samples = 8;
    FullAdder fa;
    double fa_ins[3][8] = {
        {0, 0, 0, 0, 1, 1, 1, 1},
        {0, 0, 1, 1, 0, 0, 1, 1},
        {0, 1, 0, 1, 0, 1, 0, 1}
    };
    double** fa_inputs = twod_alloc(3, samples);
    double** fa_outs = twod_alloc(2, samples);
    
    for (int i = 0; i < samples; i++) {
        fa_inputs[0][i] = fa_ins[0][i];
        fa_inputs[1][i] = fa_ins[1][i];
        fa_inputs[2][i] = fa_ins[2][i];
    }

    for (int i = 0; i < 8; i++) {
        fa = fulladder_samplewise(fa_ins[0][i], fa_ins[1][i], fa_ins[2][i]);
        printf("%d, %d\n", fa.sum, fa.c_out);
    }

    fulladder(fa_inputs, fa_outs, samples);

    for (int i = 0; i < samples; i++) {
        printf("%f, %f\n", fa_outs[0][i], fa_outs[1][i]);
    }

    return EXIT_SUCCESS;
}
