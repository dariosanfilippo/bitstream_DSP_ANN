#include "signals.h"

struct Sig {
    double** vec_space;
    size_t space_dim;
    size_t vec_len;
    size_t sr;
};

void sig_alloc(Sig* sig, size_t dim, size_t len) {
    sig->vec_space = malloc(dim * sizeof(double*));
    for (size_t i = 0; i < dim; i++) {
        sig->vec_space[i] = malloc(len * sizeof(double));
        for (size_t j = 0; j < len; j++) {
            sig->vec_space[i][j] = 0.0;
        }
    }
    sig->space_dim = dim;
    sig->vec_len = len;
}

void sig_free(Sig* sig) {
    for (size_t i = 0; i < sig->space_dim; i++) {
        free(sig->vec_space[i]);
    }
    free(sig->vec_space);
}

void sig_setconst(Sig* sig, size_t vec_id, double val) {
    assert(vec_id < sig->space_dim);
    for (size_t i = 0; i < sig->vec_len; i++) {
        sig->vec_space[vec_id][i] = val;
    }
}

void sig_print(Sig* sig, size_t vec_id) {
    assert(vec_id < sig->space_dim);
    for (size_t i = 0; i < sig->vec_len; i++) {
        printf("Sample %zu: %.20f\n", i, sig->vec_space[vec_id][i]);
    }
}

size_t sig_getdim(Sig* sig) {
    return sig->space_dim;
}

size_t sig_getlen(Sig* sig) {
    return sig->vec_len;
}
