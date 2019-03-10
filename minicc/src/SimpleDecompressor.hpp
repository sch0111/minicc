#pragma once

/**
* @Brief		��ѹ����ʹ�ýӿڶ��壨��ģʽ�ṹ���󣬲�֧��Ƕ�ף�ֻ�Դ������ֵ�������ڴ棩���Ƕ��̰߳�ȫ��.
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
	///���ݽ�ѹ��.
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

		//�ṹ��ָ����������ѹ����������ṹ��Ƕ��(�ڲ������ڴ�).
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

		//�ṹ��ָ�����ݶԱ�ѹ����������ṹ��Ƕ��(�ڲ������ڴ�).
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

		//�������ǽṹ��ʵ������ѹ����������ṹ��Ƕ��(�ڲ������ڴ�).
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

		///ѹ��ʵ����ں���.
		template<typename T>
		int DecompressImpl(T&& t, char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressObject(std::forward<T>(t), pSrc, unSrclen);
		}

		///�ṹ�����ѹ��ʵ����ں���(��ǰ���ݶ�����жԱ�ѹ��).
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

		//��ѹ�Զ���ṹ������(���ṹ��ѹ��).
		template<typename T>
		typename std::enable_if<is_user_class<T>::value, int>::type DecompressObject(T&& t, char *&pSrc, uint32_t &unSrclen)
		{
			//std::cout << "begin to Decompress Tuple object !" << t.Meta()._Mysize << std::endl;
			///ѹ���ṹͼ�ֶζԱ�ѹ��map����ǰ�汾Ҫ��ѹ����ѹ�Ľṹ��ȫƥ�䣩.
			char *pMapData = (char *)pSrc;
			int nMapSize = GetMapSize(pMapData, unSrclen);
			QuickCompDecodeAdd(pSrc, unSrclen, nMapSize);

			return DecompressTuple(t.Meta(), pMapData, pSrc, unSrclen);
		}

		//��ѹ�Զ���ṹ������(���ṹ��ѹ��).
		template<typename T>
		typename std::enable_if<is_user_class<T>::value, int>::type DecompressObject(T&& t, T&& prevT,\
			char *&pSrc, uint32_t &unSrclen)
		{
			//std::cout << "begin to Decompress Tuple object !" << t.Meta()._Mysize << std::endl;
			///ѹ���ṹͼ�ֶζԱ�ѹ��map����ǰ�汾Ҫ��ѹ����ѹ�Ľṹ��ȫƥ�䣩.
			char *pMapData = (char *)pSrc;
			int nMapSize = GetMapSize(pMapData, unSrclen);
			QuickCompDecodeAdd(pSrc, unSrclen, nMapSize);

			return DecompressTuple(t.Meta(), prevT.Meta(), pMapData, pSrc, unSrclen);
		}

		//��ѹtuple.
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


		/*************��ѹ����*************/

		//��ѹ��map����.
		template<typename T>
		typename std::enable_if<is_singlevalue_container<T>::value, int>::type DecompressObject(T&& t,\
			char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressArray(t, pSrc, unSrclen);
		}

		//��ѹ[stack/priority_queue]������.
		template<typename T>
		typename std::enable_if<is_stack<T>::value || is_priority_queue<T>::value, int>::type DecompressObject(T&& t,\
			char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressAdapter(t, [&]{ return t.top(); }, pSrc, unSrclen);
		}

		///��ǰ�������漰�ƻ�����ṹ��Ϣ���ݲ�֧�ֲ���.
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


		/*************��ѹ����*************/

		//��ѹ��������.
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

		//������ѹ.
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

		//��ͨ��ָ������ѹ.
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

		//ָ������ѹ.
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

		//�ṹ�����Ա�ѹ��.
		template<typename T>
		typename std::enable_if<is_normal_class<T>::value, int>::type DecompressObject(T *pT, T *pPrevT,\
			std::size_t nIndex, char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressImpl(pT, pPrevT, pSrc, unSrclen);
		}


	private:
		///�ֶ�ѹ�������.
		CFieldDecompress m_FieldDecompress;
	};
} // namespace minicc