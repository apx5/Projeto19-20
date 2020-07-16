#include <iostream>
#include "prelude.h"
#include <vector>
#include <cstdlib>
#include <string>
#include <fstream>
#include <chrono>
#include <tuple>

using namespace std::chrono;

#define benchmark(str, func) \
	do { \
	    auto start = high_resolution_clock::now(); \
	    (void) func; \
	    auto stop = high_resolution_clock::now(); \
	    auto duration = duration_cast<milliseconds>(stop - start); \
	    std::cout << str << ": " << duration.count() << " milliseconds" <<  std::endl; \
	} while (0)

int main (void)
{
    std::vector<int> input_vector;
    int n = 0;
    while (std::cin >> n)
	    input_vector.push_back(n);

    auto dbl = [] (int x){ return x + x;};
    auto even = [](int x) { return x % 2 == 0; };

    benchmark("filter even",               filter(even, input_vector));
    benchmark("map (*2)",                  map(dbl, input_vector));
    benchmark("reverse",                   reverse(input_vector));
    benchmark("uncurry zip . split id id", zip(input_vector, input_vector));

    return 0;
}
