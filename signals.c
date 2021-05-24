/*******************************************************************************
 *
 *      This module implements an infrastructure to operate with signals
 *      based on a vector space representation.
 *
 * Copyright (c) Dario Sanfilippo 2021.
 *
 ******************************************************************************/

#include "signals.h"

struct Sig {
    audio** vec_space;
    size_t space_dim;
    size_t vec_len;
};

/* Allocate a space of "dim" vectors of size "len" samples initialised to 0 */
void sig_alloc(Sig* sig, size_t dim, size_t len) {
    sig->vec_space = malloc(dim * sizeof(audio*));
    for (size_t i = 0; i < dim; i++) {
        sig->vec_space[i] = malloc(len * sizeof(audio));
        for (size_t j = 0; j < len; j++) {
            sig->vec_space[i][j] = 0.0;
        }
    }
    sig->space_dim = dim;
    sig->vec_len = len;
}

/* Free signals memory after use */
void sig_free(Sig* sig) {
    for (size_t i = 0; i < sig->space_dim; i++) {
        free(sig->vec_space[i]);
    }
    free(sig->vec_space);
}

/* Set a specific vector to a constant value */
void sig_setconst(Sig* sig, size_t vec_id, audio val) {
    assert(vec_id < sig->space_dim);
    for (size_t i = 0; i < sig->vec_len; i++) {
        sig->vec_space[vec_id][i] = val;
    }
}

/* Print a specific vector on screen */
void sig_print(Sig* sig, size_t vec_id) {
    assert(vec_id < sig->space_dim);
    for (size_t i = 0; i < sig->vec_len; i++) {
        printf("Sample %zu: %.20f\n", i, sig->vec_space[vec_id][i]);
    }
}

/* Get the dimension of a space */
size_t sig_getdim(Sig* sig) {
    return sig->space_dim;
}

/* Get the length of the vectors in a space */
size_t sig_getlen(Sig* sig) {
    return sig->vec_len;
}
