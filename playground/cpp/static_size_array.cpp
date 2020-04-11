#include <cstddef>
#include <iostream>

/*
 * Static Size Array
 *
 * The `len` template parameter is really static; it can only be a value known
 * at compile-time. Maybe this restriction could be worked around with template
 * metaprogramming, by defining the natural numbers and operations on them.
 *
 * Are there other ways to improve this implementation?
 */
template <typename T, size_t len>
struct ssa
{
    T buf[len] = { T() };

    T & operator[] (size_t idx)
    {
        static T def = T();
        /* How to deal with index out of bounds? */
        //assert(idx <= len);
        return (idx <= len) ?
            buf[idx]:
            def;
    }

    template <size_t other_len>
    ssa<T, len + other_len> operator+ (ssa<T, other_len> & other) const
    {
        ssa<T, len + other_len> ret;

        for (size_t i = 0; i < len; i++)
            ret[i] = buf[i];

        for (size_t i = 0; i < other_len; i++)
            ret[len + i] = other[i];

        return ret;
    }
};


template <typename T, size_t len>
void foreach (ssa<T, len> & buf, void f (const T&))
{
    for (size_t i = 0; i < len; i++)
        f(buf[i]);
}

template <typename T>
void f (const T & t)
{
    std::cerr << t << " ";
}

int main (void)
{
    ssa<int, 5> arr({ 1, 2, 3, 4, 5 });
    ssa<int, 10> arr2 = arr + arr;

    foreach(arr, f);
    std::cerr << std::endl;

    foreach(arr2, f);
    std::cerr << std::endl;

    return 0;
}
