#pragma once

/**
* @Brief		解压方法使用接口定义（简单模式结构对象，不支持嵌套，只对传入对象赋值不开辟内存）（非多线程安全）.
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
	class CSimpleDecompressor : NonCopyable
	{
	public:
		CSimpleDecompressor()
		{
		}
		~CSimpleDecompressor()
		{
		}

	public:

		//结构体指针类型数据压缩，不允许结构体嵌套(内部开辟内存).
		template<typename T>
		typename std::enable_if<is_normal_class<T>::value, int>::type Decompress(T *pT, uint32_t unItems,\
			const char *pSrc, uint32_t unSrclen, uint32_t &unDecomplen)
		{
			int nRetCode = MINICC_InvalidParams;
			T *pPrevT = nullptr;
			uint32_t unOrigDstlen = unSrclen;

			for (uint32_t i = 0; i < unItems; i++) {
				nRetCode = DecompressImpl(&pT[i], pPrevT, (char *&)pSrc, unSrclen);
				CHECK_RETCODE(nRetCode);
				pPrevT = &pT[i];
			}
			unDecomplen = unOrigDstlen - unSrclen;

			return nRetCode;
		}

		//结构体指针数据对比压缩，不允许结构体嵌套(内部开辟内存).
		template<typename T>
		typename std::enable_if<is_normal_class<T>::value, int>::type Decompress(T *pT, T *pPrevT,\
			const char *pSrc, uint32_t unSrclen, uint32_t &unDecomplen)
		{
			int nRetCode = MINICC_InvalidParams;
			uint32_t unOrigDstlen = unSrclen;
			nRetCode = DecompressImpl(pT, pPrevT, (char *&)pSrc, unSrclen);
			unDecomplen = unOrigDstlen - unSrclen;

			return nRetCode;
		}

		//容器或是结构体实体数据压缩，不允许结构体嵌套(内部开辟内存).
		template<typename T>
		int Decompress(T&& t, const char *pSrc, uint32_t unSrclen/*, uint32_t &unDecomplen*/)
		{
			int nRetCode = MINICC_InvalidParams;
			uint32_t unOrigDstlen = unSrclen;
			nRetCode = DecompressImpl(std::forward<T>(t), (char *&)pSrc, unSrclen);
			//unDecomplen = unOrigDstlen - unSrclen;

			return nRetCode;
		}

	private:

		///压缩实现入口函数.
		template<typename T>
		int DecompressImpl(T&& t, char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressObject(std::forward<T>(t), pSrc, unSrclen);
		}

		///结构体对象压缩实现入口函数(带前数据对象进行对比压缩).
		template<typename T>
		int DecompressImpl(T *pT, T *pPrevT, char *&pSrc, uint32_t &unSrclen)
		{
			if (!pPrevT) {
				return DecompressObject(*pT, pSrc, unSrclen);
			}
			else {
				return DecompressObject(*pT, *pPrevT, pSrc, unSrclen);
			}
		}

		//解压自定义结构体类型(单结构体压缩).
		template<typename T>
		typename std::enable_if<is_user_class<T>::value, int>::type DecompressObject(T&& t, char *&pSrc, uint32_t &unSrclen)
		{
			//std::cout << "begin to Decompress Tuple object !" << t.Meta()._Mysize << std::endl;
			///压缩结构图字段对比压缩map（当前版本要求压缩解压的结构完全匹配）.
			char *pMapData = (char *)pSrc;
			int nMapSize = GetMapSize(pMapData, unSrclen);
			QuickCompDecodeAdd(pSrc, unSrclen, nMapSize);

			return DecompressTuple(t.Meta(), pMapData, pSrc, unSrclen);
		}

		//解压自定义结构体类型(单结构体压缩).
		template<typename T>
		typename std::enable_if<is_user_class<T>::value, int>::type DecompressObject(T&& t, T&& prevT,\
			char *&pSrc, uint32_t &unSrclen)
		{
			//std::cout << "begin to Decompress Tuple object !" << t.Meta()._Mysize << std::endl;
			///压缩结构图字段对比压缩map（当前版本要求压缩解压的结构完全匹配）.
			char *pMapData = (char *)pSrc;
			int nMapSize = GetMapSize(pMapData, unSrclen);
			QuickCompDecodeAdd(pSrc, unSrclen, nMapSize);

			return DecompressTuple(t.Meta(), prevT.Meta(), pMapData, pSrc, unSrclen);
		}

		//解压tuple.
		template<typename T>
		typename std::enable_if<is_tuple<T>::value, int>::type DecompressObject(T&& t,\
			char *&pMapData, char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressTuple(t, pMapData, pSrc, unSrclen);
		}

		template<std::size_t I = 0, typename Tuple>
		typename std::enable_if<I == std::tuple_size<Tuple>::value, int>::type DecompressTuple(const Tuple& t,\
			char *&pMapData, char *&pSrc, uint32_t &unSrclen)
		{
			//std::cout << "tuple_size<Tuple> == 0\r\n" << std::endl;
			return MINICC_OK;
		}

		template<std::size_t I = 0, typename Tuple>
		typename std::enable_if<I == std::tuple_size<Tuple>::value, int>::type DecompressTuple(const Tuple& t, const Tuple& prevT,\
			char *&pMapData, char *&pSrc, uint32_t &unSrclen)
		{
			//std::cout << "tuple_size<Tuple> == 0\r\n" << std::endl;
			return MINICC_OK;
		}

		template<std::size_t I = 0, typename Tuple>
		typename std::enable_if < I < std::tuple_size<Tuple>::value, int>::type DecompressTuple(const Tuple& t,\
			char *&pMapData, char *&pSrc, uint32_t &unSrclen)
		{
			int nRetCode = MINICC_OK;
			nRetCode = m_FieldDecompress.Decompress_First(std::get<I>(t), I, pMapData, pSrc, unSrclen);
			CHECK_RETCODE(nRetCode);
			nRetCode = DecompressTuple<I + 1>(t, pMapData, pSrc, unSrclen);
			CHECK_RETCODE(nRetCode);

			return nRetCode;
		}

		template<std::size_t I = 0, typename Tuple>
		typename std::enable_if < I < std::tuple_size<Tuple>::value, int>::type DecompressTuple(const Tuple& t, const Tuple& prevT,\
			char *&pMapData, char *&pSrc, uint32_t &unSrclen)
		{
			int nRetCode = m_FieldDecompress.Decompress_Next(std::get<I>(t), std::get<I>(prevT), I, pMapData, pSrc, unSrclen);
			CHECK_RETCODE(nRetCode);
			nRetCode = DecompressTuple<I + 1>(t, prevT, pMapData, pSrc, unSrclen);
			CHECK_RETCODE(nRetCode);

			return nRetCode;
		}


		/*************解压容器*************/

		//解压非map容器.
		template<typename T>
		typename std::enable_if<is_singlevalue_container<T>::value, int>::type DecompressObject(T&& t,\
			char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressArray(t, pSrc, unSrclen);
		}

		//解压[stack/priority_queue]适配器.
		template<typename T>
		typename std::enable_if<is_stack<T>::value || is_priority_queue<T>::value, int>::type DecompressObject(T&& t,\
			char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressAdapter(t, [&]{ return t.top(); }, pSrc, unSrclen);
		}

		///当前迭代器涉及破坏对象结构信息，暂不支持操作.
		template<typename Adapter, typename F>
		inline int DecompressAdapter(Adapter& v, F f,\
			char *&pSrc, uint32_t &unSrclen)
		{
			//std::cout << "v.size() = " << v.size() << std::endl;
			int nRetCode = MINICC_InvalidParams;

			return nRetCode;
		}

		template<typename T>
		typename std::enable_if<is_queue<T>::value, int>::type DecompressObject(T&& t,\
			char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressAdapter(t, [&]{ return t.front(); }, pSrc, unSrclen);
		}


		/*************解压数组*************/

		//解压定长数组.
		template<typename Array>
		typename std::enable_if<!is_string<Array>::value, int>::type DecompressArray(Array& v,\
			char *&pSrc, uint32_t &unSrclen)
		{
			int nRetCode = MINICC_InvalidParams;
			for (int i=0; i < (int)v.size(); i++)
			{
				if (i > 0) {
					nRetCode = DecompressObject(v[i], v[i-1], i, pSrc, unSrclen);
					CHECK_RETCODE(nRetCode);
				}
				else {
					nRetCode = DecompressObject(v[i], i, pSrc, unSrclen);
					CHECK_RETCODE(nRetCode);
				}
			}

			return nRetCode;
		}

		//类对象解压.
		template<typename T>
		typename std::enable_if<is_normal_class<T>::value, int>::type DecompressObject(T && t, T &&prevT,\
			std::size_t nIndex, char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressImpl(&t, &prevT, pSrc, unSrclen);
		}

		template<typename T>
		typename std::enable_if<is_normal_class<T>::value && !is_pointer_ext<T>::value, int>::type DecompressObject(T && t,\
			std::size_t nIndex, char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressImpl(t, pSrc, unSrclen);
		}

		//普通类指针对象解压.
		template<typename T>
		typename std::enable_if<std::is_pointer<T>::value, int>::type DecompressObject(T && t, T &&prevT,\
			std::size_t nIndex, char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressImpl(*t, prevT, pSrc, unSrclen);
		}

		template<typename T>
		typename std::enable_if<std::is_pointer<T>::value, int>::type DecompressObject(T && t,\
			std::size_t nIndex, char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressImpl(*t, pSrc, unSrclen);
		}

		//指针对象解压.
		template<typename T>
		typename std::enable_if<is_smart_pointer<T>::value, int>::type DecompressObject(T && t, T &&prevT,\
			std::size_t nIndex, char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressImpl(t.get(), prevT.get(), pSrc, unSrclen);
		}

		template<typename T>
		typename std::enable_if<is_smart_pointer<T>::value, int>::type DecompressObject(T && t,\
			std::size_t nIndex, char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressImpl(*t.get(), pSrc, unSrclen);
		}

		//结构体对象对比压缩.
		template<typename T>
		typename std::enable_if<is_normal_class<T>::value, int>::type DecompressObject(T *pT, T *pPrevT,\
			std::size_t nIndex, char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressImpl(pT, pPrevT, pSrc, unSrclen);
		}


	private:
		///字段压缩类对象.
		CFieldDecompress m_FieldDecompress;
	};
} // namespace minicc