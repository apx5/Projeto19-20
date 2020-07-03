#include "../../include/prelude.h"

#include <vector>
#include <iostream>
#include <functional>

#define bool2str(b) ((b) ? "true" : "false")

template <typename Int>
bool is_even (Int n)
{
	return n % 2 == 0;
}

template <typename Int>
bool is_odd (Int n)
{
	return n % 2 != 0;
}

int main (void)
{
	std::vector<int> v = { 1, 5, 8, 12, 18, 23, 5, 26 };

	//bool res = ranges::accumulate(v, true, [] (bool ret, auto x) { return ret && x > 0; });
	std::cout << "all even = " << bool2str(all(v, is_even<int>)) << std::endl;
	std::cout << "any even = " << bool2str(any(v, is_even<int>)) << std::endl;

	std::cout << "all (not . even) = " << bool2str(all(v, is_odd<int>)) << std::endl;
	std::cout << "any (not . even) = " << bool2str(any(v, is_odd<int>)) << std::endl;

	return 0;
}
