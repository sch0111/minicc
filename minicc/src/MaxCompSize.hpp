#pragma once

/**
* @Brief		ѹ���������ѹ����С����.
*
*/

#include <vector>
#include <tchar.h>
#include "traits.hpp"
#include "Common.hpp"
#include "AlgorithmComDefine.hpp"

using namespace std;

namespace minicc
{
///����ѹ����.
class CMaxCompSize : NonCopyable
{
public:
	CMaxCompSize()
	{
	}
	~CMaxCompSize()
	{
	}

public:

	///��ȡ�㷨���ѹ�����ݴ�С��.
	template<typename T>
	int GetMaxCompSize(T&& t, uint32_t &unMaxCompSize)
	{
		uint32_t unSrcDataSize = 0;
		uint32_t unStructItems = 0;

		int nRetCode = StatisticsImpl(std::forward<T>(t), unSrcDataSize, unStructItems, true);
		CHECK_RETCODE(nRetCode);

		///�����󳤶�����.
		if (OverLimitDataLength(unSrcDataSize)) {
			return MINICC_OverLimitDataLength;
		}

		unMaxCompSize = unSrcDataSize + (unSrcDataSize / 4) + (unStructItems * 6) + 8;
#ifdef PRINT_DEBUG_INFO
		cout << "= > CMaxCompSize GetMaxCompSize(T&& t) unSrcDataSize:" << unSrcDataSize << " unStructItems:" << unStructItems << " unMaxCompSize:" << unMaxCompSize << endl;
#endif
		return nRetCode;
	}

	///��ȡ�㷨���ѹ�����ݴ�С.
	template<typename T>
	int GetMaxCompSize(T&& t, uint32_t &unSrcDataSize, uint32_t &unMaxCompSize)
	{
		uint32_t unStructItems = 0;
		int nRetCode = StatisticsImpl(std::forward<T>(t), unSrcDataSize, unStructItems, true);
		CHECK_RETCODE(nRetCode);

		///�����󳤶�����.
		if (OverLimitDataLength(unSrcDataSize)) {
			return MINICC_OverLimitDataLength;
	    }

		unMaxCompSize = unSrcDataSize + (unSrcDataSize / 4) + (unStructItems * 6) + 8;
#ifdef PRINT_DEBUG_INFO
		cout << "= > CMaxCompSize GetMaxCompSize(T&& t) unSrcDataSize:" << unSrcDataSize << " unStructItems:" << unStructItems << " unMaxCompSize:" << unMaxCompSize << endl;
#endif
		return nRetCode;
	}

private:

	template<typename T>
	int StatisticsImpl(T&& t, uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		return ErgodicObject(std::forward<T>(t), unSrcDataSize, unStructItems, bRootNode);
	}

	template<typename T>
	typename std::enable_if<is_user_class<T>::value, int>::type ErgodicObject(T&& t,\
		uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		unStructItems += 1;
		if (bRootNode) {
			unSrcDataSize += sizeof(t);
		}

		return ErgodicTuple(t.Meta(), unSrcDataSize, unStructItems);
	}

	template<typename T>
	typename std::enable_if<is_tuple<T>::value, int>::type ErgodicObject(T&& t,\
		uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		return ErgodicTuple(t, unSrcDataSize, unStructItems);
	}

	template<std::size_t I = 0, typename Tuple>
	typename std::enable_if<I == std::tuple_size<Tuple>::value, int>::type ErgodicTuple(const Tuple& t,\
		uint32_t &unSrcDataSize, uint32_t &unStructItems)
	{
		return MINICC_OK;
	}

	template<std::size_t I = 0, typename Tuple>
	typename std::enable_if < I < std::tuple_size<Tuple>::value, int>::type ErgodicTuple(const Tuple& t,\
		uint32_t &unSrcDataSize, uint32_t &unStructItems)
	{
		int nRetCode = MINICC_OK;
		nRetCode = ErgodicObject(std::get<I>(t), I, unSrcDataSize, unStructItems, false);
		CHECK_RETCODE(nRetCode);
		nRetCode = ErgodicTuple<I + 1>(t, unSrcDataSize, unStructItems);
		CHECK_RETCODE(nRetCode);

		return nRetCode;
	}

	/*************����*************/

	//��map����.
	template<typename T>
	typename std::enable_if<is_singlevalue_container<T>::value, int>::type ErgodicObject(T&& t,\
		uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
#ifdef PRINT_DEBUG_INFO
		cout << "= > CMaxCompSize is_singlevalue_container size:" << t.size() << endl;
#endif		
		///����¼���б�������ĳ��ȣ��˴�����ӽṹ���ܳ����м�ȥ.
		unSrcDataSize -= sizeof(t);

		return ErgodicArray(t, unSrcDataSize, unStructItems, true);
	}

	//[stack/priority_queue]������.
	template<typename T>
	typename std::enable_if<is_stack<T>::value || is_priority_queue<T>::value, int>::type ErgodicObject(T&& t,\
		uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
#ifdef PRINT_DEBUG_INFO
		cout << "= > CMaxCompSize stack / priority_queue size:" << t.size() << endl;
#endif	
		return ErgodicAdapter(t, [&]{ return t.top(); }, unSrcDataSize, unStructItems, true);
	}

	template<typename Adapter, typename F>
	inline int ErgodicAdapter(Adapter& v, F f,\
		uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		///���ڵ�����������䶯�ڴ棬�Ҵ��ڲ��������޷���������⣬��ǰ�ݲ�֧�ֵ�������ѹ������.
		int nRetCode = MINICC_IllegalDataType;

#ifdef PRINT_DEBUG_INFO
		cout << "= > CMaxCompSize ErgodicAdapter(Adapter& v, F f) ��ǰ��֧�ֵ�����ѹ��" << endl;
#endif	
		return nRetCode;
	}

	template<typename T>
	typename std::enable_if<is_queue<T>::value, int>::type ErgodicObject(T&& t,\
		uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
#ifdef PRINT_DEBUG_INFO
		cout << "= > CMaxCompSize is_queue size:" << t.size() << endl;
#endif	
		return ErgodicAdapter(t, [&]{ return t.front(); }, unSrcDataSize, unStructItems, true);
	}

	template<typename T>
	typename std::enable_if<is_map_container<T>::value, int>::type ErgodicObject(T&& t,\
		uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
#ifdef PRINT_DEBUG_INFO
		cout << "= > CMaxCompSize is_map_container size:" << t.size() << endl;
#endif	
		int nRetCode = MINICC_OK;
		uint32_t index = 0;
		for (auto& pair : t) {
			nRetCode = ErgodicObject(pair.first, index, unSrcDataSize, unStructItems, bRootNode);
			CHECK_RETCODE(nRetCode);
			nRetCode = ErgodicObject(pair.second, index, unSrcDataSize, unStructItems, bRootNode);
			CHECK_RETCODE(nRetCode);
			index ++;
		}

		return nRetCode;
	}

	/*************����*************/

	//std::array
	template<typename T, size_t N>
	int ErgodicObject(std::array<T, N>& t,\
		uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		return ErgodicArray(t, unSrcDataSize, unStructItems, true);
	}

	template <typename T, size_t N>
	int ErgodicObject(T(&t)[N], uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		return ErgodicArray(t, unSrcDataSize, unStructItems, bRootNode);
	}

	template <size_t N>
	int ErgodicObject(TCHAR(&t)[N], uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		return ErgodicArray(std::basic_string<TCHAR>(t), unSrcDataSize, unStructItems, false);
	}

	template<typename Array>
	typename std::enable_if<!is_string<Array>::value, int>::type ErgodicArray(Array& v,\
		uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		int nRetCode = MINICC_OK;
		uint32_t index = 0;
		for (auto i : v) {
			nRetCode = ErgodicObject(i, index, unSrcDataSize, unStructItems, bRootNode);
			CHECK_RETCODE(nRetCode);
			index ++;
		}

		return nRetCode;
	}

	template<typename Array>
	typename std::enable_if<is_string<Array>::value, int>::type ErgodicArray(Array& v,\
		uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		int nRetCode = MINICC_OK;
		uint32_t index = 0;
		for (auto i : v) {
			nRetCode = ErgodicObject(i, index, unSrcDataSize, unStructItems, bRootNode);
			CHECK_RETCODE(nRetCode);
			index ++;
		}

		return nRetCode;
	}

	template<typename T>
	typename std::enable_if<is_basic_type<T>::value, int>::type ErgodicObject(T&& t,\
		uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		return MINICC_OK;
	}


	template<typename T>
	typename std::enable_if<is_normal_class<T>::value && !is_pointer_ext<T>::value, int>::type ErgodicObject(T && t,\
		std::size_t nIndex, uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		return StatisticsImpl(std::forward<T>(t), unSrcDataSize, unStructItems, bRootNode);
	}

	template<typename T>
	typename std::enable_if<std::is_pointer<T>::value, int>::type ErgodicObject(T && t,\
		std::size_t nIndex, uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		return MINICC_IllegalDataType;
	}

	template<typename T>
	typename std::enable_if<is_smart_pointer<T>::value, int>::type ErgodicObject(T&& t,\
		std::size_t nIndex, uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		return MINICC_IllegalDataType;
	}

	template<typename T>
	typename std::enable_if<is_basic_type<T>::value, int>::type ErgodicObject(T&& t, \
		std::size_t nIndex, uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		return MINICC_OK;
	}

	template<typename T>
	typename std::enable_if<is_string<T>::value, int>::type ErgodicObject(T&& t, \
		std::size_t nIndex, uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		unSrcDataSize += (uint32_t)t.size();
		return MINICC_OK;
	}

	template<typename T>
	typename std::enable_if<is_enum_type<T>::value, int>::type ErgodicObject(T&& t, \
		std::size_t nIndex, uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		return MINICC_OK;
	}

	template <typename T, size_t N>
	int ErgodicObject(T(&t)[N], std::size_t nIndex, uint32_t &unSrcDataSize, uint32_t &unStructItems, bool bRootNode)
	{
		return StatisticsImpl(t, unSrcDataSize, unStructItems, bRootNode);
	}
};
} // namespace minicc