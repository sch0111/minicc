#pragma once

/**
* @Brief		����ͨ�ö���.
*/

#include <tuple>

///�ṹ�ֶθ�ʽ��.
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