#pragma once

/**
* @Brief		ѹ������ʹ�ýӿڶ��壨��ģʽ�ṹ���󣬲�֧��Ƕ�ף�ֻ�Դ������ֵ�������ڴ棩�����̰߳�ȫ��.
*
*/

#include <vector>
#include <tchar.h>
#include "traits.hpp"
#include "Common.hpp"
#include "MaxCompSize.hpp"
#include "AlgorithmComDefine.hpp"
#include "FieldCompress.hpp"

using namespace std;

namespace minicc
{

///����ѹ����.
class CSimpleCompressor : NonCopyable
{
public:
	CSimpleCompressor()
	{
	}
	~CSimpleCompressor()
	{
	}

public:

	///��ȡ�㷨���ѹ�����ݴ�С.
	uint32_t GetMaxCompSize(uint32_t unSrcDataSize, uint32_t unStructItems)
	{
		///�����󳤶�����.
		if (OverLimitDataLength(unSrcDataSize)) {
			return MINICC_OverLimitDataLength;
		}

		return unSrcDataSize + (unSrcDataSize / 4) + (unStructItems * 6) + 8;
	}

	///��ȡ�㷨���ѹ�����ݴ�С.
	template<typename T>
	int GetMaxCompSize(T&& t, uint32_t &unMaxCompSize)
	{
		static CMaxCompSize maxCompSize;
		return maxCompSize.GetMaxCompSize(t, unMaxCompSize);
	}

	//�ṹ��ָ����������ѹ����������ṹ��Ƕ��(�ڲ������ڴ�).
	template<typename T>
	typename std::enable_if<is_normal_class<T>::value, int>::type Compress(T *pT, uint32_t unItems,\
		const char *pDst, uint32_t unDstlen, uint32_t &unComplen)
	{
		int nRetCode = MINICC_InvalidParams;
		T *pPrevT = nullptr;
		unComplen = 0;
		uint32_t unOrigDstlen = unDstlen;

		for (uint32_t i = 0; i < unItems; i++) { 
			nRetCode = CompressImpl(&pT[i], pPrevT, (char *&)pDst, unDstlen);
			CHECK_RETCODE(nRetCode);
			pPrevT = &pT[i];
		}
		unComplen = unOrigDstlen - unDstlen;

		return nRetCode;
	}

	//�ṹ��ָ�����ݶԱ�ѹ����������ṹ��Ƕ��(�ڲ������ڴ�).
	template<typename T>
	typename std::enable_if<is_normal_class<T>::value, int>::type Compress(T *pT, T *pPrevT,\
		const char *pDst, uint32_t unDstlen, uint32_t &unComplen)
	{
		int nRetCode = MINICC_InvalidParams;
		unComplen = 0;
		uint32_t unOrigDstlen = unDstlen;

		nRetCode = CompressImpl(pT, pPrevT, (char *&)pDst, unDstlen);
		CHECK_RETCODE(nRetCode);
		unComplen = unOrigDstlen - unDstlen;

		return nRetCode;
	}

	//�������ǽṹ��ʵ������ѹ����������ṹ��Ƕ��(�ڲ������ڴ�).
	template<typename T>
	int Compress(T&& t,\
		const char *pDst, uint32_t unDstlen, uint32_t &unComplen)
	{
		int nRetCode = MINICC_InvalidParams;
		unComplen = 0;
		uint32_t unOrigDstlen = unDstlen;

		nRetCode = CompressImpl(std::forward<T>(t), (char *&)pDst, unDstlen);
		CHECK_RETCODE(nRetCode);
		unComplen = unOrigDstlen - unDstlen;

		return nRetCode;
	}

private:

	///ѹ��ʵ����ں���.
	template<typename T>
	int CompressImpl(T&& t, char *&pDst, uint32_t &unDstlen)
	{
		return CompressObject(std::forward<T>(t), pDst, unDstlen);
	}

	///�ṹ�����ѹ��ʵ����ں���(��ǰ���ݶ�����жԱ�ѹ��).
	template<typename T>
	int CompressImpl(T *pT, T *pPrevT, char *&pDst, uint32_t &unDstlen)
	{
		if (!pPrevT) {
			return CompressObject(*pT, pDst, unDstlen);
		}
		else {
			return CompressObject(*pT, *pPrevT, pDst, unDstlen);
		}
	}

	//ѹ���Զ���ṹ������(���ṹ��ѹ��).
	template<typename T>
	typename std::enable_if<is_user_class<T>::value, int>::type CompressObject(T&& t,\
		char *&pDst, uint32_t &unDstlen)
	{
		//std::cout << "begin to compress Tuple object !" << t.Meta()._Mysize << std::endl;
		///ѹ���ṹͼ�ֶζԱ�ѹ��map����ǰ�汾Ҫ��ѹ����ѹ�Ľṹ��ȫƥ�䣩.
		char *pMapData = (char *)pDst;
		int nBytes = SetMapSizeByAmount(pMapData, unDstlen, (int)t.Meta()._Mysize);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressTuple(t.Meta(), pMapData, pDst, unDstlen);
	}

	//ѹ���Զ���ṹ������(���ṹ��ѹ��).
	template<typename T>
	typename std::enable_if<is_user_class<T>::value, int>::type CompressObject(T&& t, T&& prevT,\
		char *&pDst, uint32_t &unDstlen)
	{
		//std::cout << "begin to compress Tuple object !" << t.Meta()._Mysize << std::endl;
		///ѹ���ṹͼ�ֶζԱ�ѹ��map����ǰ�汾Ҫ��ѹ����ѹ�Ľṹ��ȫƥ�䣩.
		char *pMapData = (char *)pDst;
		int nBytes = SetMapSizeByAmount(pMapData, unDstlen, (int)t.Meta()._Mysize);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressTuple(t.Meta(), prevT.Meta(), pMapData, pDst, unDstlen);
	}

	//ѹ��tuple.
	template<typename T>
	typename std::enable_if<is_tuple<T>::value, int>::type CompressObject(T&& t,\
		char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
		return CompressTuple(t, pMapData, pDst, unDstlen);
	}

	template<std::size_t I = 0, typename Tuple>
	typename std::enable_if<I == std::tuple_size<Tuple>::value, int>::type CompressTuple(const Tuple& t,\
		char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
		//std::cout << "tuple_size<Tuple> == 0\r\n" << std::endl;
		return MINICC_OK;
	}

	template<std::size_t I = 0, typename Tuple>
	typename std::enable_if<I == std::tuple_size<Tuple>::value, int>::type CompressTuple(const Tuple& t, const Tuple& prevT,\
		char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
		//std::cout << "tuple_size<Tuple> == 0\r\n" << std::endl;
		return MINICC_OK;
	}

	template<std::size_t I = 0, typename Tuple>
	typename std::enable_if < I < std::tuple_size<Tuple>::value, int>::type CompressTuple(const Tuple& t,\
		char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
		int nRetCode = m_FieldCompress.Compress_First(std::get<I>(t), I, pMapData, pDst, unDstlen);
		CHECK_RETCODE(nRetCode);
		nRetCode = CompressTuple<I + 1>(t, pMapData, pDst, unDstlen);
		CHECK_RETCODE(nRetCode);

		return nRetCode;
	}

	template<std::size_t I = 0, typename Tuple>
	typename std::enable_if < I < std::tuple_size<Tuple>::value, int>::type CompressTuple(const Tuple& t, const Tuple& prevT,\
		char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
		int nRetCode = m_FieldCompress.Compress_Next(std::get<I>(t), std::get<I>(prevT), I, pMapData, pDst, unDstlen);
		CHECK_RETCODE(nRetCode);
		nRetCode = CompressTuple<I + 1>(t, prevT, pMapData, pDst, unDstlen);
		CHECK_RETCODE(nRetCode);

		return nRetCode;
	}


	/*************ѹ������*************/

	//ѹ����map����.
	template<typename T>
	typename std::enable_if<is_singlevalue_container<T>::value, int>::type CompressObject(T&& t,\
		char *&pDst, uint32_t &unDstlen)
	{
		return CompressArray(t, pDst, unDstlen);
	}

	//ѹ��[stack/priority_queue]������.
	template<typename T>
	typename std::enable_if<is_stack<T>::value || is_priority_queue<T>::value, int>::type CompressObject(T&& t,\
		char *&pDst, uint32_t &unDstlen)
	{
		return CompressAdapter(t, [&]{ return t.top(); }, pDst, unDstlen);
	}

	///��ǰ�������漰�ƻ�����ṹ��Ϣ���ݲ�֧�ֲ���.
	template<typename Adapter, typename F>
	inline int CompressAdapter(Adapter& v, F f, char *&pDst, uint32_t &unDstlen)
	{
		int nRetCode = MINICC_InvalidParams;

		///�������������������.
		int nBytes = Compress_UINT32(pDst, unDstlen, (uint32_t)v.size());
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		decltype(f()) *pT = nullptr;
		decltype(f()) *pPrevT = nullptr;

		for (size_t i = 0, size = v.size(); i < size; i++) {
			pT = &f();
			nRetCode = CompressObject(pT, pPrevT, i, pDst, unDstlen);
			CHECK_RETCODE(nRetCode);
			pPrevT = pT;
			v.pop();
		}

		return nRetCode;
	}

	template<typename T>
	typename std::enable_if<is_queue<T>::value, int>::type CompressObject(T&& t,\
		char *&pDst, uint32_t &unDstlen)
	{
		return CompressAdapter(t, [&]{ return t.front(); }, pDst, unDstlen);
	}


	/*************ѹ������*************/

	//ѹ����������.
	template<typename Array>
	typename std::enable_if<!is_string<Array>::value, int>::type CompressArray(Array& v,\
		char *&pDst, uint32_t &unDstlen)
	{
		int nRetCode = MINICC_InvalidParams;
		for (int i = 0; i < (int)v.size(); i++)
		{
			if (i > 0) {
				nRetCode = CompressObject(v[i], v[i - 1], i, pDst, unDstlen);
				CHECK_RETCODE(nRetCode);
			}
			else {
				nRetCode = CompressObject(v[i], i, pDst, unDstlen);
				CHECK_RETCODE(nRetCode);
			}
		}

		return nRetCode;
	}

	//�����ѹ��.
	template<typename T>
	typename std::enable_if<is_normal_class<T>::value, int>::type CompressObject(T && t, T &&prevT,\
		std::size_t nIndex, char *&pDst, uint32_t &unDstlen)
	{
		return CompressImpl(&t, &prevT, pDst, unDstlen);
	}

	template<typename T>
	typename std::enable_if<is_normal_class<T>::value && !is_pointer_ext<T>::value, int>::type CompressObject(T && t,\
		std::size_t nIndex, char *&pDst, uint32_t &unDstlen)
	{
		return CompressImpl(t, pDst, unDstlen);
	}

	//��ͨ��ָ�����ѹ��.
	template<typename T>
	typename std::enable_if<std::is_pointer<T>::value, int>::type CompressObject(T && t, T &&prevT,\
		std::size_t nIndex, char *&pDst, uint32_t &unDstlen)
	{
		return CompressImpl(*t, prevT, pDst, unDstlen);
	}

	template<typename T>
	typename std::enable_if<std::is_pointer<T>::value, int>::type CompressObject(T && t,\
		std::size_t nIndex, char *&pDst, uint32_t &unDstlen)
	{
		return CompressImpl(*t, pDst, unDstlen);
	}

	//ָ�����ѹ��.
	template<typename T>
	typename std::enable_if<is_smart_pointer<T>::value, int>::type CompressObject(T && t, T &&prevT,\
		std::size_t nIndex, char *&pDst, uint32_t &unDstlen)
	{
		return CompressImpl(t.get(), prevT.get(), pDst, unDstlen);
	}

	template<typename T>
	typename std::enable_if<is_smart_pointer<T>::value, int>::type CompressObject(T && t,\
		std::size_t nIndex, char *&pDst, uint32_t &unDstlen)
	{
		return CompressImpl(*t.get(), pDst, unDstlen);
	}

	//�ṹ�����Ա�ѹ��.
	template<typename T>
	typename std::enable_if<is_normal_class<T>::value, int>::type CompressObject(T *pT, T *pPrevT,\
		std::size_t nIndex, char *&pDst, uint32_t &unDstlen)
	{
		return CompressImpl(pT, pPrevT, pDst, unDstlen);
	}


private:
	///�ֶ�ѹ�������.
	CFieldCompress m_FieldCompress;
};

} // namespace minicc