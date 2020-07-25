#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>

#include "prelude.h"

#define benchmark(str, func)                   \
  do {                                         \
    auto start = std::clock();                 \
    (void) func;                               \
    auto stop = std::clock();                  \
    auto duration = 1000                       \
                  * (stop - start)             \
                  / CLOCKS_PER_SEC;            \
    std::cout << str << ": " << duration       \
              << " ms" << std::endl;           \
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
