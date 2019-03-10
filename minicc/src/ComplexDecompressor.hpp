#pragma once

/**
* @Brief		解压方法使用接口定义(复杂模式结构对象（通讯应用），支持嵌套，不支持指针)（非多线程安全）.
*
*/

#include <vector>
#include <tchar.h>
#include "traits.hpp"
#include "Common.hpp"
#include "AlgorithmComDefine.hpp"
#include "FieldDecompress.hpp"

using namespace std;


namespace minicc
{
	///数据解压器.
	class CComplexDecompressor : NonCopyable
	{
	public:
		CComplexDecompressor()
		{
		}
		~CComplexDecompressor()
		{
		}

	public:

		//容器或是结构体实体数据解压，不允许结构体嵌套(内部开辟内存).
		template<typename T>
		int Decompress(T&& t, char *pSrc, uint32_t unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor Decompress(T&& t) unSrclen = " << unSrclen << std::endl;
			PrintBufferByHex(pSrc, unSrclen);
			cout << endl;
#endif

			int nRetCode = DecompressImpl(std::forward<T>(t), nullptr, (char *)pSrc, unSrclen);
			CHECK_RETCODE(nRetCode);

			///判断数据是否解压完全，不完全则认为数据异常.
			if (unSrclen != 0) {
				nRetCode = MINICC_InvalidParams;
			}

			return nRetCode;
		}

	private:

		///结构体对象解压实现入口函数.
		template<typename T>
		int DecompressImpl(T&& t, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressObject(std::forward<T>(t), pMapData, pSrc, unSrclen);
		}

		///结构体对象解缩实现入口函数(带前数据对象进行对比解压).
		template<typename T>
		int DecompressImpl(T && t, T&& prevT, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressObject(std::forward<T>(t), std::forward<T>(prevT), pMapData, pSrc, unSrclen);
		}

		//解压自定义结构体类型(单结构体解压).
		template<typename T>
		typename std::enable_if<is_user_class<T>::value, int>::type DecompressObject(T&& t,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor begin to Decompress Tuple object, t.Meta()._Mysize = " << t.Meta()._Mysize << std::endl;
#endif
			///解压结构图字段对比解压map（当前版本要求解压解压的结构完全匹配）.
			char *pTmpMapData = (char *)pSrc;
			int nMapSize = GetMapSize(pTmpMapData, unSrclen);
			QuickCompDecodeAdd(pSrc, unSrclen, nMapSize);

			return DecompressTuple(t.Meta(), pTmpMapData, pSrc, unSrclen);
		}

		//解压自定义结构体类型(单结构体解压).
		template<typename T>
		typename std::enable_if<is_user_class<T>::value, int>::type DecompressObject(T&& t, T&& prevT,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor begin to Decompress Tuple object, t.Meta()._Mysize = " << t.Meta()._Mysize << std::endl;
#endif
			///解压结构图字段对比解压map（当前版本要求解压解压的结构完全匹配）.
			char *pTmpMapData = (char *)pSrc;
			int nMapSize = GetMapSize(pTmpMapData, unSrclen);
			QuickCompDecodeAdd(pSrc, unSrclen, nMapSize);

			return DecompressTuple(t.Meta(), prevT.Meta(), pTmpMapData, pSrc, unSrclen);
		}

		//解压tuple.
		template<typename T>
		typename std::enable_if<is_tuple<T>::value, int>::type DecompressObject(T&& t,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressTuple(t, pMapData, pSrc, unSrclen);
		}

		template<std::size_t I = 0, typename Tuple>
		typename std::enable_if<I == std::tuple_size<Tuple>::value, int>::type DecompressTuple(const Tuple& t,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressTuple(const Tuple& t, ...) tuple_size<Tuple> == 0" << std::endl;
#endif
			return MINICC_OK;
		}

		template<std::size_t I = 0, typename Tuple>
		typename std::enable_if<I == std::tuple_size<Tuple>::value, int>::type DecompressTuple(const Tuple& t, const Tuple& prevT,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressTuple(const Tuple& t, const Tuple& prevT, ...) tuple_size<Tuple> == 0" << std::endl;
#endif
			return MINICC_OK;
		}

		template<std::size_t I = 0, typename Tuple>
		typename std::enable_if < I < std::tuple_size<Tuple>::value, int>::type DecompressTuple(const Tuple& t,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressTuple(const Tuple& t, ...) I = " << I << std::endl;
#endif
			int nRetCode = MINICC_OK;
			nRetCode = DecompressObject(std::get<I>(t), I, pMapData, pSrc, unSrclen);
			CHECK_RETCODE(nRetCode);
			nRetCode = DecompressTuple<I + 1>(t, pMapData, pSrc, unSrclen);
			CHECK_RETCODE(nRetCode);

			return nRetCode;
		}

		template<std::size_t I = 0, typename Tuple>
		typename std::enable_if < I < std::tuple_size<Tuple>::value, int>::type DecompressTuple(const Tuple& t, const Tuple& prevT,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressTuple(const Tuple& t, const Tuple& prevT, ...) I = " << I << std::endl;
#endif
			int nRetCode = MINICC_OK;
			nRetCode = DecompressObject(std::get<I>(t), std::get<I>(prevT), I, pMapData, pSrc, unSrclen);
			CHECK_RETCODE(nRetCode);
			nRetCode = DecompressTuple<I + 1>(t, prevT, pMapData, pSrc, unSrclen);
			CHECK_RETCODE(nRetCode);

			return nRetCode;
		}


		/*************解压容器*************/

		//解压非map容器.
		template<typename T>
		typename std::enable_if<is_singlevalue_container<T>::value, int>::type DecompressObject(T&& t,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor is_singlevalue_container DecompressObject(T&& t ...) t.size = " << t.size() << std::endl;
#endif
			///获取容器对象数组个数.
			uint32_t unSize = 0;
			int nBytes = Decompress_UINT32(pSrc, unSrclen, unSize);

			///检查最大长度限制.
			using U = typename std::decay<T>::type;
			if (OverLimitDataLength(sizeof(U::value_type)*unSize)) {
				return MINICC_OverLimitDataLength;
			}

			QuickCompDecodeAdd(pSrc, unSrclen, nBytes);
			t.resize(unSize);

			return DecompressArray(t, (int)t.size(), pMapData, pSrc, unSrclen);
		}
		template<typename T>
		typename std::enable_if<is_singlevalue_container<T>::value, int>::type DecompressObject(T&& t, T&& prevT,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor is_singlevalue_container DecompressObject(T&& t, T&& prevT...) t.size = " << t.size() << " | " << prevT.size() << std::endl;
#endif
			///获取容器对象数组个数.
			uint32_t unSize = 0;
			int nBytes = Decompress_UINT32(pSrc, unSrclen, unSize);

			///检查最大长度限制.
			using U = typename std::decay<T>::type;
			if (OverLimitDataLength(sizeof(U::value_type)*unSize)) {
				return MINICC_OverLimitDataLength;
			}

			QuickCompDecodeAdd(pSrc, unSrclen, nBytes); 
			t.resize(unSize);

			return DecompressArray(t, (int)t.size(), pMapData, pSrc, unSrclen);
		}


		///******适配器对象解压操作******

		///由于适配器无法在不影响对象结构的情况下遍历内容，当前暂不支持适配器方式.
		template<typename Adapter, typename F>
		inline int DecompressAdapter(Adapter& v, F f,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor CompressAdapter v.size() = " << v.size() << std::endl;
#endif
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor CompressAdapter 由于适配器无法在不影响对象结构的情况下遍历内容，当前暂不支持适配器方式 " << std::endl;
			return MINICC_IllegalDataType;
#endif
		}

		//解压[stack/priority_queue]适配器.
		template<typename T>
		typename std::enable_if<is_stack<T>::value || is_priority_queue<T>::value, int>::type DecompressObject(T&& t,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor is_stack | is_priority_queue DecompressObject(T&& t ...) t.size = " << t.size() << std::endl;
#endif
			return DecompressAdapter(t, [&]{ return t.top(); }, pSrc, unSrclen);
		}
		template<typename T>
		typename std::enable_if<is_stack<T>::value || is_priority_queue<T>::value, int>::type DecompressObject(T&& t, T&& prevT,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor is_stack | is_priority_queue DecompressObject(T&& t, T&& prevT...) t.size = " << t.size() << " | " << prevT.size() << std::endl;
#endif
			return DecompressAdapter(t, [&]{ return t.top(); }, pSrc, unSrclen);
		}

		//队列对象.
		template<typename T>
		typename std::enable_if<is_queue<T>::value, int>::type DecompressObject(T&& t,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor is_queue DecompressObject(T&& t ...) t.size = " << t.size() << std::endl;
#endif
			return DecompressAdapter(t, [&]{ return t.front(); }, pSrc, unSrclen);
		}
		template<typename T>
		typename std::enable_if<is_queue<T>::value, int>::type DecompressObject(T&& t, T&& prevT,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor is_queue DecompressObject(T&& t, T&& prevT, ...) t.size = " << t.size() << " | " << prevT.size() << std::endl;
#endif
			return DecompressAdapter(t, [&]{ return t.front(); }, pSrc, unSrclen);
		}


		/*************解压数组*************/

		//std::array
		template<typename T, size_t N>
		int DecompressObject(std::array<T, N>& t, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(std::array<T, N>& t, ...) N = " << N << std::endl;
#endif
			return DecompressArray(t, N, pMapData, pSrc, unSrclen);
		}
		template<typename T, size_t N>
		int DecompressObject(std::array<T, N>& t, std::array<T, N>& prevT, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(std::array<T, N>& t, std::array<T, N>& prevT, ...) N = " << N << std::endl;
#endif
			return DecompressArray(t, N, pMapData, pSrc, unSrclen);
		}

		//定长数组.
		template <typename T, size_t N>
		int DecompressObject(T(&t)[N], char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(T(&t)[N], ...) N = " << N << std::endl;
#endif
			return DecompressArray(t, N, pMapData, pSrc, unSrclen);
		}
		template <typename T, size_t N>
		int DecompressObject(T(&t)[N], T(&prevT)[N], char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(T(&t)[N], T(&prevT)[N], ...) N = " << N << std::endl;
#endif
			return DecompressArray(t, N, pMapData, pSrc, unSrclen);
		}

		//解压定长类对象数组.
		template<typename Array>
		typename std::enable_if<!is_string<Array>::value, int>::type DecompressArray(Array& v, int nVSize,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor enable_if<!is_string<Array> DecompressFixedArray(Array& v, ...) nVSize = " << nVSize << std::endl;
#endif
			int nRetCode = MINICC_OK;
			int nBytes = 0;

			///容器对象对比解压映射表.
			char *pArrMapData = (char *)pSrc;
			int nMapSize = GetMapSize(pArrMapData, unSrclen);
			QuickCompDecodeAdd(pSrc, unSrclen, nMapSize);

			///数组个数大于0才进行解压.
			for (int i = 0; i < nVSize; i++)
			{
				if (i > 0) {
					nRetCode = DecompressObject(v[i], v[i - 1], i, pArrMapData, pSrc, unSrclen);
					CHECK_RETCODE(nRetCode);
				}
				else {
					nRetCode = DecompressObject(v[i], i, pArrMapData, pSrc, unSrclen);
					CHECK_RETCODE(nRetCode);
				}
			}

			return nRetCode;
		}

		///******解压结构体内对象******.

		//类对象解压.
		template<typename T>
		typename std::enable_if<is_normal_class<T>::value && !is_pointer_ext<T>::value, int>::type DecompressObject(T && t,\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor enable_if<is_normal_class<T>::value && !is_pointer_ext<T> DecompressObject(T && t, ...) nIndex = " << nIndex << std::endl;
#endif
			return DecompressImpl(std::forward<T>(t), pMapData, pSrc, unSrclen);
		}
		template<typename T>
		typename std::enable_if<is_normal_class<T>::value && !is_pointer_ext<T>::value, int>::type DecompressObject(T && t, T &&prevT,\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor enable_if<is_normal_class<T>::value && !is_pointer_ext<T> DecompressObject(T && t, T &&prevT, ...) nIndex = " << nIndex << std::endl;
#endif
			return DecompressImpl(std::forward<T>(t), std::forward<T>(prevT), pMapData, pSrc, unSrclen);
		}

		//普通类指针对象解压.
		template<typename T>
		typename std::enable_if<std::is_pointer<T>::value, int>::type DecompressObject(T && PreT, T &&LastT,\
			std::size_t nIndex, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor enable_if<std::is_pointer<T> DecompressObject(T && t, ...) nIndex = " << nIndex << std::endl;
#endif
			return MINICC_IllegalDataType;
		}

		template<typename T>
		typename std::enable_if<std::is_pointer<T>::value, int>::type DecompressObject(T && PreT,\
			std::size_t nIndex, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor enable_if<std::is_pointer<T> DecompressObject(T && t, T &&prevT, ...) nIndex = " << nIndex << std::endl;
#endif
			return MINICC_IllegalDataType;
		}

		//智能指针对象解压.
		template<typename T>
		typename std::enable_if<is_smart_pointer<T>::value, int>::type DecompressObject(T && PreT, T &&LastT,\
			std::size_t nIndex, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor enable_if<is_smart_pointer<T> DecompressObject(T && t, ...) nIndex = " << nIndex << std::endl;
#endif
			return MINICC_IllegalDataType;
		}

		template<typename T>
		typename std::enable_if<is_smart_pointer<T>::value, int>::type DecompressObject(T && PreT,\
			std::size_t nIndex, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor enable_if<is_smart_pointer<T> DecompressObject(T && t, T &&prevT, ...) nIndex = " << nIndex << std::endl;
#endif
			return MINICC_IllegalDataType;
		}

		//基础类型解压.
		template<typename T>
		typename std::enable_if<is_basic_type<T>::value, int>::type DecompressObject(T&& t,\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor enable_if<is_basic_type<T> DecompressObject(T&& t, ...) nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_First(t, nIndex, pMapData, pSrc, unSrclen);
		}
		template<typename T>
		typename std::enable_if<is_basic_type<T>::value, int>::type DecompressObject(T&& t, T&& prevT,\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor enable_if<is_basic_type<T> DecompressObject(T&& t, T&& prevT, ...) nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_Next(t, prevT, nIndex, pMapData, pSrc, unSrclen);
		}



		//string类型解压.
		template<typename T>
		typename std::enable_if<is_string<T>::value, int>::type DecompressObject(T&& t,\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor enable_if<is_string<T> DecompressObject(T&& t, ...) nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_First(t, nIndex, pMapData, pSrc, unSrclen);
		}
		template<typename T>
		typename std::enable_if<is_string<T>::value, int>::type DecompressObject(T&& t, T&& prevT,\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor enable_if<is_string<T> DecompressObject(T&& t, T&& prevT, ...) nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_Next(t, prevT, nIndex, pMapData, pSrc, unSrclen);
		}

		//解压枚举类型.
		template<typename T>
		typename std::enable_if<is_enum_type<T>::value, int>::type DecompressObject(T&& t,\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor enable_if<is_enum_type<T> DecompressObject(T&& t, ...) nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_First(t, nIndex, pMapData, pSrc, unSrclen);
		}
		template<typename T>
		typename std::enable_if<is_enum_type<T>::value, int>::type DecompressObject(T&& t, T&& prevT,\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor enable_if<is_enum_type<T> DecompressObject(T&& t, T&& prevT, ...) nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_Next(t, prevT, nIndex, pMapData, pSrc, unSrclen);
		}

		//******解压定长数组******.

		///非基本类型定长数据（结构对象等）.
		template <typename T, size_t N>
		int DecompressObject(T(&t)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(T(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			///获取容器对象数组个数.
			uint32_t unSize = 0;
			int nBytes = Decompress_UINT32(pSrc, unSrclen, unSize);
			QuickCompDecodeAdd(pSrc, unSrclen, nBytes);
			if (unSize != N) return MINICC_InvalidParams;

			return DecompressArray(t, N, pMapData, pSrc, unSrclen);
		}
		template <typename T, size_t N>
		int DecompressObject(T(&t)[N], T(&prevT)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(T(&t)[N], T(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			///获取容器对象数组个数.
			uint32_t unSize = 0;
			int nBytes = Decompress_UINT32(pSrc, unSrclen, unSize);
			QuickCompDecodeAdd(pSrc, unSrclen, nBytes);
			if (unSize != N) return MINICC_InvalidParams;

			return DecompressArray(t, N, pMapData, pSrc, unSrclen);
		}

		///解压定长char数组.
		template <size_t N>
		int DecompressObject(char(&t)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(char(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_First(t, nIndex, pMapData, pSrc, unSrclen);
		}
		template <size_t N>
		int DecompressObject(char(&t)[N], char(&prevT)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(char(&t)[N], char(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_Next(t, prevT, nIndex, pMapData, pSrc, unSrclen);
		}

		///解压定长unsigned char数组.
		template <size_t N>
		int DecompressObject(unsigned char(&t)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(unsigned char(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_First(t, nIndex, pMapData, pSrc, unSrclen);
		}
		template <size_t N>
		int DecompressObject(unsigned char(&t)[N], unsigned char(&prevT)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(unsigned char(&t)[N], unsigned char(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_Next(t, prevT, nIndex, pMapData, pSrc, unSrclen);
		}

		///解压定长int16_t数组.
		template <size_t N>
		int DecompressObject(int16_t(&t)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(int16_t(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_First(t, nIndex, pMapData, pSrc, unSrclen);
		}
		template <size_t N>
		int DecompressObject(int16_t(&t)[N], int16_t(&prevT)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(int16_t(&t)[N], int16_t(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_Next(t, prevT, nIndex, pMapData, pSrc, unSrclen);
		}

		///解压定长uint16_t数组.
		template <size_t N>
		int DecompressObject(uint16_t(&t)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(uint16_t(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_First(t, nIndex, pMapData, pSrc, unSrclen);
		}
		template <size_t N>
		int DecompressObject(uint16_t(&t)[N], uint16_t(&prevT)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(uint16_t(&t)[N], uint16_t(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_Next(t, prevT, nIndex, pMapData, pSrc, unSrclen);
		}

		///解压定长int数组.
		template <size_t N>
		int DecompressObject(int(&t)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(int(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_First(t, nIndex, pMapData, pSrc, unSrclen);
		}
		template <size_t N>
		int DecompressObject(int(&t)[N], int(&prevT)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(int(&t)[N], int(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_Next(t, prevT, nIndex, pMapData, pSrc, unSrclen);
		}

		///解压定长uint32_t数组.
		template <size_t N>
		int DecompressObject(uint32_t(&t)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(uint32_t(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_First(t, nIndex, pMapData, pSrc, unSrclen);
		}
		template <size_t N>
		int DecompressObject(uint32_t(&t)[N], uint32_t(&prevT)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(uint32_t(&t)[N], uint32_t(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_Next(t, prevT, nIndex, pMapData, pSrc, unSrclen);
		}

		///解压定长float数组.
		template <size_t N>
		int DecompressObject(float(&t)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(float(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_First(t, nIndex, pMapData, pSrc, unSrclen);
		}
		template <size_t N>
		int DecompressObject(float(&t)[N], float(&prevT)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(float(&t)[N], float(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_Next(t, prevT, nIndex, pMapData, pSrc, unSrclen);
		}

		///解压定长int64_t数组.
		template <size_t N>
		int DecompressObject(int64_t(&t)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(int64_t(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_First(t, nIndex, pMapData, pSrc, unSrclen);
		}
		template <size_t N>
		int DecompressObject(int64_t(&t)[N], int64_t(&prevT)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(int64_t(&t)[N], int64_t(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_Next(t, prevT, nIndex, pMapData, pSrc, unSrclen);
		}

		///解压定长uint64_t数组.
		template <size_t N>
		int DecompressObject(uint64_t(&t)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(uint64_t(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_First(t, nIndex, pMapData, pSrc, unSrclen);
		}
		template <size_t N>
		int DecompressObject(uint64_t(&t)[N], uint64_t(&prevT)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(uint64_t(&t)[N], uint64_t(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_Next(t, prevT, nIndex, pMapData, pSrc, unSrclen);
		}

		///解压定长double数组.
		template <size_t N>
		int DecompressObject(double(&t)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(double(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_First(t, nIndex, pMapData, pSrc, unSrclen);
		}
		template <size_t N>
		int DecompressObject(double(&t)[N], double(&prevT)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(double(&t)[N], double(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_Next(t, prevT, nIndex, pMapData, pSrc, unSrclen);
		}

		///解压定长bool数组.
		template <size_t N>
		int DecompressObject(bool(&t)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(bool(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_First(t, nIndex, pMapData, pSrc, unSrclen);
		}
		template <size_t N>
		int DecompressObject(bool(&t)[N], bool(&prevT)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(bool(&t)[N], bool(&prevT)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			return m_FieldDecompress.Decompress_Next(t, prevT, nIndex, pMapData, pSrc, unSrclen);
		}


private:
		///字段解压类对象.
		CFieldDecompress m_FieldDecompress;
};
} // namespace minicc