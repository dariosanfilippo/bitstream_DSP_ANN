#ifndef BITMATH
#define BITMATH

typedef struct FullAdder FullAdder;

void fulladder(Sig* in0, Sig* in1, Sig* in2, Sig* out0, Sig* out1,
    size_t in_vec_id0, size_t in_vec_id1, size_t in_vec_id2, 
        size_t out_vec_id0, size_t out_vec_id1);
FullAdder fulladder_samplewise(bool in0, bool in1, bool c_in);
void binaryadder(Sig* in0, Sig* in1, Sig* out, size_t in_vec_id0, 
    size_t in_vec_id1, size_t out_vec_id);
bool binaryadder_samplewise(bool in0, bool in1, bool* state);
void binarymultiplier(Sig* in0, Sig* in1, Sig* out, size_t in_vec_id0,
    size_t in_vec_id1, size_t out_vec_id);
void bi2uni(Sig* in, Sig* out, size_t in_vec_id, size_t out_vec_id);
void uni2bi(Sig* in, Sig* out, size_t in_vec_id, size_t out_vec_id);

#endif
