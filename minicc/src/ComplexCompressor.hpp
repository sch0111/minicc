#pragma once

/**
* @Brief		压缩方法使用接口定义(复杂模式结构对象（通讯应用），支持嵌套，支持嵌套，用于网络数据压缩压缩传输)（非多线程安全）.
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
///数据压缩器.
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

	//容器或是结构体实体数据压缩，允许结构体嵌套，用于通讯数据包压缩压缩.
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
		std::cout << "= > CComplexCompressor Compress(T&& t) 压缩比：" << m_unCompDataSize / (m_unSrcDataSize * 1.0) << " | " << (m_unSrcDataSize * 1.0) / m_unCompDataSize << std::endl;
#endif
#endif
		return nRetCode;
	}

	///获取压缩缓存指针.
	const char* GetString()
	{
		return m_pData;
	}

	///获取源数据大小.
	int GetSrcDataSize()
	{
		return m_unSrcDataSize;
	}
	///获取最大压缩数据大小.
	int GetMaxCompDataSize()
	{
		return m_unMaxCompDataSize;
	}
	///获取压缩数据大小.
	int GetCompDataSize()
	{
		return m_unCompDataSize;
	}

	///对象初始化.
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

	///压缩对象参数初始化.
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

	//结构体对象压缩实现入口函数.
	template<typename T>
	int CompressImpl(T && t, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
		return CompressObject(std::forward<T>(t), pMapData, pDst, unDstlen);
	}

	///结构体对象压缩实现入口函数(带前数据对象进行对比压缩).
	template<typename T>
	int CompressImpl(T && t, T&& prevT, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
		return CompressObject(std::forward<T>(t), std::forward<T>(prevT), pMapData, pDst, unDstlen);
	}

	//压缩自定义结构体类型(单结构体压缩).
	template<typename T>
	typename std::enable_if<is_user_class<T>::value, int>::type CompressObject(T&& t,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor begin to compress Tuple object, t.Meta()._Mysize = " << t.Meta()._Mysize << std::endl;
#endif
		///压缩结构图字段对比压缩map（当前版本要求压缩解压的结构完全匹配）.
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
		///压缩结构图字段对比压缩map（当前版本要求压缩解压的结构完全匹配）.
		char *pTmpMapData = (char *)pDst;
		int nBytes = SetMapSizeByAmount(pTmpMapData, unDstlen, (int)t.Meta()._Mysize);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressTuple(t.Meta(), prevT.Meta(), pTmpMapData, pDst, unDstlen);
	}

	//压缩tuple.
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


	/*************压缩容器*************/

	//压缩非map容器.
	template<typename T>
	typename std::enable_if<is_singlevalue_container<T>::value, int>::type CompressObject(T&& t,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor is_singlevalue_container CompressObject(T&& t ...) t.size = " << t.size() << std::endl;
#endif
		///保存容器对象数组个数.
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
		///保存容器对象数组个数.
		int nBytes = Compress_UINT32(pDst, unDstlen, (uint32_t)t.size());
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressArray(t, (uint32_t)t.size(), pMapData, pDst, unDstlen);
	}


	///******适配器对象压缩操作******

	template<typename Adapter, typename F>
	inline int CompressAdapter(Adapter& v, F f, \
		char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressAdapter v.size() = " << v.size() << std::endl;
#endif
		int nVSize = v.size();
		///保存容器对象个数.
		int nRetCode = Compress_UINT32(pDst, unDstlen, nVSize);
		QuickCompEncodeAdd(pDst, unDstlen, nRetCode);

		///容器对象对比压缩映射表.
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

	//压缩[stack/priority_queue]适配器.
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

	//队列对象.
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

	/*************压缩数组*************/

	//std::array
	template<typename T, size_t N>
	int CompressObject(std::array<T, N>& t, \
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(std::array<T, N>& t, ...) N = " << N  << std::endl;
#endif
		///保存容器对象数组个数.
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
		///保存容器对象数组个数.
		int nBytes = Compress_UINT32(pDst, unDstlen, N);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressArray(t, N, pMapData, pDst, unDstlen);
		//return CompressArray(t, N, prevT, N, pMapData, pDst, unDstlen);
	}

	//定长数组.
	template <typename T, size_t N>
	int CompressObject(T(&t)[N],\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(T(&t)[N], ...) N = " << N << std::endl;
#endif
		///保存容器对象数组个数.
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
		///保存容器对象数组个数.
		int nBytes = Compress_UINT32(pDst, unDstlen, N);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressArray(t, N, pMapData, pDst, unDstlen);
		//return CompressArray(t, N, prevT, N, pMapData, pDst, unDstlen);
	}

	//压缩定长非字符串数组（当前为类对象）.
	template<typename Array>
	typename std::enable_if<!is_string<Array>::value, int>::type CompressArray(Array& v, int nVSize,\
		char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor enable_if<!is_string<Array> CompressArray(Array& v, ...) nVSize = " << nVSize << std::endl;
#endif
		int nRetCode = MINICC_OK;
		int nBytes = 0;

		/////保存容器对象数组个数.
		//nBytes = Compress_UINT32(pDst, unDstlen, nVSize);
		//QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		///容器对象对比压缩映射表.
		char *pArrMapData = (char *)pDst;
		nBytes = SetMapSizeByAmount(pArrMapData, unDstlen, nVSize);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		///数组个数大于0才进行压缩.
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

	///******压缩结构体内对象******.

	///压缩类对象.
	template<typename T>
	typename std::enable_if<is_normal_class<T>::value && !is_pointer_ext<T>::value, int>::type CompressObject(T && t, \
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor enable_if<is_normal_class<T>::value && !is_pointer_ext<T> CompressObject(T && t, ...) nIndex = " << nIndex << std::endl;
#endif
		///类对象容器字段压缩表示默认置1.
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
		///类对象容器字段压缩表示默认置1.
		SetQuickCompEncode(pMapData, nIndex);
		return CompressImpl(std::forward<T>(t), std::forward<T>(prevT), pMapData, pDst, unDstlen);
	}


	//普通类指针对象压缩.
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

	//智能指针对象压缩.
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

	//压缩基础类型.
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

	//压缩string类型.
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

	//压缩枚举类型.
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

	//******压缩定长数组******.

	///非基本类型定长数据（结构对象等）.
	template <typename T, size_t N>
	int CompressObject(T(&t)[N],\
		std::size_t nIndex, char *pMapData, char *&pDst, uint32_t &unDstlen)
	{	
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CComplexCompressor CompressObject(T(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
		///类对象容器字段压缩表示默认置1.
		SetQuickCompEncode(pMapData, nIndex);

		///保存容器对象数组个数.
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
		///类对象容器字段压缩表示默认置1.
		SetQuickCompEncode(pMapData, nIndex); 

		///保存容器对象数组个数.
		int nBytes = Compress_UINT32(pDst, unDstlen, N);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		return CompressArray(t, N, pMapData, pDst, unDstlen);
	}

	///压缩定长char数组.
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

	///压缩定长unsigned char数组.
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

	///压缩定长int16_t数组.
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

	///压缩定长uint16_t数组.
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

	///压缩定长int数组.
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

	///压缩定长uint32_t数组.
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

	///压缩定长float数组.
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

	///压缩定长int64_t数组.
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

	///压缩定长uint64_t数组.
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

	///压缩定长double数组.
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

	///压缩定长bool数组.
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

	///字段压缩类对象.
	CFieldCompress m_FieldCompress;
	///获取压缩最大大小类对象.
	CMaxCompSize m_maxCompSize;

	///数据缓存.
	char *m_pData;
	///缓存大小.
	uint32_t m_unDataSize;

	///原始数据大小.
	uint32_t m_unSrcDataSize;
	///最大缓存大小.
	uint32_t m_unMaxCompDataSize;
	///压缩数据大小.
	uint32_t m_unCompDataSize;
};
} // namespace minicc