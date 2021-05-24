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

    FILE* fptr;
    fptr = fopen("binaryadder.csv", "w+");

    size_t SR = 3072000;
    size_t len = 65536;
    audio amp = .25;
    audio freq0 = 1000.0;
    audio freq1 = 3000.0;
    audio phase = .0;

    Sig* in = malloc(sizeof(Sig));
    Sig* out = malloc(sizeof(Sig));
    sig_alloc(in, 2, len, SR);
    sig_alloc(out, 1, len, SR);
    sine(SR, amp, freq0, phase, in, 0);
    sine(SR, amp, freq1, phase, in, 1);
    dsm3(in, in, 0, 0);
    dsm3(in, in, 1, 1);

    binaryadder(in, in, out, 0, 1, 0);

    for (size_t i = 0; i < in->vec_len; i++) {
        fprintf(fptr, "%f\n", out->vec_space[0][i]);
    }

    sig_free(in);
    sig_free(out);
    fclose(fptr);

    return EXIT_SUCCESS;
}
