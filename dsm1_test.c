#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "signals.c"
#include "osc.c"
#include "dsm.c"

int main(void) {

    FILE *fptr;
    fptr = fopen("dsm1.csv", "w+");

    size_t SR = 3072000; /* 192000 * 16 */
    size_t len = 65536;

    Sig* in = malloc(sizeof(Sig));
    Sig* out = malloc(sizeof(Sig));
    sig_alloc(in, 1, len);
    sig_alloc(out, 1, len);

    sine(SR, .25, 1000.0, in);
    dsm1(in, out);
    
    for (size_t i = 0; i < in->vec_len; i++) {
        fprintf(fptr, "%f\n", out->vec_space[0][i]);
    }

    sig_free(in);
    sig_free(out);

    return EXIT_SUCCESS;

}
