#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "signals.c"
#include "osc.c"
#include "dsm.c"
#include "bitmath.c"

int main(void) {

    FILE *fptr;
    fptr = fopen("fa.csv", "w+");

    size_t len = 8;

    double fa_ins[3][8] = {
        {0, 0, 0, 0, 1, 1, 1, 1},
        {0, 0, 1, 1, 0, 0, 1, 1},
        {0, 1, 0, 1, 0, 1, 0, 1}
    };

    Sig* in = malloc(sizeof(Sig));
    Sig* out = malloc(sizeof(Sig));
    sig_alloc(in, 3, len);
    sig_alloc(out, 2, len);

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < in->vec_len; j++) {
            in->vec_space[i][j] = fa_ins[i][j];
        }
    }

    fulladder(in, in, in, out, out, 0, 1, 2, 0, 1);

    for (int i = 0; i < in->vec_len; i++) {
        fprintf(fptr, "%f, %f\n", out->vec_space[0][i], out->vec_space[1][i]);
    }

    sig_free(in);
    sig_free(out);

    return EXIT_SUCCESS;
}
