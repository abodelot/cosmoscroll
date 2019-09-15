#include "Math.hpp"

#include <ctime>

namespace math {

// Set the random numbers sequence seed with the current system time, so that it is always different
unsigned int static set_random_seed()
{
    unsigned int seed = static_cast<unsigned int>(time(NULL));
    std::srand(seed);
    return seed;
}


unsigned int g_seed = set_random_seed();


int rand(int begin, int end)
{
    return std::rand() % (end - begin + 1) + begin;
}


float rand(float begin, float end)
{
    return static_cast<float>(std::rand()) / RAND_MAX * (end - begin) + begin;
}


void set_seed(unsigned int s)
{
    std::srand(s);
    g_seed = s;
}

}
