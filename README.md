# function binding 

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A simple function binding demo that achieves the following basic features:

0. Header Only
   
1. No need for repetitive checks of return values to confirm the correct execution of functions.

2. All introduced libraries or functions have a unified exception handling approach.

3. For some libraries, negative numbers indicate errors, while for others, positive numbers indicate errors. Different meanings of return values between different libraries can be handled correctly.

4. Exception levels can be controlled to manage the exception throwing and handling process of the bound functions.

5. Newly bound functions have the same parameters, forms, and usage as the original function. The bound function adds an optional 'exception level' parameter at the end of the parameter list (defaulting to normal throw)."



#### *Basic Usage*

```cpp
#include "func_bind_T.h"
#include "add.h"

template <typename T>
void exceptionDo(T *p, example_error &e)
{
    /**
     * @brief  you can do somethings for member
     */
    if (p != nullptr)
        p->exp_times++;

    /**
     * @brief  or  example_error
     */
    std::cout << "e.what()=" << e.what() << '\n';
}

/**
 * @brief  If you get bored, use macros.
 */
#define varName(var) #var
#define L_BIND2(func2, space, c) lam_bind(func2, space::needThrowException<func_ret_t<decltype(func2)>>, exceptionDo<std::remove_pointer_t<decltype(c)>>, varName(func2), c)

/**
 * @brief  See add.cpp file for details
 *         Set rules for int return values, throw an exception when less than 0
 *         Set rules for double return values, throw an exception when greater than 0
 */

int (*padd)(int, int); // test function pointer
struct A
{

    A(int x)
    {
        exp_times = x;
        padd = add;

        add_exp = lam_bind(add, ADD::needThrowException<int>, exceptionDo<A>, varName(add), this);
        reduce_exp = L_BIND2(reduce, ADD, this);
        padd_exp = L_BIND2(padd, ADD, this);
        none_para_exp = L_BIND2(none_para, ADD, this);
    }
    function_exp<int(int, int)> add_exp;
    function_exp<int(int, int)> padd_exp;
    function_exp<double(double, double)> reduce_exp;
    function_exp<int()> none_para_exp;

    int exp_times = 0;

    template <typename T>
    friend void exceptionDo(T *p, example_error &e);
};

/**
 * @brief  Test whether basic functions are normal
 */
void test_basic_function(A &a)
{
    for (int i = 0; i < 10; i++)
    {
        try
        {

            auto res = a.reduce_exp(i, 5);
            std::cout << "reduce_exp=" << res << '\n';
            auto res1 = a.add_exp(i, -5);
            std::cout << "add_exp=" << res1 << '\n';
        }
        catch (const std::exception &e)
        {
            std::cout << "exp_times=" << a.exp_times << '\n';
        }
        std::cout << "\n";
    }
}

/**
 * @brief  Test whether the parameters of exception control are effective
 */
void test_ExceptionLevel(A &a, ExceptionLevel level)
{
    try
    {

        auto res = a.reduce_exp(90, 5, level);
        std::cout << "reduce_exp=" << res << '\n';
        auto res1 = a.add_exp(-90, 5, level);
        std::cout << "add_exp=" << res1 << '\n';
        auto res2 = a.padd_exp(-10, 5, level);
        std::cout << "padd_exp=" << res2 << '\n';
        auto res3 = a.none_para_exp(level);
        std::cout << "none_para_exp=" << res3 << '\n';
        std::cout << "test_ExceptionLevel end normally\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "exp_times=" << a.exp_times << '\n';
    }
}

int main()
{
    A a(0);

    std::cout << "\n---------------test_basic_function begin---------------\n";
    test_basic_function(a);
    std::cout << "\n---------------ignoreExcep begin---------------\n";
    test_ExceptionLevel(a, ExceptionLevel::ignoreExcep);
    std::cout << "\n---------------promptExcep begin---------------\n";
    test_ExceptionLevel(a, ExceptionLevel::promptExcep);
    std::cout << "\n---------------silentExcep begin---------------\n";
    test_ExceptionLevel(a, ExceptionLevel::silentExcep);
}
```
In this basic example:

A set of bound functions should be encapsulated within a class.
When exceptions occur, a friend template is needed to handle the exception cleanup.
A specific namespace with a set of functions is required to determine whether exceptions are thrown (with potentially various return value types).
The new functions are identical to the original functions, with a default parameter controlling exception behavior.
Of course, this is just a simple demo, demonstrating how to combine original C functions with exceptions in a rudimentary way, without considering factors like overhead.



#### *Result*

```cpp

---------------test_basic_function begin---------------
reduce_exp=-5
e.what()=Exception!!  -5=add(0,-5)
exp_times=1

reduce_exp=-4
e.what()=Exception!!  -4=add(1,-5)
exp_times=2

reduce_exp=-3
e.what()=Exception!!  -3=add(2,-5)
exp_times=3

reduce_exp=-2
e.what()=Exception!!  -2=add(3,-5)
exp_times=4

reduce_exp=-1
e.what()=Exception!!  -1=add(4,-5)
exp_times=5

reduce_exp=0
add_exp=0

e.what()=Exception!!  1.000000=reduce(6,5.000000)
exp_times=6

e.what()=Exception!!  2.000000=reduce(7,5.000000)
exp_times=7

e.what()=Exception!!  3.000000=reduce(8,5.000000)
exp_times=8

e.what()=Exception!!  4.000000=reduce(9,5.000000)
exp_times=9


---------------ignoreExcep begin---------------
reduce_exp=85
add_exp=-85
padd_exp=-5
test_ExceptionLevel end normally

---------------promptExcep begin---------------
e.what()=Exception!!  85.000000=reduce(90,5.000000)
reduce_exp=85
e.what()=Exception!!  -85=add(-90,5)
add_exp=-85
e.what()=Exception!!  -5=padd(-10,5)
padd_exp=-5
test_ExceptionLevel end normally

---------------silentExcep begin---------------
exp_times=12---------------test_basic_function begin---------------
reduce_exp=-5
e.what()=Exception!!  -5=add(0,-5)
exp_times=1

reduce_exp=-4
e.what()=Exception!!  -4=add(1,-5)
exp_times=2

reduce_exp=-3
e.what()=Exception!!  -3=add(2,-5)
exp_times=3

reduce_exp=-2
e.what()=Exception!!  -2=add(3,-5)
exp_times=4

reduce_exp=-1
e.what()=Exception!!  -1=add(4,-5)
exp_times=5

reduce_exp=0
add_exp=0

e.what()=Exception!!  1.000000=reduce(6,5.000000)
exp_times=6

e.what()=Exception!!  2.000000=reduce(7,5.000000)
exp_times=7

e.what()=Exception!!  3.000000=reduce(8,5.000000)
exp_times=8

e.what()=Exception!!  4.000000=reduce(9,5.000000)
exp_times=9


---------------ignoreExcep begin---------------
reduce_exp=85
add_exp=-85
padd_exp=-5
none_para_exp=-1
test_ExceptionLevel end normally

---------------promptExcep begin---------------
e.what()=Exception!!  85.000000=reduce(90,5.000000)
reduce_exp=85
e.what()=Exception!!  -85=add(-90,5)
add_exp=-85
e.what()=Exception!!  -5=padd(-10,5)
padd_exp=-5
e.what()=Exception!!  -1=none_para( )
none_para_exp=-1
test_ExceptionLevel end normally

---------------silentExcep begin---------------
exp_times=13

```

If all your original functions have at least one parameter, you may encounter a compilation warning when using this header file (warning: unused function 'printFolder' [-unused-function]). This is done for better demonstration purposes as I used fold expressions to print all input parameters and return values when exceptions occur. When the original functions have no parameters, there will be an unused static function 'printFolder'. There's no need to be concerned about this.
