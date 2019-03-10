#pragma once

/**
* @Brief		字段压缩方法定义.
*
*/

#include <tchar.h>
#include <iostream>
#include "traits.hpp"
#include "Common.hpp"
#include "BasicTypeCompress.hpp"

using namespace std;

namespace minicc
{
///字段压缩.
class CFieldCompress : NonCopyable
{
public:
	CFieldCompress()
	{
	}
	~CFieldCompress()
	{
	}

public:

	template<typename T>
	int Compress_First(T val, std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
		char szErrInfo[MAX_ERROR_INFO];
		sprintf_s(szErrInfo, "= > CFieldCompress Compress_First field [%d] is unknow data type!", nIndex + 1);	
#ifdef PRINT_DEBUG_INFO
		std::cout << szErrInfo << std::endl;
#endif
		return MINICC_IllegalDataType;
	}

	template<typename T>
	int Compress_Next(T val, T prevVal, std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
		char szErrInfo[MAX_ERROR_INFO];
		sprintf_s(szErrInfo, "= > CFieldCompress Compress_Next field [%d] is unknow data type!", nIndex + 1);
#ifdef PRINT_DEBUG_INFO
		std::cout << szErrInfo << std::endl;
#endif
		return MINICC_IllegalDataType;
	}

	template<>
	int Compress_First(char val, std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First char val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != 0) {
			nRetCode = Compress_Char(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_Next(char val, char prevVal, std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next char val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != prevVal) {
			nRetCode = Compress_Char(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_First(unsigned char val, std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First unsigned char val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != 0) {
			nRetCode = Compress_UINT8(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_Next(unsigned char val, unsigned char prevVal,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next unsigned char val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != prevVal) {
			nRetCode = Compress_UINT8(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_First(int16_t val, std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First int16_t val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != 0) {
			nRetCode = Compress_INT16(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_Next(int16_t val, int16_t prevVal,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next int16_t val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != prevVal) {
			nRetCode = Compress_INT16(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_First(uint16_t val, std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First uint16_t val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != 0) {
			nRetCode = Compress_UINT16(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_Next(uint16_t val, uint16_t prevVal,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next uint16_t val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != prevVal) {
			nRetCode = Compress_UINT16(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_First(int val, std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First int val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != 0) {
			nRetCode = Compress_INT32(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_Next(int val, int prevVal, std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next int val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != prevVal) {
			nRetCode = Compress_INT32(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_First(uint32_t val, std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First uint32_t val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != 0) {
			nRetCode = Compress_UINT32(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_Next(uint32_t val, uint32_t prevVal,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next uint32_t val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != prevVal) {
			nRetCode = Compress_UINT32(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_First(float val, std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First float val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != 0.0) {
			nRetCode = Compress_Float(pDst, unDstlen, val, s_nCompRetDecDigits);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_Next(float val, float prevVal,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next float val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != prevVal) {
			nRetCode = Compress_Float(pDst, unDstlen, val, s_nCompRetDecDigits);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_First(int64_t val, std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First int64_t val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != 0) {
			nRetCode = Compress_INT64(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_Next(int64_t val, int64_t prevVal,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next int64_t val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != prevVal) {
			nRetCode = Compress_INT64(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_First(uint64_t val, std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First uint64_t val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != 0) {
			nRetCode = Compress_UINT64(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_Next(uint64_t val, uint64_t prevVal,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next uint64_t val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != prevVal) {
			nRetCode = Compress_UINT64(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_First(double val, std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First double val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != 0.0) {
			nRetCode = Compress_Double(pDst, unDstlen, val, s_nCompRetDecDigits);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_Next(double val, double prevVal,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next double val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != prevVal) {
			nRetCode = Compress_Double(pDst, unDstlen, val, s_nCompRetDecDigits);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_First(bool val, std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First bool val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != 0) {
			nRetCode = Compress_UINT8(pDst, unDstlen, (unsigned char)val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_Next(bool val, bool prevVal,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next bool val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != prevVal) {
			nRetCode = Compress_UINT8(pDst, unDstlen, (unsigned char)val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;

	}

	template<>
	int Compress_First(const TCHAR* val, std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First const TCHAR* val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (strlen(val) != 0) {
			nRetCode = Compress_String(pDst, unDstlen, (char *)val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_Next(const TCHAR* val, const TCHAR* prevVal,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next const TCHAR* val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (strcmp(val, prevVal) != 0) {
			nRetCode = Compress_String(pDst, unDstlen, (char *)val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_First(std::string val,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First std::string val = " << val.c_str() << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (strlen(val.c_str()) != 0) {
			nRetCode = Compress_String(pDst, unDstlen, (char *)val.c_str());
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_Next(std::string val, std::string prevVal,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next std::string val = " << val.c_str() << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (strcmp(val.c_str(), prevVal.c_str()) != 0) {
			nRetCode = Compress_String(pDst, unDstlen, (char *)val.c_str());
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_First(const basic_string<TCHAR>& val,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First const basic_string<TCHAR>& val = " << val.c_str() << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (strlen(val.c_str()) != 0) {
			nRetCode = Compress_String(pDst, unDstlen, (char *)val.c_str());
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<>
	int Compress_Next(const basic_string<TCHAR>& val, const basic_string<TCHAR>& prevVal,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next const basic_string<TCHAR>& val = " << val.c_str() << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (strcmp(val.c_str(), prevVal.c_str()) != 0) {
			nRetCode = Compress_String(pDst, unDstlen, (char *)val.c_str());
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	//过滤指针的情况.
	template<typename T>
	typename std::enable_if<is_pointer_ext<T>::value, int>::type Compress_First(T val,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
		char szErrInfo[MAX_ERROR_INFO];
		sprintf_s(szErrInfo, "= > CFieldCompress field [%d] is error, not surpport dynamic pointer!", nIndex + 1);		
#ifdef PRINT_DEBUG_INFO
		std::cout << szErrInfo << std::endl;
#endif
		return MINICC_IllegalDataType;
	}

	template<typename T>
	typename std::enable_if<is_pointer_ext<T>::value, int>::type Compress_Next(T val, T prevVal,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
		char szErrInfo[MAX_ERROR_INFO];
		sprintf_s(szErrInfo, "= > CFieldCompress field [%d] is error, not surpport dynamic pointer!", nIndex + 1);
#ifdef PRINT_DEBUG_INFO
		std::cout << szErrInfo << std::endl;
#endif
		return MINICC_IllegalDataType;
	}

	template<typename T>
	typename std::enable_if<is_enum_type<T>::value, int>::type Compress_First(T val,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First is_enum_type val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != 0) {
			nRetCode = Compress_INT32(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	template<typename T>
	typename std::enable_if<is_enum_type<T>::value, int>::type Compress_Next(T val, T prevVal,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
	std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next is_enum_type val = " << val << std::endl;
#endif
		int nRetCode = MINICC_OK;
		if (val != prevVal) {
			nRetCode = Compress_INT32(pDst, unDstlen, val);
			SetQuickCompEncode(pMapData, nIndex);
			QuickCompEncodeAdd(pDst, unDstlen, nRetCode);
		}

		return nRetCode;
	}

	//******压缩定长数组******.

	template <unsigned N, typename T>
	int Compress_First(T(&val)[N],\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
		char szErrInfo[MAX_ERROR_INFO];
		sprintf_s(szErrInfo, "= > CFieldCompress Compress_First(T(&val)[N],...) field [%d] is unknow data type!", nIndex + 1);
#ifdef PRINT_DEBUG_INFO
		std::cout << szErrInfo << std::endl;
#endif
		return MINICC_IllegalDataType;
	}
	template <unsigned N, typename T>
	int Compress_Next(T(&val)[N], T(&prevVal)[N],\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
		char szErrInfo[MAX_ERROR_INFO];
		sprintf_s(szErrInfo, "= > CFieldCompress Compress_Next(T(&val)[N], T(&prevVal)[N],...) field [%d] is unknow data type!", nIndex + 1);
#ifdef PRINT_DEBUG_INFO
		std::cout << szErrInfo << std::endl;
#endif
		return MINICC_IllegalDataType;
	}

	//压缩定长char字符串数组.
	template <unsigned N>
	int Compress_First(char(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First(char(&val)[N] N = " << N << std::endl;
#endif
		return CompressArray_First(val, N, nIndex, pMapData, pDst, unDstlen);
	}
	template <unsigned N>
	int Compress_Next(char(&val)[N], char(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next(char(&val)[N], char(&prevVal)[N] N = " << N << std::endl;
#endif
		return CompressArray_Next(val, prevVal, N, nIndex, pMapData, pDst, unDstlen);
	}

	//压缩定长unsigned char字符串数组.
	template <unsigned N>
	int Compress_First(unsigned char(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First(unsigned char(&val)[N] N = " << N << std::endl;
#endif
		return CompressArray_First(val, N, nIndex, pMapData, pDst, unDstlen);
	}
	template <unsigned N>
	int Compress_Next(unsigned char(&val)[N], unsigned char(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next(unsigned char(&val)[N], unsigned char(&prevVal)[N] N = " << N << std::endl;
#endif
		return CompressArray_Next(val, prevVal, N, nIndex, pMapData, pDst, unDstlen);
	}

	//压缩定长int16_t数组.
	template <unsigned N>
	int Compress_First(int16_t(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First(int16_t(&val)[N] N = " << N << std::endl;
#endif
		return CompressArray_First(val, N, nIndex, pMapData, pDst, unDstlen);
	}
	template <unsigned N>
	int Compress_Next(int16_t(&val)[N], int16_t(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next(int16_t(&val)[N], int16_t(&prevVal)[N] N = " << N << std::endl;
#endif
		return CompressArray_Next(val, prevVal, N, nIndex, pMapData, pDst, unDstlen);
	}

	//压缩定长uint16_t数组.
	template <unsigned N>
	int Compress_First(uint16_t(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First(uint16_t(&val)[N] N = " << N << std::endl;
#endif
		return CompressArray_First(val, N, nIndex, pMapData, pDst, unDstlen);
	}
	template <unsigned N>
	int Compress_Next(uint16_t(&val)[N], uint16_t(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next(uint16_t(&val)[N], uint16_t(&prevVal)[N] N = " << N << std::endl;
#endif
		return CompressArray_Next(val, prevVal, N, nIndex, pMapData, pDst, unDstlen);
	}

	//压缩定长int数组.
	template <unsigned N>
	int Compress_First(int(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First(int(&val)[N] N = " << N << std::endl;
#endif
		return CompressArray_First(val, N, nIndex, pMapData, pDst, unDstlen);
	}
	template <unsigned N>
	int Compress_Next(int(&val)[N], int(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next(int(&val)[N], int(&prevVal)[N] N = " << N << std::endl;
#endif
		return CompressArray_Next(val, prevVal, N, nIndex, pMapData, pDst, unDstlen);
	}

	//压缩定长uint32_t数组.
	template <unsigned N>
	int Compress_First(uint32_t(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First(int(&val)[N] N = " << N << std::endl;
#endif
		return CompressArray_First(val, N, nIndex, pMapData, pDst, unDstlen);
	}
	template <unsigned N>
	int Compress_Next(uint32_t(&val)[N], uint32_t(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next(uint32_t(&val)[N], uint32_t(&prevVal)[N] N = " << N << std::endl;
#endif
		return CompressArray_Next(val, prevVal, N, nIndex, pMapData, pDst, unDstlen);
	}

	//压缩定长float数组.
	template <unsigned N>
	int Compress_First(float(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First(float(&val)[N] N = " << N << std::endl;
#endif
		return CompressArray_First(val, N, nIndex, pMapData, pDst, unDstlen);
	}
	template <unsigned N>
	int Compress_Next(float(&val)[N], float(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next(float(&val)[N], float(&prevVal)[N] N = " << N << std::endl;
#endif
		return CompressArray_Next(val, prevVal, N, nIndex, pMapData, pDst, unDstlen);
	}

	//压缩定长int64_t数组.
	template <unsigned N>
	int Compress_First(int64_t(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First(int64_t(&val)[N] N = " << N << std::endl;
#endif
		return CompressArray_First(val, N, nIndex, pMapData, pDst, unDstlen);
	}
	template <unsigned N>
	int Compress_Next(int64_t(&val)[N], int64_t(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next(int64_t(&val)[N], int64_t(&prevVal)[N] N = " << N << std::endl;
#endif
		return CompressArray_Next(val, prevVal, N, nIndex, pMapData, pDst, unDstlen);
	}

	//压缩定长uint64_t数组.
	template <unsigned N>
	int Compress_First(uint64_t(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First(uint64_t(&val)[N] N = " << N << std::endl;
#endif
		return CompressArray_First(val, N, nIndex, pMapData, pDst, unDstlen);
	}
	template <unsigned N>
	int Compress_Next(uint64_t(&val)[N], uint64_t(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next(uint64_t(&val)[N], uint64_t(&prevVal)[N] N = " << N << std::endl;
#endif
		return CompressArray_Next(val, prevVal, N, nIndex, pMapData, pDst, unDstlen);
	}

	//压缩定长double数组.
	template <unsigned N>
	int Compress_First(double(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First(double(&val)[N] N = " << N << std::endl;
#endif
		return CompressArray_First(val, N, nIndex, pMapData, pDst, unDstlen);
	}
	template <unsigned N>
	int Compress_Next(double(&val)[N], double(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next(double(&val)[N], double(&prevVal)[N] N = " << N << std::endl;
#endif
		return CompressArray_Next(val, prevVal, N, nIndex, pMapData, pDst, unDstlen);
	}

	//压缩定长bool数组.
	template <unsigned N>
	int Compress_First(bool(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_First(bool(&val)[N] N = " << N << std::endl;
#endif
		return CompressArray_First(val, N, nIndex, pMapData, pDst, unDstlen);
	}
	template <unsigned N>
	int Compress_Next(bool(&val)[N], bool(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] Compress_Next(bool(&val)[N], bool(&prevVal)[N] N = " << N << std::endl;
#endif
		return CompressArray_Next(val, prevVal, N, nIndex, pMapData, pDst, unDstlen);
	}


	//*****压缩定长非字符串数组 统一操作方法******.

	template<typename Array>
	typename std::enable_if<!is_string<Array>::value, int>::type CompressArray_First(Array& val, int nItems,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] enable_if<!is_string<Array>::value, int>::type CompressArray_First" << std::endl;
#endif
		int nRetCode = MINICC_OK;
		SetQuickCompEncode(pMapData, nIndex);

		char *pArrMapData = (char *)pDst;
		int nBytes = SetMapSizeByAmount(pArrMapData, unDstlen, nItems);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		for (int i = 0; i < nItems; i++)
		{
			nRetCode = Compress_First(val[i], i, pArrMapData, pDst, unDstlen);
			CHECK_RETCODE(nRetCode);
		}

		return nRetCode;
	}
	template<typename Array>
	typename std::enable_if<!is_string<Array>::value, int>::type CompressArray_Next(Array& val, Array& prevVal, int nItems,\
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] enable_if<!is_string<Array>::value, int>::type CompressArray_Next" << std::endl;
#endif
		int nRetCode = MINICC_OK;
		SetQuickCompEncode(pMapData, nIndex);

		char *pArrMapData = (char *)pDst;
		int nBytes = SetMapSizeByAmount(pArrMapData, unDstlen, nItems);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		for (int i = 0; i < nItems; i++) {
			nRetCode = Compress_Next(val[i], prevVal[i], i, pArrMapData, pDst, unDstlen);
			CHECK_RETCODE(nRetCode);
		}

		return nRetCode;
	}

	//******压缩定长字符串数组 统一操作方法******.

	template<typename Array>
	typename std::enable_if<is_string<Array>::value, int>::type CompressArray_First(Array& val, int nItems, \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] enable_if<is_string<Array>::value, int>::type CompressArray_First" << std::endl;
#endif
		int nRetCode = MINICC_OK;
		SetQuickCompEncode(pMapData, nIndex);

		char *pArrMapData = (char *)pDst;
		int nBytes = SetMapSizeByAmount(pArrMapData, unDstlen, nItems);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		//for (auto i : val)
		for (int i = 0; i < nItems; i++) {
			nRetCode = Compress_First(val[i], i, pArrMapData, pDst, unDstlen);
			CHECK_RETCODE(nRetCode);
		}

		return nRetCode;
	}
	template<typename Array>
	typename std::enable_if<is_string<Array>::value, int>::type CompressArray_Next(Array& val, Array& prevVal, int nItems, \
		std::size_t nIndex, char *&pMapData, char *&pDst, uint32_t &unDstlen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldCompress nIndex [" << nIndex << "] enable_if<is_string<Array>::value, int>::type CompressArray_Next" << std::endl;
#endif
		int nRetCode = MINICC_OK;
		SetQuickCompEncode(pMapData, nIndex);

		char *pArrMapData = (char *)pDst;
		int nBytes = SetMapSizeByAmount(pArrMapData, unDstlen, nItems);
		QuickCompEncodeAdd(pDst, unDstlen, nBytes);

		for (int i = 0; i < nItems; i++) {
			nRetCode = Compress_Next(val[i], prevVal[i], i, pArrMapData, pDst, unDstlen);
			CHECK_RETCODE(nRetCode);
		}

		return nRetCode;
	}
};
} // namespace minicc