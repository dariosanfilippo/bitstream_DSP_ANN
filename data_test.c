#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "signals.c"
#include "osc.c"
#include "dsm.c"
#include "data.c"

int main(void) {
    
    Sig* in = malloc(sizeof(Sig));
    Sig* out = malloc(sizeof(Sig));
    sig_alloc(in, 4096, 1024);
    sig_alloc(out, 4096, 1);
    
    gen_freq(in, out);

    sig_print(in, 434);
    sig_print(in, 103);
    
    for (size_t i = 0; i < out->space_dim; i++) {
        sig_print(out, i);
    }

    return 0;
}

