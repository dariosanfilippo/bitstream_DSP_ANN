/*******************************************************************************
 *
 *      This file contains first, second, and third-order delta-sigma
 *      modulators for multi-bit to one-bit conversion.
 *
 *      While the first-order modulator is stable for any signal in the 
 *      full-scale digital amplitude, order-two and order-three modulators
 *      may result in distorted or unstable behaviours and should be used
 *      with input signals up to about 75% of the full amplitude scale.
 *
 *      For best performance, 50% amplitude peaks should be used combined
 *      with oversampling to overcome the 6 dB loss in the signal-to-noise
 *      ratio.
 *
 *      Copyright (C) Dario Sanfilippo 2021 – sanfilippo.dario@gmail.com.
 *
 ******************************************************************************/

#include "dsm.h"

/* First-order DSM */
void dsm1(Sig* in, Sig* out, size_t in_vec_id, size_t out_vec_id) {
    assert(in->vec_len == out->vec_len);
    
    audio state = 0.0;
    audio integrator = 0.0;
    
    for (size_t i = 0; i < in->vec_len; i++) {
        integrator += in->vec_space[in_vec_id][i] - state;
        out->vec_space[out_vec_id][i] = state = integrator < 0.0 ? -1.0 : 1.0;
    }
}

/* Second-order DSM */
void dsm2(Sig* in, Sig* out, size_t in_vec_id, size_t out_vec_id) {
    assert(in->vec_len == out->vec_len);
    
    audio state = 0.0;
    audio integrator1 = 0.0;
    audio integrator2 = 0.0;
    
    for (size_t i = 0; i < in->vec_len; i++) {
        integrator1 += in->vec_space[in_vec_id][i] - state;
        integrator2 += integrator1 - state * 2.0;
        out->vec_space[out_vec_id][i] = state = integrator2 < 0.0 ? -1.0 : 1.0;
    }
}

/* Third-order DSM */
void dsm3(Sig* in, Sig* out, size_t in_vec_id, size_t out_vec_id) {
    assert(in->vec_len == out->vec_len);
    
    audio state = 0.0;
    audio integrator1 = 0.0;
    audio integrator2 = 0.0;
    audio integrator3 = 0.0;
    audio c1 = 1.0 / 4.0;
    audio c2 = 1.0 / 3.0;
    audio c3 = 1.0 / 8.0;
    
    for (size_t i = 0; i < in->vec_len; i++) {
        integrator1 += in->vec_space[in_vec_id][i] * c1 - state * c1;
        integrator2 += integrator1 * c2 - state * c2;
        integrator3 += integrator2 * c3 - state * c3;
        out->vec_space[out_vec_id][i] = state = integrator3 < 0.0 ? -1.0 : 1.0;
    }
}
