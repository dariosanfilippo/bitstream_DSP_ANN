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

void dsm1(Sig* in, Sig* out);
void dsm2(Sig* in, Sig* out);
void dsm3(Sig* in, Sig* out);
void fulladder(Sig* threeins, Sig* twoouts);
FullAdder fulladder_samplewise(bool in0, bool in1, bool c_in);
void binaryadder(Sig* twoins, Sig* out);
bool binaryadder_samplewise(bool in0, bool in1, bool* state);
void binarymultiplier(Sig* in, Sig* out);

#endif
