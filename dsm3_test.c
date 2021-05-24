#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "signals.c"
#include "osc.c"
#include "dsm.c"

int main(void) {

    FILE *fptr;
    fptr = fopen("dsm3.csv", "w+");

    size_t SR = 3072000; /* 192000 * 16 */
    size_t len = 65536;
    double amp = .25;
    double freq = 1000.0;
    double phase = .0;

    Sig* in = malloc(sizeof(Sig));
    Sig* out = malloc(sizeof(Sig));
    sig_alloc(in, 1, len);
    sig_alloc(out, 1, len);

    sine(SR, amp, freq, phase, in, 0);
    dsm3(in, out, 0, 0);
    
    for (size_t i = 0; i < in->vec_len; i++) {
        fprintf(fptr, "%f\n", out->vec_space[0][i]);
    }

    sig_free(in);
    sig_free(out);

    return EXIT_SUCCESS;

}
