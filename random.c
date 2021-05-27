#include "random.h"

audio frand(void) {
    return rand() / (audio) RAND_MAX;
}
