#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "signals.c"
#include "dsm.c"

int main() {

    FILE *fptr;
    fptr = fopen("fa.csv", "w+");

    size_t len = 8;

    FullAdder fa;
    double fa_ins[3][8] = {
        {0, 0, 0, 0, 1, 1, 1, 1},
        {0, 0, 1, 1, 0, 0, 1, 1},
        {0, 1, 0, 1, 0, 1, 0, 1}
    };

    Sig* threeins = malloc(sizeof(Sig));
    Sig* twoouts = malloc(sizeof(Sig));
    sig_alloc(threeins, 3, len);
    sig_alloc(twoouts, 2, len);

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < threeins->vec_len; j++) {
            threeins->vec_space[i][j] = fa_ins[i][j];
        }
    }

    fulladder(threeins, twoouts);

    for (int i = 0; i < threeins->vec_len; i++) {
        fprintf(fptr, "%f, %f\n", twoouts->vec_space[0][i], twoouts->vec_space[1][i]);
    }

    return EXIT_SUCCESS;
}
