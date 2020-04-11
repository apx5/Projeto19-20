From the book _"Functional Programming in C++"_ by Ivan Cukic:

 * (2.3) Optimise moving semantics for performant function composition
 * (3.1.1) What is [`std::forward`] for?
 * (3.1.1) Automatic return type deduction (`auto` & `decltype(auto)`); in
   general, automatic type deduction
 * (3.1.3) Call operator (`operator()`)
 * (3.2) Lambda and scoping specifiers
 * (3.4) [`std::function`]
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
 * [`constexpr`]: what's the difference from `const` and `static const`?

Questions that came up while playing with things:

 * [Reference][0] vs Pointer (`&` vs `*`)
     * A reference is never `NULL`, and therefore it's safe to dereference
       (hence autodereferencing) (assuming the object it points to is valid);
       It's still possible to create invalid references, for exemple, when
       returning the reference to a function's local variable;
     * A reference has some sort ["lifetime"][1] associated; when its scope
       ends, the object it points to is automatically `free`'d;
 * How to specify function types? Can we specify types _a la C_?
     * Yes, that has to be true, as a lot of C code can be compiled with a
       C++ compiler. What is then the use for `std::function`? And how to
       specify function types in template parameters? E.g., the prototype of
       `foreach` for vectors can be something like the following:
       `template <typename F, typename T> void foreach (const std::vector<T> & v, F func)`.
       This has the nicety that we can pass `foreach` whatever type of
       function, as long as inside `foreach`'s body, its use makes sense. This
       doesn't give any info to a user of `foreach`. What kind of functions can
       he use with it? Does he a get references to elements? Does he get
       `const` references to the elements? Does he get pointers? `const`
       pointers? Sure, if he doesn't give it an appropriately typed function
       a compile-time error is thrown; but what should he do to fix it?
 * What does `const` really mean to the compiler? In `void foo (const T & x)`
   does it assume `*x` is not changed outside of `foo`, or does it just mean
   `foo` itself will not change `*x`? Does it make a difference if `x` is a
   pointer instead of a reference? I.e., `const T * x` instead of `const T & x`?
 * What is `-fconcepts` and how is `template <typename T> void foo (T x) {/*...*/}`
   different from `void foo (auto x) {/*...*/}`?
     * (**BEWARE! SPECULATION AHEAD!**) Does it have to do with stack
       allocation? In the function template, the `x` parameter has a known size
       at compile-time, because a template function is instantiated for each
       different `T`; while in the case of the function with an `auto`
       parameter there's only one function, for all the different types.

# C++

Things to look at:

 * [FunctionalPlus]: C++ Functional Programming utilities library
 * [RangesV3]: compositional operations over various types of collections

In [`include/fplus/composition.hpp`] is defined `lazy :: ((a -> b), a) -> (() -> b)`,
a call operator that turns a non-nullary function into a nullary function with
the given arguments. The created function is not memoised, so each call of this
newly created function will in turn call the internal function. This may or may
not be the wanted behaviour. There are also memoisation utilities defined in
this header. Both could be combined to create a memoised lazy call of a given
function and its arguments. With a macro it would be possible to create a
(non-)memoised lazy evaluation utility for arbitrary expressions. For
performance reasons and implementation simplicity, it would be best to define
these lazy expression evaluation constructs on their own.

# Haskell

Things to look at:

 * [_Tackling the awkward squad_][simonpj]: Haskell solutions to real-world
   problems
 * [Parallel & Concurrent Programming Tutorial][parcon_tuto]: Source of the
   `chat-server` program in the playground

[`include/fplus/composition.hpp`]: https://github.com/Dobiasd/FunctionalPlus/blob/master/include/fplus/composition.hpp
[FunctionalPlus]: https://github.com/Dobiasd/FunctionalPlus
[RangesV3]: https://github.com/ericniebler/range-v3
[parcon_tuto]: https://github.com/simonmar/par-tutorial
[simonpj]: https://www.microsoft.com/en-us/research/publication/tackling-awkward-squad-monadic-inputoutput-concurrency-exceptions-foreign-language-calls-haskell

[`std::forward`]: https://en.cppreference.com/w/cpp/utility/forward
[`std::function`]: https://en.cppreference.com/w/cpp/utility/functional/function
[`constexpr`]: https://en.cppreference.com/w/cpp/language/constexpr

[0]: https://en.cppreference.com/w/cpp/language/reference
[1]: https://en.cppreference.com/w/cpp/language/lifetime
