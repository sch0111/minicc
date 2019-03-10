#pragma once

/**
* @Brief		基础通用定义.
*/

#include <tuple>

///结构字段格式化.
#define META(...) auto Meta()->decltype(std::tie(__VA_ARGS__)){ return std::tie(__VA_ARGS__); }

namespace minicc
{
class NonCopyable
{
public:
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator = (const NonCopyable&) = delete; 
	NonCopyable() = default; 
};

} // namespace minicc