#pragma once

/**
* @Brief		��ѹ����ʹ�ýӿڶ���(����ģʽ�ṹ����ͨѶӦ�ã���֧��Ƕ�ף���֧��ָ��)���Ƕ��̰߳�ȫ��.
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

		//�������ǽṹ��ʵ�����ݽ�ѹ��������ṹ��Ƕ��(�ڲ������ڴ�).
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

			///�ж������Ƿ��ѹ��ȫ������ȫ����Ϊ�����쳣.
			if (unSrclen != 0) {
				nRetCode = MINICC_InvalidParams;
			}

			return nRetCode;
		}

	private:

		///�ṹ������ѹʵ����ں���.
		template<typename T>
		int DecompressImpl(T&& t, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressObject(std::forward<T>(t), pMapData, pSrc, unSrclen);
		}

		///�ṹ��������ʵ����ں���(��ǰ���ݶ�����жԱȽ�ѹ).
		template<typename T>
		int DecompressImpl(T && t, T&& prevT, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
			return DecompressObject(std::forward<T>(t), std::forward<T>(prevT), pMapData, pSrc, unSrclen);
		}

		//��ѹ�Զ���ṹ������(���ṹ���ѹ).
		template<typename T>
		typename std::enable_if<is_user_class<T>::value, int>::type DecompressObject(T&& t,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor begin to Decompress Tuple object, t.Meta()._Mysize = " << t.Meta()._Mysize << std::endl;
#endif
			///��ѹ�ṹͼ�ֶζԱȽ�ѹmap����ǰ�汾Ҫ���ѹ��ѹ�Ľṹ��ȫƥ�䣩.
			char *pTmpMapData = (char *)pSrc;
			int nMapSize = GetMapSize(pTmpMapData, unSrclen);
			QuickCompDecodeAdd(pSrc, unSrclen, nMapSize);

			return DecompressTuple(t.Meta(), pTmpMapData, pSrc, unSrclen);
		}

		//��ѹ�Զ���ṹ������(���ṹ���ѹ).
		template<typename T>
		typename std::enable_if<is_user_class<T>::value, int>::type DecompressObject(T&& t, T&& prevT,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor begin to Decompress Tuple object, t.Meta()._Mysize = " << t.Meta()._Mysize << std::endl;
#endif
			///��ѹ�ṹͼ�ֶζԱȽ�ѹmap����ǰ�汾Ҫ���ѹ��ѹ�Ľṹ��ȫƥ�䣩.
			char *pTmpMapData = (char *)pSrc;
			int nMapSize = GetMapSize(pTmpMapData, unSrclen);
			QuickCompDecodeAdd(pSrc, unSrclen, nMapSize);

			return DecompressTuple(t.Meta(), prevT.Meta(), pTmpMapData, pSrc, unSrclen);
		}

		//��ѹtuple.
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


		/*************��ѹ����*************/

		//��ѹ��map����.
		template<typename T>
		typename std::enable_if<is_singlevalue_container<T>::value, int>::type DecompressObject(T&& t,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor is_singlevalue_container DecompressObject(T&& t ...) t.size = " << t.size() << std::endl;
#endif
			///��ȡ���������������.
			uint32_t unSize = 0;
			int nBytes = Decompress_UINT32(pSrc, unSrclen, unSize);

			///�����󳤶�����.
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
			///��ȡ���������������.
			uint32_t unSize = 0;
			int nBytes = Decompress_UINT32(pSrc, unSrclen, unSize);

			///�����󳤶�����.
			using U = typename std::decay<T>::type;
			if (OverLimitDataLength(sizeof(U::value_type)*unSize)) {
				return MINICC_OverLimitDataLength;
			}

			QuickCompDecodeAdd(pSrc, unSrclen, nBytes); 
			t.resize(unSize);

			return DecompressArray(t, (int)t.size(), pMapData, pSrc, unSrclen);
		}


		///******�����������ѹ����******

		///�����������޷��ڲ�Ӱ�����ṹ������±������ݣ���ǰ�ݲ�֧����������ʽ.
		template<typename Adapter, typename F>
		inline int DecompressAdapter(Adapter& v, F f,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor CompressAdapter v.size() = " << v.size() << std::endl;
#endif
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor CompressAdapter �����������޷��ڲ�Ӱ�����ṹ������±������ݣ���ǰ�ݲ�֧����������ʽ " << std::endl;
			return MINICC_IllegalDataType;
#endif
		}

		//��ѹ[stack/priority_queue]������.
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

		//���ж���.
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


		/*************��ѹ����*************/

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

		//��������.
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

		//��ѹ�������������.
		template<typename Array>
		typename std::enable_if<!is_string<Array>::value, int>::type DecompressArray(Array& v, int nVSize,\
			char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor enable_if<!is_string<Array> DecompressFixedArray(Array& v, ...) nVSize = " << nVSize << std::endl;
#endif
			int nRetCode = MINICC_OK;
			int nBytes = 0;

			///��������ԱȽ�ѹӳ���.
			char *pArrMapData = (char *)pSrc;
			int nMapSize = GetMapSize(pArrMapData, unSrclen);
			QuickCompDecodeAdd(pSrc, unSrclen, nMapSize);

			///�����������0�Ž��н�ѹ.
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

		///******��ѹ�ṹ���ڶ���******.

		//������ѹ.
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

		//��ͨ��ָ������ѹ.
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

		//����ָ������ѹ.
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

		//�������ͽ�ѹ.
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



		//string���ͽ�ѹ.
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

		//��ѹö������.
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

		//******��ѹ��������******.

		///�ǻ������Ͷ������ݣ��ṹ����ȣ�.
		template <typename T, size_t N>
		int DecompressObject(T(&t)[N],\
			std::size_t nIndex, char *pMapData, char *&pSrc, uint32_t &unSrclen)
		{
#ifdef PRINT_DEBUG_INFO
			std::cout << "= > CComplexDecompressor DecompressObject(T(&t)[N], ...) N = " << N << " nIndex = " << nIndex << std::endl;
#endif
			///��ȡ���������������.
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
			///��ȡ���������������.
			uint32_t unSize = 0;
			int nBytes = Decompress_UINT32(pSrc, unSrclen, unSize);
			QuickCompDecodeAdd(pSrc, unSrclen, nBytes);
			if (unSize != N) return MINICC_InvalidParams;

			return DecompressArray(t, N, pMapData, pSrc, unSrclen);
		}

		///��ѹ����char����.
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

		///��ѹ����unsigned char����.
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

		///��ѹ����int16_t����.
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

		///��ѹ����uint16_t����.
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

		///��ѹ����int����.
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

		///��ѹ����uint32_t����.
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

		///��ѹ����float����.
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

		///��ѹ����int64_t����.
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

		///��ѹ����uint64_t����.
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

		///��ѹ����double����.
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

		///��ѹ����bool����.
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
		///�ֶν�ѹ�����.
		CFieldDecompress m_FieldDecompress;
};
} // namespace minicc