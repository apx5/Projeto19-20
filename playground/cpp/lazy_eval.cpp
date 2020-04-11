/*
 * This example needs PThreads
 */

/*
 * <functional>
 *  function
 *
 * <iostream>
 *  cerr
 *  endl
 *
 * <mutex>
 *  once_flag
 *
 * <optional>
 *  optional
 */
#include <functional>
#include <iostream>
#include <mutex>
#include <optional>

/*
 * TODO: We know that a function must of type `RetType(*func)(void)`, i.e., a
 * nullary function of a certain return type. Can we change the template
 * argument from the function type itself to the return type of the fuction?
 * Then it'd be possible to specialize lazy_funcall<void>.
 */
template <typename F>
class lazy_funcall
{
    /*
     * NOTE: RetType *MUST NOT* be `void`!
     *
     * TODO: Why is this and how to work around it? The book's implementation
     *       requires the RetType to have a default constructor; This
     *       implementation requires RetType not to be `void`; Is there a way
     *       to overcome both of these limitations?
     *       EDIT: Maybe! See TODO above!
     */
    const F func;
    typedef decltype(func()) RetType;
    mutable std::optional<RetType> ret;
    mutable std::once_flag call_once_flag;

public:
    lazy_funcall (F f) : func(f) { }

    const RetType & operator() () const
    {
        std::call_once(call_once_flag, [this] { ret = func(); });
        return ret.value();
    }

    /* Why isn't it possible to have two overloads? */
    //RetType operator() () const
    //{
    //    std::call_once(call_once_flag, [this] { ret = func(); });
    //    return ret.value();
    //}
};

template <typename F>
lazy_funcall<F> delay_funcall (F && f)
{
    return lazy_funcall<F>(std::forward<F>(f));
}

/**
 * @brief Delay evaluation of expressions and blocks of code
 * @param capt A lambda's captures (defaults to `[&]`)
 * @param expr An expression
 * @param block A block of code
 *
 * delays suffixed with an underscore require the capture of variables to be
 *     specified.
 *
 * TODO: Is there a way to not differentiate between the two?
 */
#define delay_(     capt, block) delay_funcall(capt() block)
#define delay_expr_(capt, expr)  delay_(capt,         { return (expr); })
#define delay_expr(       expr)  delay_expr_([&],     (expr))
#define delay(            block) delay_([&],          block)

int func ()
{
    std::cerr << "Doing stuff inside `func`" << std::endl;
    return 42;
}

int main (void)
{
    auto f = delay_funcall(func);
    auto delayed_expr = delay_expr(f());
    auto delayed_block = delay({
            int x = 1;
            int y = 2;
            return x + y;
            });
    //auto void_returning_func = delay({ std::cerr << "This function returns nothing" << std::endl; });

    (void) f(); // discard the first call (to keep output clean)

    for (unsigned i = 0; i < 10; i++) {
        std::cerr << "f()             = " << f()             << std::endl
                  << "delayed_expr()  = " << delayed_expr()  << std::endl
                  << "delayed_block() = " << delayed_block() << std::endl;
    }

    return 0;
}
