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
    size_t sr;
};

/* Allocate a space of "dim" vectors of size "len" samples initialised to 0 */
void sig_alloc(Sig* sig, size_t dim, size_t len, size_t srate) {
    sig->vec_space = malloc(dim * sizeof(audio*));
    for (size_t i = 0; i < dim; i++) {
        sig->vec_space[i] = malloc(len * sizeof(audio));
        for (size_t j = 0; j < len; j++) {
            sig->vec_space[i][j] = 0.0;
        }
    }
    sig->space_dim = dim;
    sig->vec_len = len;
    sig->sr = srate;
}

/* Free signals memory after use */
void sig_free(Sig* sig) {
    for (size_t i = 0; i < sig->space_dim; i++) {
        free(sig->vec_space[i]);
    }
    free(sig->vec_space);
}

/* Set a specific vector to a constant value */
void sig_set_const(Sig* sig, size_t vec_id, audio val) {
    assert(vec_id < sig->space_dim);
    for (size_t i = 0; i < sig->vec_len; i++) {
        sig->vec_space[vec_id][i] = val;
    }
}

/* Print a specific sample from a specific vector on screen */
void sig_print_sample(Sig* sig, size_t vec_id, size_t sample_id) {
    assert((vec_id < sig->space_dim) & (sample_id < sig->vec_len));
    printf("Vec %zu, sample %zu: %.20f\n", vec_id, sample_id, 
        sig->vec_space[vec_id][sample_id]);
}

/* Print a specific vector on screen */
void sig_print_vec(Sig* sig, size_t vec_id) {
    assert(vec_id < sig->space_dim);
    for (size_t i = 0; i < sig->vec_len; i++) {
        printf("Vec %zu, sample %zu: %.20f\n", vec_id, i, 
            sig->vec_space[vec_id][i]);
    }
}

/* Print an entire space on screen */
void sig_print_space(Sig* sig) {
    for (size_t i = 0; i < sig->space_dim; i++) {
        for (size_t j = 0; j < sig->vec_len; j++) {
            printf("Vec %zu, sample %zu: %.20f\n", i, j, 
                sig->vec_space[i][j]);
        }
    }
}
