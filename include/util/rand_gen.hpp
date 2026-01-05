#ifndef __RANDGEN_HPP__
#define __RANDGEN_HPP__

#include <random>

class RandGen
{
    private:
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;  
    RandGen();

    public:
    static RandGen& get_instance();
	RandGen(const RandGen&) = delete;
	RandGen& operator=(const RandGen&) = delete;
    int get_rand_int();
};

#endif
