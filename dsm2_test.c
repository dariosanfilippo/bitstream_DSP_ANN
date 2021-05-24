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
    fptr = fopen("dsm2.csv", "w+");

    size_t SR = 3072000; /* 192000 * 16 */
    size_t len = 65536;
    audio amp = .25;
    audio freq = 1000.0;
    audio phase = .0;

    Sig* in = malloc(sizeof(Sig));
    Sig* out = malloc(sizeof(Sig));
    sig_alloc(in, 1, len, SR);
    sig_alloc(out, 1, len, SR);

    sine(SR, amp, freq, phase, in, 0);
    dsm2(in, out, 0, 0);
    
    for (size_t i = 0; i < in->vec_len; i++) {
        fprintf(fptr, "%f\n", out->vec_space[0][i]);
    }

    sig_free(in);
    sig_free(out);

    return EXIT_SUCCESS;

}
