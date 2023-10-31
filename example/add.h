/**
 * @File Name: add.h
 * @Author : wxl   Email:???
 * @brief  A simple test material, including a namespace to determine 
 * whether an exception is thrown and two original functions binded.
 * @Version : 1.0
 * @Create Date : 2023-10-31
 * 
 * @copyright Copyright (c) 2023  wxl
 */
#ifndef __ADD
#define __ADD
#include <string>
#include <iostream>

namespace ADD
{
	template <typename T>
	bool needThrowException(T t)
	{
		return false;
	}

	template<>
	bool needThrowException(int t);

    	template<>
	bool needThrowException(double t);
}

int add(int a, int b); 
double reduce(double a, double b);
int none_para();





#endif