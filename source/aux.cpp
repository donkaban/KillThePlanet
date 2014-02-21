#include "minimath.h"
#include "utils.h"

#include <cstdlib>

std::mutex aux::globalLocker;
std::chrono::time_point<std::chrono::system_clock> timer::start_time = std::chrono::system_clock::now();

float rnd() {return static_cast<float>(rand())/static_cast<float>(RAND_MAX);}
