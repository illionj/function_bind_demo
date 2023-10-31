
#include "add.h"

int add(int a, int b)
{
    return a + b;
}

double reduce(double a, double b)
{
    return a - b;
}

int none_para()
{
    /**
     * @brief  do somethings
     */
    return -1;
}


/**
 * @brief  Set rules for int return values, throw an exception when less than 0
 * @tparam  
 * @param  t: 
 * @return true: 
 * @return false: 
 */
template <>
bool ADD::needThrowException(int t)
{
    return t < 0;
}


/**
 * @brief  Set rules for double return values, throw an exception when greater than 0
 * @tparam  
 * @param  t: 
 * @return true: 
 * @return false: 
 */
template <>
bool ADD::needThrowException(double t)
{
    return t > 0;
}


