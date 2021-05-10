#ifndef SIGNALS
#define SIGNALS

typedef struct Sig Sig;

void sig_alloc(Sig* sig);
void sig_free(Sig* sig);
void sig_setconst(Sig* sig, size_t vec_id, double val);
void sig_print(Sig* sig, size_t vec_id);
size_t sig_getdim(Sig* sig);
size_t sig_getlen(Sig* sig);

#endif
