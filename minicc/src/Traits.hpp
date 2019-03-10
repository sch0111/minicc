#pragma once

/**
* @Brief		数据类型特化定义.
*
*/

#include <memory>
#include <type_traits>
#include <vector>
#include <array>
#include <queue>
#include <stack>

using namespace std;

namespace minicc
{
	template<typename T>
	using decay_t = typename std::decay<T>::type;

	template<typename T> std::false_type static check_tuple_size(...);
	template<typename T> std::true_type static  check_tuple_size(decltype(std::tuple_size<T>::value)*);

	template<typename T>
	struct has_tuple_size : decltype(check_tuple_size<T>(nullptr))
	{
	};

	template<typename T>
	struct has_begin_end
	{
	private:
		template<typename U> static auto Check(int) -> decltype(std::declval<U>().begin(), std::declval<U>().end(), std::true_type());
		template<typename U> static std::false_type Check(...);

	public:
		enum
		{
			value = std::is_same<decltype(Check<T>(0)), std::true_type>::value
		};
	};

	template <typename T>
	struct has_const_iterator
	{
	private:
		template<typename C> static std::true_type Check(typename C::const_iterator*);
		template<typename C> static std::false_type  Check(...); 
	public:
		enum
		{
			value = std::is_same<decltype(Check<T>(0)), std::true_type>::value
		}; 
	}; 

	template <typename T>
	struct has_mapped_type
	{
	private:
		template<typename C> static std::true_type Check(typename C::mapped_type*);
		template<typename C> static std::false_type  Check(...);
	public:
		enum
		{
			value = std::is_same<decltype(Check<T>(0)), std::true_type>::value
		};
	};

	template<typename T> struct is_poiner_extent : std::false_type{};
	template<typename T> struct is_poiner_extent<std::shared_ptr<T>> : std::true_type{};
	template<typename T> struct is_poiner_extent<std::unique_ptr<T>> : std::true_type{};
	template<typename T> struct is_poiner_extent<std::weak_ptr<T>> : std::true_type{};
} // namespace minicc

template<typename T>
struct is_string : std::integral_constant<bool, std::is_same<minicc::decay_t<T>, std::basic_string<TCHAR>>::value || std::is_same<minicc::decay_t<T>, TCHAR[]>::value>{};

template <typename T>
struct is_container : public std::integral_constant<bool, minicc::has_const_iterator<minicc::decay_t<T>>::value&&minicc::has_begin_end<minicc::decay_t<T>>::value>{};

template <typename T>
struct is_singlevalue_container : public std::integral_constant<bool, !std::is_array<minicc::decay_t<T>>::value&&!minicc::has_tuple_size<minicc::decay_t<T>>::value && is_container<minicc::decay_t<T>>::value&&!minicc::has_mapped_type<minicc::decay_t<T>>::value>{};

template <typename T>
struct is_map_container : public std::integral_constant<bool, is_container<minicc::decay_t<T>>::value&&minicc::has_mapped_type<minicc::decay_t<T>>::value>{};

template<typename T>
struct is_normal_class : std::integral_constant<bool, std::is_class<minicc::decay_t<T>>::value&&!is_string<T>::value && !minicc::is_poiner_extent<minicc::decay_t<T>>::value>
{};

template<typename T>
struct is_basic_type : std::integral_constant<bool, std::is_arithmetic<minicc::decay_t<T>>::value>
{};

template<typename T>
struct is_enum_type : std::integral_constant < bool, std::is_enum<minicc::decay_t<T>>::value>
{};

template<typename T>
struct is_smart_pointer : minicc::is_poiner_extent<minicc::decay_t<T>>{};

template<typename T>
struct is_pointer_ext : std::integral_constant<bool, std::is_pointer<minicc::decay_t<T>>::value || is_smart_pointer<minicc::decay_t<T>>::value>{};

template <typename T, template <typename...> class Template>
struct is_specialization_of : std::false_type {};

template <template <typename...> class Template, typename... Args>
struct is_specialization_of<Template<Args...>, Template> : std::true_type{};

template<typename T> struct is_tuple : is_specialization_of<minicc::decay_t<T>, std::tuple>{};
template<typename T> struct is_queue : is_specialization_of<minicc::decay_t<T>, std::queue>{};
template<typename T> struct is_stack : is_specialization_of<minicc::decay_t<T>, std::stack>{};
template<typename T> struct is_priority_queue : is_specialization_of<minicc::decay_t<T>, std::priority_queue>{};

template<typename T>
struct is_container_adapter : std::integral_constant<bool, is_queue<T>::value || is_stack<T>::value || is_priority_queue<T>::value>
{
};

template<typename T>
struct is_user_class : std::integral_constant<bool, is_normal_class<T>::value&&!is_container_adapter<T>::value&&!is_container<T>::value&&!is_tuple<T>::value>
{};

