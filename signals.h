/*******************************************************************************
 *
 *      This module implements an infrastructure to operate with signals
 *      based on a vector space representation.
 *
 *      Copyright (c) Dario Sanfilippo 2021.
 *
 ******************************************************************************/

#ifndef SIGNALS
#define SIGNALS

#define RESO 32

#if RESO == 32
# define audio float
#elif RESO == 64
# define audio double
#else
#error invalid resolution: it must be 32 or 64 bits.
#endif

typedef struct Sig Sig;

void sig_alloc(Sig* sig, size_t dim, size_t len, size_t srate);
void sig_free(Sig* sig);
void sig_set_const(Sig* sig, size_t vec_id, audio val);
void sig_print_sample(Sig* sig, size_t vec_id, size_t sample_id);
void sig_print_vec(Sig* sig, size_t vec_id);
void sig_print_space(Sig* sig);
size_t sig_getdim(Sig* sig);
size_t sig_getlen(Sig* sig);

#endif
