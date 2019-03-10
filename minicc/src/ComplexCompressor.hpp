#pragma once

/**
* @Brief		ѹ������ʹ�ýӿڶ���(����ģʽ�ṹ����ͨѶӦ�ã���֧��Ƕ�ף�֧��Ƕ�ף�������������ѹ��ѹ������)���Ƕ��̰߳�ȫ��.
*
*/

#include <vector>
#include <tchar.h>
#include "traits.hpp"
#include "Common.hpp"
#include "AlgorithmComDefine.hpp"
#include "FieldCompress.hpp"
#include "MaxCompSize.hpp"

using namespace std;

namespace minicc
{
///����ѹ����.
class CComplexCompressor : NonCopyable
{
public:
	CComplexCompressor()
	{
		m_unSrcDataSize = 0;
		m_unMaxCompDataSize = 0;
		m_unCompDataSize = 0;
		m_pData = nullptr;
	}
	~CComplexCompressor()
	{
		Reset();
	}

public:

	//�������ǽṹ��ʵ������ѹ��������ṹ��Ƕ�ף�����ͨѶ���ݰ�ѹ��ѹ��.
	template<typename T>
	int Compress(T&& t)
	{
		int nRetCode = InitParams(std::forward<T>(t));
		CHECK_RETCODE(nRetCode);

		uint32_t unDstlen = m_unMaxCompDataSize;
		char *pDst = m_pData;

		nRetCode = CompressImpl(std::forward<T>(t), nullptr, pDst, unDstlen);
		CHECK_RETCODE(nRetCode);
		m_unCompDataSize = m_unMaxCompDataSize - unDstlen;

#ifdef PRINT_DEBUG_INFO	
		PrintBufferByHex(m_pData, m_unCompDataSize);
		cout << endl;
#else
#ifdef _DEBUG
		std::cout << "= > CComplexCompressor Compress(T&& t) CompDataSize = " << m_unCompDataSize << " SrcDataSize = " << m_unSrcDataSize << " MaxCompDataSize = " << m_unMaxCompDataSize << std::endl;
		std::cout << "= > CComplexCompressor Compress(T&& t) ѹ���ȣ�" << m_unCompDataSize / (m_unSrcDataSize * 1.0) << " | " << (m_unSrcDataSize * 1.0) / m_unCompDataSize << std::endl;
#endif
#endif
		return nRetCode;
	}

	///��ȡѹ������ָ��.
	const char* GetString()
	{
		return m_pData;
	}

	///��ȡԴ���ݴ�С.
	int GetSrcDataSize()
	{
		return m_unSrcDataSize;
	}
	///��ȡ���ѹ�����ݴ�С.
	int GetMaxCompDataSize()
	{
		return m_unMaxCompDataSize;
	}
	///��ȡѹ�����ݴ�С.
	int GetCompDataSize()
	{
		return m_unCompDataSize;
	}

	///�����ʼ��.
	void Reset()
	{
		m_unSrcDataSize = 0;
		m_unMaxCompDataSize = 0;
		m_unCompDataSize = 0;

		m_unDataSize = 0;
		if (nullptr == m_pData) {
			delete[] m_pData;
			m_pData = nullptr;
		}
	}

private:

	///ѹ�����������ʼ��.
	template<typename T>
	int InitParams(T&& t)
	{
		m_unSrcDataSize = 0;
		m_unMaxCompDataSize = 0;
		m_unCompDataSize = 0;

		int nRetCode = m_maxCompSize.GetMaxCompSize(std::forward<T>(t), m_unSrcDataSize, m_unMaxCompDataSize);
		CHECK_RETCODE(nRetCode);

		if (m_unMaxCompDataSize > m_unDataSize)
		{
			if (m_pData) {
				delete[] m_pData;
				m_pData = nullptr;
			}

			m_pData = new char[m_unMaxCompDataSize];
			m_unDataSize = m_unMaxCompDataSize;
		}

#ifdef _DEBUG
		memset(m_pData, 0, m_unMaxCompDataSize);
#endif

#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor InitParams(T&& t) unSrcDataSize = " << m_unSrcDataSize << " unMaxCompSize = " << m_unMaxCompDataSize << std::endl;
#endif
		return MINICC_OK;
	}

	//�ṹ�����ѹ��ʵ����ں���.
	template<typename T>
	int CompressImpl(T && t, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
		return CompressObject(std::forward<T>(t), pMapData, pDst, unDstlen);
	}

	///�ṹ�����ѹ��ʵ����ں���(��ǰ���ݶ�����жԱ�ѹ��).
	template<typename T>
	int CompressImpl(T && t, T&& prevT, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
		return CompressObject(std::forward<T>(t), std::forward<T>(prevT), pMapData, pDst, unDstlen);
	}

	//ѹ���Զ���ṹ������(���ṹ��ѹ��).
	template<typename T>
	typename std::enable_if<is_user_class<T>::value, int>::type CompressObject(T&& t,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor begin to compress Tuple object, t.Meta()._Mysize = " << t.Meta()._Mysize << std::endl;
#endif
		///ѹ���ṹͼ�ֶζԱ�ѹ��map����ǰ�汾Ҫ��ѹ����ѹ�Ľṹ��ȫƥ�䣩.
		char *pTmpMapData = (char *)pDst;
		int nBytes = SetMapSizeByAmount(pTmpMapData, unDstlen, (int)t.Meta()._Mysize);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressTuple(t.Meta(), pTmpMapData, pDst, unDstlen);
	}
	template<typename T>
	typename std::enable_if<is_user_class<T>::value, int>::type CompressObject(T&& t, T&& prevT,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor begin to compress Tuple object, t.Meta()._Mysize = " << t.Meta()._Mysize << std::endl;
#endif
		///ѹ���ṹͼ�ֶζԱ�ѹ��map����ǰ�汾Ҫ��ѹ����ѹ�Ľṹ��ȫƥ�䣩.
		char *pTmpMapData = (char *)pDst;
		int nBytes = SetMapSizeByAmount(pTmpMapData, unDstlen, (int)t.Meta()._Mysize);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressTuple(t.Meta(), prevT.Meta(), pTmpMapData, pDst, unDstlen);
	}

	//ѹ��tuple.
	template<typename T>
	typename std::enable_if<is_tuple<T>::value, int>::type CompressObject(T&& t,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
		return CompressTuple(t, pMapData, pDst, unDstlen);
	}

	template<std::size_t I = 0, typename Tuple>
	typename std::enable_if<I == std::tuple_size<Tuple>::value, int>::type CompressTuple(const Tuple& t,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressTuple(const Tuple& t, ...) tuple_size<Tuple> == 0" << std::endl;
#endif
		return MINICC_OK;
	}

	template<std::size_t I = 0, typename Tuple>
	typename std::enable_if<I == std::tuple_size<Tuple>::value, int>::type CompressTuple(const Tuple& t, const Tuple& prevT,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressTuple(const Tuple& t, const Tuple& prevT, ...) tuple_size<Tuple> == 0" << std::endl;
#endif
		return MINICC_OK;
	}

	template<std::size_t I = 0, typename Tuple>
	typename std::enable_if < I < std::tuple_size<Tuple>::value, int>::type CompressTuple(const Tuple& t,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressTuple(const Tuple& t, ...) I = " << I << std::endl;
#endif
		int nRetCode = MINICC_OK;
		nRetCode = CompressObject(std::get<I>(t), I, pMapData, pDst, unDstlen);
		CHECK_RETCODE(nRetCode);
		nRetCode = CompressTuple<I + 1>(t, pMapData, pDst, unDstlen);
		CHECK_RETCODE(nRetCode);

		return nRetCode;
	}

	template<std::size_t I = 0, typename Tuple>
	typename std::enable_if < I < std::tuple_size<Tuple>::value, int>::type CompressTuple(const Tuple& t, const Tuple& prevT,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressTuple(const Tuple& t, const Tuple& prevT, ...) I = " << I << std::endl;
#endif
		int nRetCode = MINICC_OK;
		nRetCode = CompressObject(std::get<I>(t), std::get<I>(prevT), I, pMapData, pDst, unDstlen);
		CHECK_RETCODE(nRetCode);
		nRetCode = CompressTuple<I + 1>(t, prevT, pMapData, pDst, unDstlen);
		CHECK_RETCODE(nRetCode);

		return nRetCode;
	}


	/*************ѹ������*************/

	//ѹ����map����.
	template<typename T>
	typename std::enable_if<is_singlevalue_container<T>::value, int>::type CompressObject(T&& t,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor is_singlevalue_container CompressObject(T&& t ...) t.size = " << t.size() << std::endl;
#endif
		///�������������������.
		int nBytes = Compress_UINT32(pDst, unDstlen, (uint32_t)t.size());
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressArray(t, (int)t.size(), pMapData, pDst, unDstlen);
	}
	template<typename T>
	typename std::enable_if<is_singlevalue_container<T>::value, int>::type CompressObject(T&& t, T&& prevT,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor is_singlevalue_container CompressObject(T&& t, T&& prevT...) t.size = " << t.size() << " | " << prevT.size() << std::endl;
#endif
		///�������������������.
		int nBytes = Compress_UINT32(pDst, unDstlen, (uint32_t)t.size());
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressArray(t, (uint32_t)t.size(), pMapData, pDst, unDstlen);
	}


	///******����������ѹ������******

	template<typename Adapter, typename F>
	inline int CompressAdapter(Adapter& v, F f, \
		char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressAdapter v.size() = " << v.size() << std::endl;
#endif
		int nVSize = v.size();
		///���������������.
		int nRetCode = Compress_UINT32(pDst, unDstlen, nVSize);
		QuickCompEncodeAdd(pDst, unDstlen, nRetCode);

		///��������Ա�ѹ��ӳ���.
		char *pArrMapData = (char *)pDst;
		int nBytes = SetMapSizeByAmount(pArrMapData, unDstlen, nVSize);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		decltype(f()) *pT = nullptr;
		decltype(f()) *plastT = nullptr;

		for (size_t i = 0, nVSize = v.size(); i < nVSize; i++)
		{
			pT = &f();
			if (i > 0) {
				nRetCode = CompressObject(*pT, *plastT, i, pArrMapData, pDst, unDstlen);
				CHECK_RETCODE(nRetCode);
			}
			else {
				nRetCode = CompressObject(*pT, i, pArrMapData, pDst, unDstlen);
				CHECK_RETCODE(nRetCode);
			}

			plastT = pT;
			v.pop();
		}

		return nRetCode;
	}

	//ѹ��[stack/priority_queue]������.
	template<typename T>
	typename std::enable_if<is_stack<T>::value || is_priority_queue<T>::value, int>::type CompressObject(T&& t,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor is_stack | is_priority_queue CompressObject(T&& t ...) t.size = " << t.size() << std::endl;
#endif
		return CompressAdapter(t, [&]{ return t.top(); }, pDst, unDstlen);
	}
	template<typename T>
	typename std::enable_if<is_stack<T>::value || is_priority_queue<T>::value, int>::type CompressObject(T&& t, T&& prevT,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor is_stack | is_priority_queue CompressObject(T&& t, T&& prevT...) t.size = " << t.size() << " | " << prevT.size() << std::endl;
#endif
		return CompressAdapter(t, [&]{ return t.top(); }, pDst, unDstlen);
	}

	//���ж���.
	template<typename T>
	typename std::enable_if<is_queue<T>::value, int>::type CompressObject(T&& t,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor is_queue CompressObject(T&& t ...) t.size = " << t.size() << std::endl;
#endif
		return CompressAdapter(t, [&]{ return t.front(); }, pDst, unDstlen);
	}
	template<typename T>
	typename std::enable_if<is_queue<T>::value, int>::type CompressObject(T&& t, T&& prevT,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor is_queue CompressObject(T&& t, T&& prevT, ...) t.size = " << t.size() << " | " << prevT.size() << std::endl;
#endif
		return CompressAdapter(t, [&]{ return t.front(); }, pDst, unDstlen);
	}

	/*************ѹ������*************/

	//std::array
	template<typename T, size_t N>
	int CompressObject(std::array<T, N>& t, \
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(std::array<T, N>& t, ...) N = " << N  << std::endl;
#endif
		///�������������������.
		int nBytes = Compress_UINT32(pDst, unDstlen, N);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressArray(t, N, pMapData, pDst, unDstlen);
	}
	template<typename T, size_t N>
	int CompressObject(std::array<T, N>& t, std::array<T, N>& prevT,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(std::array<T, N>& t, std::array<T, N>& prevT, ...) N = " << N << std::endl;
#endif
		///�������������������.
		int nBytes = Compress_UINT32(pDst, unDstlen, N);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressArray(t, N, pMapData, pDst, unDstlen);
		//return CompressArray(t, N, prevT, N, pMapData, pDst, unDstlen);
	}

	//��������.
	template <typename T, size_t N>
	int CompressObject(T(&t)[N],\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(T(&t)[N], ...) N = " << N << std::endl;
#endif
		///�������������������.
		int nBytes = Compress_UINT32(pDst, unDstlen, N);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressArray(t, N, pMapData, pDst, unDstlen);
	}
	template <typename T, size_t N>
	int CompressObject(T(&t)[N], T(&prevT)[N],\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(T(&t)[N], T(&prevT)[N], ...) N = " << N << std::endl;
#endif
		///�������������������.
		int nBytes = Compress_UINT32(pDst, unDstlen, N);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressArray(t, N, pMapData, pDst, unDstlen);
		//return CompressArray(t, N, prevT, N, pMapData, pDst, unDstlen);
	}

	//ѹ���������ַ������飨��ǰΪ�����.
	template<typename Array>
	typename std::enable_if<!is_string<Array>::value, int>::type CompressArray(Array& v, int nVSize,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor enable_if<!is_string<Array> CompressArray(Array& v, ...) nVSize = " << nVSize << std::endl;
#endif
		int nRetCode = MINICC_OK;
		int nBytes = 0;

		/////�������������������.
		//nBytes = Compress_UINT32(pDst, unDstlen, nVSize);
		//QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		///��������Ա�ѹ��ӳ���.
		char *pArrMapData = (char *)pDst;
		nBytes = SetMapSizeByAmount(pArrMapData, unDstlen, nVSize);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		///�����������0�Ž���ѹ��.
		for (int i = 0; i < nVSize; i++)
		{
			if (i > 0) {
				nRetCode = CompressObject(v[i], v[i - 1], i, pArrMapData, pDst, unDstlen);
				CHECK_RETCODE(nRetCode);
			}
			else {
				nRetCode = CompressObject(v[i], i, pArrMapData, pDst, unDstlen);
				CHECK_RETCODE(nRetCode);
			}
		}

		return nRetCode;
	}

	///******ѹ���ṹ���ڶ���******.

	///ѹ�������.
	template<typename T>
	typename std::enable_if<is_normal_class<T>::value && !is_pointer_ext<T>::value, int>::type CompressObject(T && t, \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor enable_if<is_normal_class<T>::value && !is_pointer_ext<T> CompressObject(T && t, ...) nIndex = " << nIndex << std::endl;
#endif
		///����������ֶ�ѹ����ʾĬ����1.
		SetQuickCompEncode(pMapData, nIndex);

		return CompressImpl(std::forward<T>(t), pMapData, pDst, unDstlen);
	}
	template<typename T>
	typename std::enable_if<is_normal_class<T>::value && !is_pointer_ext<T>::value, int>::type CompressObject(T && t, T &&prevT, \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor enable_if<is_normal_class<T>::value && !is_pointer_ext<T>  CompressObject(T && t, T &&prevT, ...) nIndex = " << nIndex << std::endl;
#endif
		///����������ֶ�ѹ����ʾĬ����1.
		SetQuickCompEncode(pMapData, nIndex);
		return CompressImpl(std::forward<T>(t), std::forward<T>(prevT), pMapData, pDst, unDstlen);
	}


	//��ͨ��ָ�����ѹ��.
	template<typename T>
	typename std::enable_if<std::is_pointer<T>::value, int>::type CompressObject(T && t, \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor enable_if<std::is_pointer<T> CompressObject(T && t, ...) nIndex = " << nIndex << std::endl;
#endif
		return MINICC_IllegalDataType;
	}
	template<typename T>
	typename std::enable_if<std::is_pointer<T>::value, int>::type CompressObject(T && t, T &&prevT, \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor enable_if<std::is_pointer<T> CompressObject(T && t, T &&prevT, ...) nIndex = " << nIndex << std::endl;
#endif
		return MINICC_IllegalDataType;
	}

	//����ָ�����ѹ��.
	template<typename T>
	typename std::enable_if<is_smart_pointer<T>::value, int>::type CompressObject(T && t, \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor enable_if<is_smart_pointer<T> CompressObject(T && t, ...) nIndex = " << nIndex << std::endl;
#endif
		return MINICC_IllegalDataType;
	}
	template<typename T>
	typename std::enable_if<is_smart_pointer<T>::value, int>::type CompressObject(T && t, T &&prevT, \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor enable_if<is_smart_pointer<T> CompressObject(T && t, T &&prevT, ...) nIndex = " << nIndex << std::endl;
#endif
		return MINICC_IllegalDataType;
	}

	//ѹ����������.
	template<typename T>
	typename std::enable_if<is_basic_type<T>::value, int>::type CompressObject(T&& t, \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor enable_if<is_basic_type<T> CompressObject(T&& t, ...) nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_First(t, nIndex, pMapData, pDst, unDstlen);
	}
	template<typename T>
	typename std::enable_if<is_basic_type<T>::value, int>::type CompressObject(T&& t, T&& prevT, \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor enable_if<is_basic_type<T> CompressObject(T&& t, T&& prevT, ...) nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_Next(t, prevT, nIndex, pMapData, pDst, unDstlen);
	}

	//ѹ��string����.
	template<typename T>
	typename std::enable_if<is_string<T>::value, int>::type CompressObject(T&& t, \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor enable_if<is_string<T> CompressObject(T&& t, ...) nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_First(t, nIndex, pMapData, pDst, unDstlen);
	}
	template<typename T>
	typename std::enable_if<is_string<T>::value, int>::type CompressObject(T&& t, T&& prevT, \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor enable_if<is_string<T> CompressObject(T&& t, T&& prevT, ...) nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_Next(t, prevT, nIndex, pMapData, pDst, unDstlen);
	}

	//ѹ��ö������.
	template<typename T>
	typename std::enable_if<is_enum_type<T>::value, int>::type CompressObject(T&& t, \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor enable_if<is_enum_type<T> CompressObject(T&& t, ...) nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_First(t, nIndex, pMapData, pDst, unDstlen);
	}
	template<typename T>
	typename std::enable_if<is_enum_type<T>::value, int>::type CompressObject(T&& t, T&& prevT, \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor enable_if<is_enum_type<T> CompressObject(T&& t, T&& prevT, ...) nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_Next(t, prevT, nIndex, pMapData, pDst, unDstlen);
	}

	//******ѹ����������******.

	///�ǻ������Ͷ������ݣ��ṹ����ȣ�.
	template <typename T, size_t N>
	int CompressObject(T(&t)[N],\
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{	
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(T(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		///����������ֶ�ѹ����ʾĬ����1.
		SetQuickCompEncode(pMapData, nIndex);

		///�������������������.
		int nBytes = Compress_UINT32(pDst, unDstlen, N);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressArray(t, N, pMapData, pDst, unDstlen);
	}
	template <typename T, size_t N>
	int CompressObject(T(&t)[N], T(&prevT)[N],\
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(T(&t)[N], T(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		///����������ֶ�ѹ����ʾĬ����1.
		SetQuickCompEncode(pMapData, nIndex); 

		///�������������������.
		int nBytes = Compress_UINT32(pDst, unDstlen, N);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressArray(t, N, pMapData, pDst, unDstlen);
	}

	///ѹ������char����.
	template <size_t N>
	int CompressObject(char(&t)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(char(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_First(t, nIndex, pMapData, pDst, unDstlen);
	}
	template <size_t N>
	int CompressObject(char(&t)[N], char(&prevT)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(char(&t)[N], char(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_Next(t, prevT, nIndex, pMapData, pDst, unDstlen);
	}

	///ѹ������unsigned char����.
	template <size_t N>
	int CompressObject(unsigned char(&t)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(unsigned char(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_First(t, nIndex, pMapData, pDst, unDstlen);
	}
	template <size_t N>
	int CompressObject(unsigned char(&t)[N], unsigned char(&prevT)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(unsigned char(&t)[N], unsigned char(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_Next(t, prevT, nIndex, pMapData, pDst, unDstlen);
	}

	///ѹ������int16_t����.
	template <size_t N>
	int CompressObject(int16_t(&t)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(int16_t(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_First(t, nIndex, pMapData, pDst, unDstlen);
	}
	template <size_t N>
	int CompressObject(int16_t(&t)[N], int16_t(&prevT)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(int16_t(&t)[N], int16_t(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_Next(t, prevT, nIndex, pMapData, pDst, unDstlen);
	}

	///ѹ������uint16_t����.
	template <size_t N>
	int CompressObject(uint16_t(&t)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(uint16_t(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_First(t, nIndex, pMapData, pDst, unDstlen);
	}
	template <size_t N>
	int CompressObject(uint16_t(&t)[N], uint16_t(&prevT)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(uint16_t(&t)[N], uint16_t(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_Next(t, prevT, nIndex, pMapData, pDst, unDstlen);
	}

	///ѹ������int����.
	template <size_t N>
	int CompressObject(int(&t)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(int(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_First(t, nIndex, pMapData, pDst, unDstlen);
	}
	template <size_t N>
	int CompressObject(int(&t)[N], int(&prevT)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(int(&t)[N], int(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_Next(t, prevT, nIndex, pMapData, pDst, unDstlen);
	}

	///ѹ������uint32_t����.
	template <size_t N>
	int CompressObject(uint32_t(&t)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(uint32_t(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_First(t, nIndex, pMapData, pDst, unDstlen);
	}
	template <size_t N>
	int CompressObject(uint32_t(&t)[N], uint32_t(&prevT)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(uint32_t(&t)[N], uint32_t(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_Next(t, prevT, nIndex, pMapData, pDst, unDstlen);
	}

	///ѹ������float����.
	template <size_t N>
	int CompressObject(float(&t)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(float(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_First(t, nIndex, pMapData, pDst, unDstlen);
	}
	template <size_t N>
	int CompressObject(float(&t)[N], float(&prevT)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(float(&t)[N], float(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_Next(t, prevT, nIndex, pMapData, pDst, unDstlen);
	}

	///ѹ������int64_t����.
	template <size_t N>
	int CompressObject(int64_t(&t)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(int64_t(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_First(t, nIndex, pMapData, pDst, unDstlen);
	}
	template <size_t N>
	int CompressObject(int64_t(&t)[N], int64_t(&prevT)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(int64_t(&t)[N], int64_t(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_Next(t, prevT, nIndex, pMapData, pDst, unDstlen);
	}

	///ѹ������uint64_t����.
	template <size_t N>
	int CompressObject(uint64_t(&t)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(uint64_t(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_First(t, nIndex, pMapData, pDst, unDstlen);
	}
	template <size_t N>
	int CompressObject(uint64_t(&t)[N], uint64_t(&prevT)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(uint64_t(&t)[N], uint64_t(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_Next(t, prevT, nIndex, pMapData, pDst, unDstlen);
	}

	///ѹ������double����.
	template <size_t N>
	int CompressObject(double(&t)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(double(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_First(t, nIndex, pMapData, pDst, unDstlen);
	}
	template <size_t N>
	int CompressObject(double(&t)[N], double(&prevT)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(double(&t)[N], double(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_Next(t, prevT, nIndex, pMapData, pDst, unDstlen);
	}

	///ѹ������bool����.
	template <size_t N>
	int CompressObject(bool(&t)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(bool(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_First(t, nIndex, pMapData, pDst, unDstlen);
	}
	template <size_t N>
	int CompressObject(bool(&t)[N], bool(&prevT)[N], \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(bool(&t)[N], bool(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		return m_FieldCompress.Compress_Next(t, prevT, nIndex, pMapData, pDst, unDstlen);
	}


private:

	///�ֶ�ѹ�������.
	CFieldCompress m_FieldCompress;
	///��ȡѹ������С�����.
	CMaxCompSize m_maxCompSize;

	///���ݻ���.
	char *m_pData;
	///�����С.
	uint32_t m_unDataSize;

	///ԭʼ���ݴ�С.
	uint32_t m_unSrcDataSize;
	///��󻺴��С.
	uint32_t m_unMaxCompDataSize;
	///ѹ�����ݴ�С.
	uint32_t m_unCompDataSize;
};
} // namespace minicc