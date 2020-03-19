#include <cstddef>
#include <iostream>

#define NFIBS 10

size_t fib (size_t n)
{
    size_t cur = 0;
    size_t next = 1;

    for (size_t i = 1; i < n; i++) {
        size_t tmp = cur + next;
        cur = next;
        next = tmp;
    }

    return next;
}

template <typename T1, typename T2 = T1>
struct pair
{
    T1 fst;
    T2 snd;

    pair (const T1 & t1 = T1(), const T2 & t2 = T2()) :
        fst(t1),
        snd(t2)
    {
    }

    pair (const pair<T1, T2> & other) :
        fst(other.fst),
        snd(other.snd)
    {
    }

    pair<T2, T1> swap () const
    {
        return pair<T2, T1>(snd, fst);
    }

    template<typename T3, typename T4, typename T5, typename T6>
    pair<T5, T6> apply_ops (T5 op1 (T1 &, T3 &), T6 op2 (T2 &, T4 &), pair<T3, T4> & other) const
    {
        return pair<T5, T6>(op1(fst, other.fst), op2(snd, other.snd));
    }

    pair<T1, T2> operator+ (pair<T1, T2> & other) const
    {
        return this.apply_ops(std::plus<T1>(), std::plus<T2>(), other);
    }

    void operator+= (pair<T1, T2> & other)
    {
        fst += other.fst;
        snd += other.snd;
        //return pair<T1, T2>(fst, snd);
    }
};

int main (void)
{
    pair<size_t, size_t> fibs[NFIBS];

    for (size_t i = 0; i < NFIBS; i++)
        fibs[i].fst = i * i;

    for (size_t i = 0; i < NFIBS; i++)
        fibs[i].snd = fib(fibs[i].fst);

    for (size_t i = 0; i < NFIBS; i++)
        std::cerr << "fib(" << fibs[i].fst << ") = " << fibs[i].snd << std::endl;

    pair<size_t, size_t> ret(0, 0);

    for (size_t i = 0; i < NFIBS; i++)
        ret += fibs[i];

    std::cerr << ret.fst << ' ' << ret.snd << std::endl;

    return 0;
}
