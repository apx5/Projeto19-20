From the book _"Functional Programming in C++"_ by Ivan Cukic:

 * (2.3) Optimise moving semantics for performant function composition
 * (3.1.1) What is `std::forward` for?
 * (3.1.1) Automatic return type deduction (`auto` & `decltype(auto)`); in
   general, automatic type deduction
 * (3.1.3) Call operator (`operator()`)
 * (3.2) Lambda and scoping specifiers
 * (3.4) `std::function`
 * (4.1.2, 4.1.5) `std::bind` performance worse than lambdas
 * (4.2) Currying & partial function application: is it worth it?
 * (5.4, 5.5) Mutable vs Immutable & `const`
 * (5.5.1) Interface vs Internal `const`ness
 * (5.5.2) Composition again but now for immutable data structures
 * (5.5.3) Named Return Value Optimisation (NRVO): Why can't the compiler move
   `const` return values?
 * (6.1) Take inspiration from Scheme's `delay` implementation and improve
   `lazy_val`
 * (6.4.1) Is it possible to do some kind of Copy-on-Write (COW)? (@see 8.2)
 * `constexpr`: what's the difference from `const` and `static const`?

Questions that came up while playing with things:

 * Reference vs Pointer (`&` vs `*`)
 * How to specify function types? Can we specify types _a la C_?
 * What does `const` really mean to the compiler? In `void foo (const T & x)`
   does it assume `*x` is not be changed outside of `foo` or does it just mean
   `foo` itself will not change `*x`? Does it make a difference if `x` was a
   pointer isntead of a reference? I.e., `const T * x` instead of `const T & x`?
 * What is `-fconcepts` and how is `template <typename T> void foo (T x) {/*...*/}`
   different from `void foo (auto x) {/*...*/}`?
