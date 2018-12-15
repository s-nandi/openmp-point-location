#include <random>
#include <ctime>
#include "pt.h"

class random_point_generator
{
private:
    std::mt19937 generator;
    std::uniform_int_distribution <int> distribution;
public:
    random_point_generator(){}
    random_point_generator(int lb, int ub) : generator(time(0)), distribution(lb - 1, ub) {}

    pt getPoint()
    {
	ptT x = distribution(generator);
	ptT y = distribution(generator);

	return {x + ptT(0.3), y + ptT(0.7)};
    }
};
