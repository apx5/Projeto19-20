#include <algorithm>
#include <numeric>
#include <functional>
#include <iterator>

#define Container               \
  template <typename, typename> \
  class
#define Function typename
#define Predicate typename
#define Type typename
#define Number typename
#define Ordinal typename


//map :: (a -> b) -> [a] -> [b]
template <Function FN, Container CN, Type A,
          Type B = typename std::result_of<FN(A)>::type,
          typename AllocA = std::allocator<A>,
          typename AllocB = std::allocator<B>>
auto map(const FN& f, const CN<A, AllocA>& c) -> CN<B, AllocB> {
  auto res = CN<B, AllocB>{};
  std::transform(std::begin(c), std::end(c), std::back_inserter(res), f);
  return res;
}

//filter :: (a -> Bool) -> [a] -> [a]
template <Predicate PR, Container CN, Type A,
          typename AllocA = std::allocator<A>>
auto filter(const PR& p, const CN<A, AllocA>& c) -> CN<A, AllocA> {
  auto res = CN<A, AllocA>{};
  res.reserve(c.size());
  std::copy_if(std::begin(c), std::end(c), std::back_inserter(res), p);
  res.shrink_to_fit();
  return res;
}


// reverse :: [a] -> [a]
template <Container CN, Type A, typename AllocA = std::allocator<A>>
auto reverse(const CN<A, AllocA>& c) -> CN<A, AllocA> {
  auto res = CN<A, AllocA>{c};
  std::reverse(std::begin(res), std::end(res));
  return res;
}

// foldl :: (b -> a -> b) -> b -> [a] -> b
template <Function FN, Type B, Container CN, Type A,
          typename AllocA = std::allocator<A>>
auto foldl(const FN& f, B&& acc, const CN<A, AllocA>& c) -> B {
  return std::accumulate(std::begin(c), std::end(c), std::forward<B>(acc), f);
}

// sum :: Num a => [a] -> a
template <Container CN, Number A, typename AllocA = std::allocator<A>>
auto sum(const CN<A, AllocA>& c) -> A {
  return foldl([](const A& acc, const A& x) { return acc + x; }, A{0}, c);
}

// zip :: [a] -> [b] -> [(a, b)]
template <Container CA, Type A, typename AllocA = std::allocator<A>,
          Container CB, Type B, typename AllocB = std::allocator<B>,
          Container CRES = CA, typename RES = std::tuple<A, B>,
          typename AllocRES = std::allocator<RES>>
auto zip(const CA<A, AllocA>& left, const CB<B, AllocB>& right)
    -> CRES<RES, AllocRES> {
  auto res = CRES<RES, AllocRES>{};
  auto l = std::begin(left);
  auto r = std::begin(right);
  while (l != std::end(left) && r != std::end(right)) {
    res.emplace_back(*l, *r);
    ++l;
    ++r;
  }
  return res;
}
