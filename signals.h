#ifndef SIGNALS
#define SIGNALS

#if audio == 32
# define audio float
#elif audio == 64
# define audio double
#else
#error invalid audio size: it must be 32 or 64 bits
#endif

typedef struct Sig Sig;

void sig_alloc(Sig* sig, size_t dim, size_t len);
void sig_free(Sig* sig);
void sig_setconst(Sig* sig, size_t vec_id, double val);
void sig_print(Sig* sig, size_t vec_id);
size_t sig_getdim(Sig* sig);
size_t sig_getlen(Sig* sig);

#endif
