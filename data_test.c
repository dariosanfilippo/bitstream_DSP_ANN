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
    sig_alloc(in, 4096, 1024);
    gen_freq(in);

    sig_print(in, 434);
    sig_print(in, 103);

    return 0;
}

