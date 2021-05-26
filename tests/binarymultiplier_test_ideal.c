#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "signals.c"
#include "osc.c"
#include "dsm.c"
#include "bitmath.c"

int main(void) {

    FILE *fptr;
    fptr = fopen("bm_ideal.csv", "w+");

    size_t SR = 3072000; /* 192000 * 16 */
    size_t len = 65536;

    Sig* in = malloc(sizeof(Sig));
    Sig* out = malloc(sizeof(Sig));

    sig_alloc(in, 2, len, SR);
    sig_alloc(out, 1, len, SR);
    sine(SR, .25, 5000.0, .0, in, 0);
    sine(SR, .25, 1000.0, .0, in, 1);
    for (size_t i = 0; i < in->vec_len; i++) {
        out->vec_space[0][i] = in->vec_space[0][i] * in->vec_space[1][i];
    }
    dsm3(out, out, 0, 0);
    for (size_t i = 0; i < in->vec_len; i++) {
        fprintf(fptr, "%f\n", out->vec_space[0][i]);
    }

    sig_free(in);
    sig_free(out);
    fclose(fptr);

    return EXIT_SUCCESS;

}
