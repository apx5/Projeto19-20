#include <lazy.hpp>

/*
 * This example requires PThreads
 */

/*
 * <functional>
 *  function
 *
 * <iostream>
 *  cerr
 *  endl
 */
#include <functional>
#include <iostream>

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
