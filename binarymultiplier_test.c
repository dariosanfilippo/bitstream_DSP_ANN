#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "signals.c"
#include "osc.c"
#include "dsm.c"
#include "bitmath.c"

int main(void) {

    FILE* fptr;
    fptr = fopen("bm.csv", "w+");

    size_t SR = 3072000; /* 192000 * 16 */
    size_t len = 65536;
    double amp = .25;
    double freq0 = 1000.0;
    double freq1 = 3000.0;
    double phase = .0;

    Sig* in = malloc(sizeof(Sig));
    Sig* out = malloc(sizeof(Sig));
    sig_alloc(in, 2, len);
    sig_alloc(out, 1, len);
    
    sine(SR, amp, freq0, phase, in, 0);
    sine(SR, amp, freq1, phase, in, 1);
    dsm3(in, in, 0, 0);
    dsm3(in, in, 1, 1);
    binarymultiplier(in, in, out, 0, 1, 0);

    for (size_t i = 0; i < in->vec_len; i++) {
        fprintf(fptr, "%f\n", out->vec_space[0][i]);
    }

    sig_free(in);
    sig_free(out);
    fclose(fptr);

    return EXIT_SUCCESS;

}
