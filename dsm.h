/* 
 * *****************************************************************************
 *
 *      Delta-sigma modulators and arithmetic.
 *
 * *****************************************************************************
 */

#ifndef DSM
#define DSM

typedef struct FullAdder FullAdder;

void dsm1(double* in, double* out, size_t samples);
void dsm2(double* in, double* out, size_t samples);
void dsm3(double* in, double* out, size_t samples);
void fulladder(double** in, double** out, size_t samples);
FullAdder fulladder_samplewise(bool in0, bool in1, bool c_in);
void binaryadder(double** in, double* out, size_t samples);
bool binaryadder_samplewise(bool in0, bool in1, bool* state);
void binarymultiplier(Sig* in, Sig* out);

#endif
