/**
 * @File Name: func_bind_T.h
 * @Author : wxl   Email:???
 * @brief  
 * @Version : 1.0
 * @Create Date : 2023-10-31
 * 
 * @copyright Copyright (c) 2023  wxl
 */
#ifndef __EXCEPTION_T
#define __EXCEPTION_T
#include <functional>
#include <sstream>
#include "example_err.h"

/**
 * @brief  Get return value type
 * @tparam T 
 */
template <typename T>
struct FUNC_RET_T
{
};

template <typename T, typename... Args>
struct FUNC_RET_T<T (*)(Args...)>
{
	using type = T;
};

template <typename T, typename... Args>
struct FUNC_RET_T<T(Args...)>
{
	using type = T;
};

#ifdef _stdcall
template <typename T, typename... Args>
struct FUNC_RET_T<T(_stdcall *)(Args...)>
{
	using type = T;
};
#endif

template <typename T>
using func_ret_t = typename FUNC_RET_T<T>::type;


/**
 * @brief  Get callable object type
 * @tparam T 
 */
template <typename T>
struct FUNC_T
{
};
template <typename T, typename... Args>
struct FUNC_T<T(Args...)>
{
	using type = T(Args...);
};
template <typename T, typename... Args>
struct FUNC_T<T (*)(Args...)>
{
	using type = T(Args...);
};

template <typename T, typename... Args>
struct FUNC_T<std::function<T(Args...)>>
{
	using type = T (*)(Args...);
};

template <typename T>
using func_t = typename FUNC_T<T>::type;



/**
 * @brief  Sometimes it's necessary to develop for 32-bit devices, especially on the Windows platform,where '_stdcall' is quite common. 
 * In my case, I use 'function_exp_base' as the base class to derive 'function_exp' to distinguish functions that are called with the '_stdcall' calling convention.
 * @tparam T 
 */
template <typename T>
struct function_exp_base
{
};

template <typename T, typename... Args>
struct function_exp_base<T(Args...)>
{
	std::function<T(Args..., ExceptionLevel)> t;
	function_exp_base(){};
	template <typename lam>
	function_exp_base(lam t)
	{
		this->t = t;
	}
	auto operator()(Args... args, ExceptionLevel level = ExceptionLevel::throwExcep)
	{

		return t(args..., level);
	}

	function_exp_base &operator=(const function_exp_base &other)
	{
		if (this == &other)
		{
			return *this; 
		}

		t = other.t;
		return *this;
	}
};

template <typename T>
struct function_exp
{
};

#ifdef _stdcall
template <typename T, typename... Args>
struct function_exp<T _stdcall(Args...)> : public function_exp_base<T(Args...)>
{
	using function_exp_base::function_exp_base;
};
#endif

template <typename T, typename... Args>
struct function_exp<T(Args...)> : public function_exp_base<T(Args...)>
{
	// clang version 10.0.0-4ubuntu1 
		using function_exp_base<T(Args...)>::function_exp_base;
	// vs2022
	// using function_exp_base::function_exp_base;
};

template <typename T, typename... Args>
struct FUNC_T<function_exp<T(Args...)>>
{
	using type = T (*)(Args...);
};

#ifdef _stdcall

template <typename T, typename... Args>
struct FUNC_T<function_exp<T _stdcall(Args...)>>
{
	using type = T(_stdcall *)(Args...);
};

#endif


/**
 * @brief  fold expression for better printing results
 * @tparam T 
 * @param  os: 
 * @param  arg: 
 * @return auto: 
 */
template <typename T>
auto comma(std::ostringstream &os, T &arg)
{
	os << ',';

	return std::to_string(arg);
}

template <typename First, typename... Args>
void printFolder(std::ostringstream &os, const First &firstarg, const Args &...args)
{

	os << '(' << firstarg;
	(os << ... << comma(os, args)) << ")";
}

static void printFolder(std::ostringstream &os)
{
	os << "( )";
}



/**
 * @brief  The new binding function actually called
 * @tparam T primitive function type
 * @tparam G Type of function that determines whether to exclude exceptions
 * @tparam E_FUNC Type of function that determines whether to exclude exceptions
 * @tparam args original function parameters
 * @tparam C The type of class into which the bound function is encapsulated
 * @param  t: 
 * @param  g: 
 * @param  ef: 
 * @param  s: 
 * @param  c: 
 * @param  level: 
 * @param  res: 
 * @return decltype(t(res...)): 
 */
template <typename T, typename G, typename E_FUNC, typename... args, typename C>
auto funcAddException(T t, G g, E_FUNC ef, const char *s, C *c, ExceptionLevel level, args &&...res) -> decltype(t(res...))
{

	auto rc = t(res...);

	try
	{
		if (level != ExceptionLevel::ignoreExcep && g(rc))
		{

			std::ostringstream os;
			os << s;
			printFolder(os, res...);
			std::string Info = std::to_string(rc) + "=" + os.str();
			throw example_error(std::string("Exception!!  ") + Info);
		}
	}
	catch (example_error &e)
	{

		if (level != ExceptionLevel::silentExcep)
			ef(c, e);
		if (level != ExceptionLevel::promptExcep)
			throw;
	}
	return rc;
}


/**
 * 
 * @brief  Use lambda to bind functions
 * @tparam T primitive function type
 * @tparam G Type of function that determines whether to exclude exceptions
 * @tparam E_FUNC Type of function that determines whether to exclude exceptions
 * @tparam args original function parameters
 * @tparam C The type of class into which the bound function is encapsulated
 * @param  t: 
 * @param  g: 
 * @param  ef: 
 * @param  FuncName: 
 * @param  c: 
 * @return constexpr auto: 
 */
template <typename T, typename G, typename E_FUNC, typename... Args, typename C>
constexpr auto lam_bind(T (*t)(Args...), G g, E_FUNC ef, const char *FuncName, C *c)
{

	return [=](Args... args, ExceptionLevel level = ExceptionLevel::throwExcep)
	{
		return funcAddException(t, g, ef, FuncName, c, level, args...);
	};
}

#ifdef _stdcall

template <typename T, typename G, typename E_FUNC, typename... Args, typename C>
constexpr auto lam_bind(T(_stdcall *t)(Args...), G g, E_FUNC ef, const char *FuncName, C *c)
{

	return [=](Args... args, ExceptionLevel level = ExceptionLevel::throwExcep)
	{
		return funcAddException(t, g, ef, FuncName, c, level, args...);
	};
}
#endif

//-----------------------------------------------------------------------------------------------
#endif 
