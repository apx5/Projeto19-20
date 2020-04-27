#include <iostream>
#include <vector>

#include <range/v3/all.hpp>

template <typename T>
std::string print_vector (std::vector<T> & v)
{
    std::cerr << "{ ";
    for (auto & x : v) {
        std::cerr << x << ", ";
    }
    std::cerr << '}';
    return "";
}

//template <typename I, typename O>
//O sum (I range)
//{
//    return ranges::accumulate(range, 0, std::plus<>());
//}

int main (void)
{
    std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

    auto r1 = ranges::accumulate(v, 0, std::plus<>());
    auto r2 = ranges::accumulate(v, 1, std::multiplies<>());
    //auto r3 = sum(v | ranges::views::transform([](int x) {return 2*x;}));

    std::cerr
        << "v = " << print_vector(v) << std::endl
        << "sum v = " << r1 << std::endl
        << "product v = " << r2 << std::endl
        //<< "sum . map $ (*2) v = " << r3 << std::endl
        ;

    return 0;
}
