#pragma once

/**
* @Brief		字段解压方法定义.
*
*/

#include <tchar.h>
#include <iostream>
#include "traits.hpp"
#include "Common.hpp"
#include "BasicTypeDecompress.hpp"

using namespace std;

namespace minicc
{
///字段解压.
class CFieldDecompress : NonCopyable
{
public:
	CFieldDecompress()
	{
	}
	~CFieldDecompress()
	{
	}

public:

	template<typename T>
	int Decompress_First(T &val, std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		char szErrInfo[MAX_ERROR_INFO];
		sprintf_s(szErrInfo, "= > CFieldDecompress Decompress_First field [%d] is unknow data type!", nIndex + 1);
#ifdef PRINT_DEBUG_INFO
		std::cout << szErrInfo << std::endl;
#endif
		//throw std::invalid_argument(szErrInfo);
		return MINICC_IllegalDataType;
	}

	template<typename T>
	int Decompress_Next(T &val, T &prevVal, std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{	
		char szErrInfo[MAX_ERROR_INFO];
		sprintf_s(szErrInfo, "= > CFieldDecompress Decompress_Next field [%d] is unknow data type!", nIndex + 1);
#ifdef PRINT_DEBUG_INFO
		std::cout << szErrInfo << std::endl;
#endif
		return MINICC_IllegalDataType;
	}

	template<>
	int Decompress_First(char &val, std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_Char(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = 0;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First char val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_Next(char &val, char &prevVal, std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_Char(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = prevVal;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next char val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_First(unsigned char &val, std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_UINT8(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = 0;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First unsigned char val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_Next(unsigned char &val, unsigned char &prevVal, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_UINT8(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = prevVal;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next unsigned char val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_First(int16_t &val, std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_INT16(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = 0;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First int16_t val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_Next(int16_t &val, int16_t &prevVal, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_INT16(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = prevVal;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next int16_t val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_First(uint16_t &val, std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_UINT16(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = 0;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First uint16_t val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_Next(uint16_t &val, uint16_t &prevVal, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_UINT16(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = prevVal;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next uint16_t val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_First(int &val, std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_INT32(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = 0;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First int val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_Next(int &val, int &prevVal, std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_INT32(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = prevVal;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next int val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_First(uint32_t &val, std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_UINT32(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = 0;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First uint32_t val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_Next(uint32_t &val, uint32_t &prevVal, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_UINT32(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = prevVal;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next uint32_t val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_First(float &val, std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_Float(pSrc, unSrclen, val, s_nCompRetDecDigits);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = 0.0;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First float val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_Next(float &val, float &prevVal, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_Float(pSrc, unSrclen, val, s_nCompRetDecDigits);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = prevVal;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next float val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_First(int64_t &val, std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_INT64(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = 0;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First int64_t val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_Next(int64_t &val, int64_t &prevVal, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_INT64(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = prevVal;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next int64_t val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_First(uint64_t &val, std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_UINT64(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = 0;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First uint64_t val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_Next(uint64_t &val, uint64_t &prevVal, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_UINT64(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = prevVal;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next uint64_t val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_First(double &val, std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_Double(pSrc, unSrclen, val, s_nCompRetDecDigits);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = 0.0;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First double val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_Next(double &val, double &prevVal, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_Double(pSrc, unSrclen, val, s_nCompRetDecDigits);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = prevVal;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next double val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_First(bool &val, std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_UINT8(pSrc, unSrclen, (unsigned char &)val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = 0;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First bool val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_Next(bool &val, bool &prevVal, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_UINT8(pSrc, unSrclen, (unsigned char &)val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = prevVal;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next bool val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_First(const TCHAR*& val, std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_String(pSrc, unSrclen, (char *)val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			//strcpy((TCHAR*)val, "");
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First const TCHAR* val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_Next(const TCHAR*& val, const TCHAR*& prevVal, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_String(pSrc, unSrclen, (char *)val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			//memcpy((TCHAR*)val, prevVal, strlen(prevVal));
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next const TCHAR* val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_First(std::string &val, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_String(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = "";
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First std::string val = " << val.c_str() << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_Next(std::string &val, std::string &prevVal, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_String(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = prevVal;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next std::string val = " << val.c_str() << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_First(const basic_string<TCHAR>& val, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_String(pSrc, unSrclen, (TCHAR *)val.c_str());
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			//strcpy((TCHAR*)val.c_str(), "");
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First const basic_string<TCHAR>& val = " << val.c_str() << std::endl;
#endif
		return nRetCode;
	}

	template<>
	int Decompress_Next(const basic_string<TCHAR>& val, const basic_string<TCHAR>& prevVal, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_String(pSrc, unSrclen, (TCHAR *)val.c_str());
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			//memcpy((TCHAR *)val.c_str(), prevVal.c_str(), strlen(prevVal.c_str()));
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next const basic_string<TCHAR>& val = " << val.c_str() << std::endl;
#endif
		return nRetCode;
	}

	//过滤指针的情况.
	template<typename T>
	typename std::enable_if<is_pointer_ext<T>::value, int>::type Decompress_First(T &val, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		char szErrInfo[MAX_ERROR_INFO];
		sprintf_s(szErrInfo, "= > CFieldDecompress field [%d] is error, not surpport dynamic pointer!", nIndex + 1);
#ifdef PRINT_DEBUG_INFO
		std::cout << szErrInfo << std::endl;
#endif
		//throw std::invalid_argument(szErrInfo);
		return MINICC_IllegalDataType;
	}
	template<typename T>
	typename std::enable_if<is_pointer_ext<T>::value, int>::type Decompress_Next(T &val, T prevVal, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		char szErrInfo[MAX_ERROR_INFO];
		sprintf_s(szErrInfo, "= > CFieldDecompress field [%d] is error, not surpport dynamic pointer!", nIndex + 1);
#ifdef PRINT_DEBUG_INFO
		std::cout << szErrInfo << std::endl;
#endif
		//throw std::invalid_argument(szErrInfo);
		return MINICC_IllegalDataType;
	}

	template<typename T>
	typename std::enable_if<is_enum_type<T>::value, int>::type Decompress_First(T &val, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_UINT8(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = 0;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First is_enum_type val = " << val << std::endl;
#endif
		return nRetCode;
	}

	template<typename T>
	typename std::enable_if<is_enum_type<T>::value, int>::type Decompress_Next(T &val, T prevVal, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		int nRetCode = MINICC_OK;
		if (IsEncoded(pMapData, nIndex)) {
			nRetCode = Decompress_UINT8(pSrc, unSrclen, val);
			QuickCompDecodeAdd(pSrc, unSrclen, nRetCode);
		}
		else {
			val = prevVal;
		}
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next is_enum_type val = " << val << std::endl;
#endif
		return nRetCode;
	}

	//******解压定长数组******.

	template <unsigned N, typename T>
	int Decompress_First(T(&val)[N],\
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		char szErrInfo[MAX_ERROR_INFO];
		sprintf_s(szErrInfo, "= > CFieldDecompress Decompress_First(T(&val)[N],...) field [%d] is unknow data type!", nIndex + 1);
#ifdef PRINT_DEBUG_INFO
		std::cout << szErrInfo << std::endl;
#endif
		return MINICC_IllegalDataType;
		//return DecompressArray_First(val, N, nIndex, pMapData, pSrc, unSrclen);
	}

	template <unsigned N, typename T>
	int Decompress_Next(T(&val)[N], T(&prevVal)[N],\
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
		char szErrInfo[MAX_ERROR_INFO];
		sprintf_s(szErrInfo, "= > CFieldDecompress Decompress_Next(T(&val)[N], T(&prevVal)[N],...) field [%d] is unknow data type!", nIndex + 1);
#ifdef PRINT_DEBUG_INFO
		std::cout << szErrInfo << std::endl;
#endif
		return MINICC_IllegalDataType;
		//return DecompressArray_Next(val, N, prevVal, N, nIndex, pMapData, pSrc, unSrclen);
	}

	//解压定长char字符串数组.
	template <unsigned N>
	int Decompress_First(char(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First(char(&val)[N] N = " << N << std::endl;
#endif
		return DecompressArray_First(val, N, nIndex, pMapData, pSrc, unSrclen);
	}
	template <unsigned N>
	int Decompress_Next(char(&val)[N], char(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next(char(&val)[N], char(&prevVal)[N] N = " << N << std::endl;
#endif
		return DecompressArray_Next(val, prevVal, N, nIndex, pMapData, pSrc, unSrclen);
	}

	//解压定长unsigned char字符串数组.
	template <unsigned N>
	int Decompress_First(unsigned char(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First(unsigned char(&val)[N] N = " << N << std::endl;
#endif
		return DecompressArray_First(val, N, nIndex, pMapData, pSrc, unSrclen);
	}
	template <unsigned N>
	int Decompress_Next(unsigned char(&val)[N], unsigned char(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next(unsigned char(&val)[N], unsigned char(&prevVal)[N] N = " << N << std::endl;
#endif
		return DecompressArray_Next(val, prevVal, N, nIndex, pMapData, pSrc, unSrclen);
	}

	//解压定长int16_t数组.
	template <unsigned N>
	int Decompress_First(int16_t(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First(int16_t(&val)[N] N = " << N << std::endl;
#endif
		return DecompressArray_First(val, N, nIndex, pMapData, pSrc, unSrclen);
	}
	template <unsigned N>
	int Decompress_Next(int16_t(&val)[N], int16_t(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next(int16_t(&val)[N], int16_t(&prevVal)[N] N = " << N << std::endl;
#endif
		return DecompressArray_Next(val, prevVal, N, nIndex, pMapData, pSrc, unSrclen);
	}

	//解压定长uint16_t数组.
	template <unsigned N>
	int Decompress_First(uint16_t(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First(uint16_t(&val)[N] N = " << N << std::endl;
#endif
		return DecompressArray_First(val, N, nIndex, pMapData, pSrc, unSrclen);
	}
	template <unsigned N>
	int Decompress_Next(uint16_t(&val)[N], uint16_t(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next(uint16_t(&val)[N], uint16_t(&prevVal)[N] N = " << N << std::endl;
#endif
		return DecompressArray_Next(val, prevVal, N, nIndex, pMapData, pSrc, unSrclen);
	}

	//解压定长int数组.
	template <unsigned N>
	int Decompress_First(int(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First(int(&val)[N] N = " << N << std::endl;
#endif
		return DecompressArray_First(val, N, nIndex, pMapData, pSrc, unSrclen);
	}
	template <unsigned N>
	int Decompress_Next(int(&val)[N], int(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next(int(&val)[N], int(&prevVal)[N] N = " << N << std::endl;
#endif
		return DecompressArray_Next(val, prevVal, N, nIndex, pMapData, pSrc, unSrclen);
	}

	//解压定长uint32_t数组.
	template <unsigned N>
	int Decompress_First(uint32_t(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First(int(&val)[N] N = " << N << std::endl;
#endif
		return DecompressArray_First(val, N, nIndex, pMapData, pSrc, unSrclen);
	}
	template <unsigned N>
	int Decompress_Next(uint32_t(&val)[N], uint32_t(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next(uint32_t(&val)[N], uint32_t(&prevVal)[N] N = " << N << std::endl;
#endif
		return DecompressArray_Next(val, prevVal, N, nIndex, pMapData, pSrc, unSrclen);
	}

	//解压定长float数组.
	template <unsigned N>
	int Decompress_First(float(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First(float(&val)[N] N = " << N << std::endl;
#endif
		return DecompressArray_First(val, N, nIndex, pMapData, pSrc, unSrclen);
	}
	template <unsigned N>
	int Decompress_Next(float(&val)[N], float(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next(float(&val)[N], float(&prevVal)[N] N = " << N << std::endl;
#endif
		return DecompressArray_Next(val, prevVal, N, nIndex, pMapData, pSrc, unSrclen);
	}

	//解压定长int64_t数组.
	template <unsigned N>
	int Decompress_First(int64_t(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First(int64_t(&val)[N] N = " << N << std::endl;
#endif
		return DecompressArray_First(val, N, nIndex, pMapData, pSrc, unSrclen);
	}
	template <unsigned N>
	int Decompress_Next(int64_t(&val)[N], int64_t(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next(int64_t(&val)[N], int64_t(&prevVal)[N] N = " << N << std::endl;
#endif
		return DecompressArray_Next(val, prevVal, N, nIndex, pMapData, pSrc, unSrclen);
	}

	//解压定长uint64_t数组.
	template <unsigned N>
	int Decompress_First(uint64_t(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First(uint64_t(&val)[N] N = " << N << std::endl;
#endif
		return DecompressArray_First(val, N, nIndex, pMapData, pSrc, unSrclen);
	}
	template <unsigned N>
	int Decompress_Next(uint64_t(&val)[N], uint64_t(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next(uint64_t(&val)[N], uint64_t(&prevVal)[N] N = " << N << std::endl;
#endif
		return DecompressArray_Next(val, prevVal, N, nIndex, pMapData, pSrc, unSrclen);
	}

	//解压定长double数组.
	template <unsigned N>
	int Decompress_First(double(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First(double(&val)[N] N = " << N << std::endl;
#endif
		return DecompressArray_First(val, N, nIndex, pMapData, pSrc, unSrclen);
	}
	template <unsigned N>
	int Decompress_Next(double(&val)[N], double(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next(double(&val)[N], double(&prevVal)[N] N = " << N << std::endl;
#endif
		return DecompressArray_Next(val, prevVal, N, nIndex, pMapData, pSrc, unSrclen);
	}

	//解压定长bool数组.
	template <unsigned N>
	int Decompress_First(bool(&val)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_First(bool(&val)[N] N = " << N << std::endl;
#endif
		return DecompressArray_First(val, N, nIndex, pMapData, pSrc, unSrclen);
	}
	template <unsigned N>
	int Decompress_Next(bool(&val)[N], bool(&prevVal)[N], \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] Decompress_Next(bool(&val)[N], bool(&prevVal)[N] N = " << N << std::endl;
#endif
		return DecompressArray_Next(val, prevVal, N, nIndex, pMapData, pSrc, unSrclen);
	}

	//*****解压定长非字符串数组 统一操作方法******.

	//解压定长数组.
	template<typename Array>
	typename std::enable_if<!is_string<Array>::value, int>::type DecompressArray_First(Array& val, int nItems, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] enable_if<!is_string<Array>::value, int>::type DecompressArray_First" << std::endl;
#endif
		int nRetCode = MINICC_OK;
		char *pArrMapData = (char *)pSrc;
		int nMapSize = GetMapSize(pArrMapData, unSrclen);
		QuickCompDecodeAdd(pSrc, unSrclen, nMapSize);

		for (int i = 0; i < nItems; i++) {
			nRetCode = Decompress_First(val[i], i, pArrMapData, pSrc, unSrclen);
			CHECK_RETCODE(nRetCode);
		}

		return nRetCode;
	}

	template<typename Array>
	typename std::enable_if<!is_string<Array>::value, int>::type DecompressArray_Next(Array& val, Array& prevVal, int nItems, \
		std::size_t nIndex, char *&pMapData, char *&pSrc, uint32_t &unSrclen)
	{
#ifdef PRINT_DEBUG_INFO
		std::cout << "= > CFieldDecompress nIndex [" << nIndex << "] enable_if<is_string<Array>::value, int>::type DecompressArray_Next" << std::endl;
#endif
		int nRetCode = MINICC_OK;
		char *pArrMapData = (char *)pSrc;
		int nMapSize = GetMapSize(pArrMapData, unSrclen);
		QuickCompDecodeAdd(pSrc, unSrclen, nMapSize);

		for (int i = 0; i < nItems; i++) {
			nRetCode = Decompress_Next(val[i], prevVal[i], i, pArrMapData, pSrc, unSrclen);
			CHECK_RETCODE(nRetCode);
		}

		return nRetCode;
	}
};
} // namespace minicc