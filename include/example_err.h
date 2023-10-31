/**
 * @File Name: example_err.h
 * @Author : wxl   Email:???
 * @brief  just a example
 * @Version : 1.0
 * @Create Date : 2023-10-31
 *
 * @copyright Copyright (c) 2023  wxl
 */
#ifndef __EXCEPTION
#define __EXCEPTION
#include <functional>
#include <sstream>

/**
 * @brief
 */
enum class ExceptionLevel : size_t
{
	ignoreExcep, // Do not use exceptions
	promptExcep, // Print all exception logs, but do not throw
	throwExcep,	 // Default settings print information and throw exceptions
	silentExcep	 // Does not print information, only throws exceptions
};

class example_error : public std::runtime_error
{
public:
	example_error(const std::string &MainErrInfo) : std::runtime_error(MainErrInfo){};

	example_error(const std::string &MainErrInfo, const std::string &ErrCode, const std::string &ErrDetail)
		: std::runtime_error(MainErrInfo), ErrCode(ErrCode), ErrDetail(ErrDetail) {}

	std::string ErrCode;
	std::string ErrDetail;
	ExceptionLevel DefaultExcep = ExceptionLevel::throwExcep;
};

#endif //