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

    auto r3 = ranges::accumulate(v
            | ranges::views::transform([] (int x) { return 2 * x; }),
            0,
            std::plus<>());

    auto r4 = ranges::accumulate(v
            | ranges::views::filter([] (int n) { return !(n & 0x1); })
            | ranges::views::transform([] (int _) { return 1; }),
            0,
            std::plus<>());

    auto r5 = ranges::accumulate(v, 0, [] (int r, int _) { return r + 1; });

    std::cerr
        << "v = " << print_vector(v) << std::endl
        << "foldl (+) 0 v = " << r1 << std::endl
        << "foldl (*) 1 v = " << r2 << std::endl
        << "foldl (+) 0 . map (*2) $ v = " << r3 << std::endl
        << "foldl (+) 0 . map (const 1) . filter ((==0) . (flip mod 2)) $ v = " << r4 << std::endl
        << "foldl (curry ((+1) . fst)) 0 v = " << r5 << std::endl;
        ;

    return 0;
}
