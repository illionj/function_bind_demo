
#include "func_bind_T.hpp"
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