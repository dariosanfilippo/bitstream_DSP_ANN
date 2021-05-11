#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "signals.c"
#include "osc.c"
#include "dsm.c"

int main(void) {

    FILE *fptr;
    fptr = fopen("bm.csv", "w+");

    size_t SR = 3072000; /* 192000 * 16 */
    size_t len = 65536;

    Sig* in = malloc(sizeof(Sig));
    Sig* out = malloc(sizeof(Sig));

    sig_alloc(in, 2, len);
    sig_alloc(out, 1, len);
    sig_setconst(in, 0, .5);
    sine(SR, .25, 5000.0, in->vec_space[0], in->vec_len);
    sine(SR, .25, 1000.0, in->vec_space[1], in->vec_len);
    dsm3(in->vec_space[0], in->vec_space[0], in->vec_len);
    dsm3(in->vec_space[1], in->vec_space[1], in->vec_len);
    binarymultiplier(in, out);

    for (size_t i = 0; i < in->vec_len; i++) {
        fprintf(fptr, "%f\n", out->vec_space[0][i]);
    }

    sig_free(in);
    sig_free(out);

    return EXIT_SUCCESS;

}
