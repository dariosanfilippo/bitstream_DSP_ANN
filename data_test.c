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
#include "data.c"

int main(void) {
    
    size_t sr = 192000;
    size_t in_len = 64;
    size_t out_len = 1;
    size_t dim = 64;

    Sig* in = malloc(sizeof(Sig));
    Sig* out = malloc(sizeof(Sig));
    sig_alloc(in, dim, in_len, sr);
    sig_alloc(out, dim, out_len, sr);

    freq_est_data(in, out);

    sig_print(in, 30);
    sig_print(out, 30);

    sig_free(in);
    sig_free(out);

    return EXIT_SUCCESS;

}


