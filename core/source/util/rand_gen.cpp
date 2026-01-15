#include "util/rand_gen.hpp"

RandGen::RandGen()
{
    std::mt19937 rng(std::random_device{}());
    this->rng = rng;
    std::uniform_int_distribution<int> dist(0, 10000);
    this->dist = dist; 
}

RandGen& RandGen::get_instance()
{
    static RandGen instance;
	return instance;
}

int RandGen::get_rand_int()
{
    return dist(rng);
}
