#include "minimath.h"
#include "utils.h"

std::mutex aux::globalLocker;

float rnd() {return static_cast<float>(rand())/static_cast<float>(RAND_MAX);}
