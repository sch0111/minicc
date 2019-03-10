#pragma once

/**
* @Brief		基础数据类型压缩解压定义.
*/

#include "stdint.h"
#include <math.h>
#include "AlgorithmComDefine.hpp"
#include <memory.h >

namespace minicc
{

///解压内容偏移及压缩字节数调整.
#define	QuickCompDecodeAdd(pSrc, unSrclen, nBytes)\
	{\
	   if (nBytes >= 0)\
	   {\
		   unSrclen -= nBytes; pSrc += nBytes;\
	   }\
	   else{\
		   return nBytes;\
	   }\
	}\

static int Decompress_Char(char *pData, uint32_t &unDataSize, char &cValue)
{
	if (unDataSize < 1)
	{
		return MINICC_InvalidParams;
	}
	else
	{
		cValue = pData[0];
		return 1;
	}

	return MINICC_InvalidParams;
}

static int Decompress_INT8(char *pData, uint32_t &unDataSize, char &nValue)
{
	if (unDataSize < 1)
	{
		return MINICC_InvalidParams;
	}
	else
	{
		nValue = pData[0];
		return 1;
	}

	return MINICC_InvalidParams;
}

static int Decompress_UINT8(char *pData, uint32_t &unDataSize, uint8_t &unValue)
{
	if (unDataSize < 1)
	{
		return MINICC_InvalidParams;
	}
	else
	{
		unValue = pData[0];
		return 1;
	}

	return MINICC_InvalidParams;
}

static int Decompress_BYTE(char *pData, uint32_t &unDataSize, char &byValue)
{
	if (unDataSize < 1)
	{
		return MINICC_InvalidParams;
	}
	else
	{
		byValue = pData[0];
		return 1;
	}

	return MINICC_InvalidParams;
}

static int Decompress_INT16(char *pData, uint32_t &unDataSize, int16_t &nValue)
{
	nValue = (pData[0] & BIT_SIGNFLAG) ? -1 : 0;
	uint32_t nBytes = 0;
	for (;nBytes<unDataSize;)
	{
		nValue = (nValue << 7) | (pData[nBytes] & BIT_DATA);
		if (pData[nBytes] & BIT_STOPFLAG)
		{
			return nBytes + 1;
		}
		nBytes++;
	}

	return MINICC_InvalidParams;
}

static int Decompress_UINT16(char *pData, uint32_t &unDataSize, uint16_t &unValue)
{
	unValue = 0;
	uint32_t nBytes = 0;
	for (;nBytes<unDataSize;)
	{
		unValue = (unValue << 7) | (pData[nBytes] & BIT_DATA);
		if (pData[nBytes] & BIT_STOPFLAG)
		{
			return nBytes + 1;
		}
		nBytes++;
	}

	return MINICC_InvalidParams;
}

static int Decompress_INT32(char *pData, uint32_t &unDataSize, int &nValue)
{
	nValue = (pData[0] & BIT_SIGNFLAG) ? -1 : 0;
	uint32_t nBytes = 0;
	for (;nBytes<unDataSize;)
	{
		nValue = (nValue << 7) | (pData[nBytes] & BIT_DATA);
		if (pData[nBytes] & BIT_STOPFLAG)
		{
			return nBytes + 1;
		}
		nBytes++;
	}

	return MINICC_InvalidParams;
}

static int Decompress_UINT32(char *pData, uint32_t &unDataSize, uint32_t &unValue)
{
	unValue = 0;
	uint32_t nBytes = 0;
	for (;nBytes<unDataSize;)
	{
		unValue = (unValue << 7) | (pData[nBytes] & BIT_DATA);
		if (pData[nBytes] & BIT_STOPFLAG)
		{
			return nBytes + 1;
		}
		nBytes++;
	}

	return MINICC_InvalidParams;
}

static int Decompress_INT64(char *pData, uint32_t &unDataSize, int64_t &iValue)
{
	iValue = (pData[0] & BIT_SIGNFLAG) ? -1 : 0;
	uint32_t nBytes = 0;
	for (;nBytes<unDataSize;)
	{
		iValue = (iValue << 7) | (pData[nBytes] & BIT_DATA);
		if (pData[nBytes] & BIT_STOPFLAG)
		{
			return nBytes + 1;
		}
		nBytes++;
	}

	return MINICC_InvalidParams;
}

static int Decompress_UINT64(char *pData, uint32_t &unDataSize, uint64_t &uiValue)
{
	uiValue = 0;
	uint32_t nBytes = 0;
	for (;nBytes<unDataSize;)
	{
		uiValue = (uiValue << 7) | (pData[nBytes] & BIT_DATA);
		if (pData[nBytes] & BIT_STOPFLAG)
		{
			return nBytes + 1;
		}
		nBytes++;
	}

	return MINICC_InvalidParams;
}

static int Decompress_Float(char *pData, uint32_t &unDataSize, float &fValue, int nDecimal)
{
	fValue = 0.0;
	int64_t iInteger = 0;
	uint64_t uiDecimal = 0;
	int nIntegerBytes = Decompress_INT64(pData, unDataSize, iInteger);
	if (nIntegerBytes < 0 || (uint32_t)nIntegerBytes >= unDataSize)
	{
		return nIntegerBytes;
	}
	uint32_t nDecimalBytes = Decompress_UINT64(&pData[nIntegerBytes], unDataSize, uiDecimal);
	if (nDecimalBytes < 0)
	{
		return nDecimalBytes;
	}
	if (iInteger >= 0) fValue = iInteger + ReverseValue_INT64(uiDecimal, nDecimal) / (float)pow(10.0, nDecimal);
	else fValue = iInteger - ReverseValue_INT64(uiDecimal, nDecimal) / (float)pow(10.0, nDecimal);

	return nIntegerBytes + nDecimalBytes;
}

static int Decompress_Double(char *pData, uint32_t &unDataSize, double &dValue, int nDecimal)
{
	dValue = 0.0;
	int64_t iInteger = 0;
	uint64_t uiDecimal = 0;
	int nIntegerBytes = Decompress_INT64(pData, unDataSize, iInteger);
	if (nIntegerBytes < 0 || (uint32_t)nIntegerBytes >= unDataSize)
	{
		return nIntegerBytes;
	}
	int nDecimalBytes = Decompress_UINT64(&pData[nIntegerBytes], unDataSize, uiDecimal);
	if (nDecimalBytes < 0)
	{
		return nDecimalBytes;
	}
	if (iInteger >= 0) dValue = iInteger + ReverseValue_INT64(uiDecimal, nDecimal) / pow(10.0, nDecimal);
	else dValue = iInteger - ReverseValue_INT64(uiDecimal, nDecimal) / pow(10.0, nDecimal);

	return nIntegerBytes + nDecimalBytes;
}

static int Decompress_String(char *pData, uint32_t &unDataSize, char *pString)
{
	uint32_t nLength = 0;
	int nDecodeBytes = Decompress_UINT32(pData, unDataSize, nLength);
	if (nDecodeBytes < 0) return nDecodeBytes;
	if (nLength < 0) nLength = 0;
	memcpy(pString, pData + nDecodeBytes, nLength);

	return nDecodeBytes + nLength;
}

static int Decompress_String(char *pData, uint32_t &unDataSize, string &str)
{
	uint32_t nLength = 0;
	int nDecodeBytes = Decompress_UINT32(pData, unDataSize, nLength);
	if (nDecodeBytes < 0) return nDecodeBytes;
	if (nLength < 0) nLength = 0;
	str = std::string(pData + nDecodeBytes, nLength);

	return nDecodeBytes + nLength;
}

static int Decompress_String(char *pData, uint32_t &unDataSize, char *pString, uint32_t unMaxSize)
{
	uint32_t nLength = 0;
	int nDecodeBytes = Decompress_UINT32(pData, unDataSize, nLength);
	if (nDecodeBytes < 0) return nDecodeBytes;
	if (nLength < 0) nLength = 0;
	if (unMaxSize > nLength)
	{
		memset(&pString[nLength], 0, unMaxSize - nLength);
		unMaxSize = nLength;
	}
	memcpy(pString, pData + nDecodeBytes, unMaxSize);

	return nDecodeBytes + nLength;
}

static int Decompress_BinaryStr(char *pData, uint32_t &unDataSize, char *pByString, uint32_t unSize)
{
	uint32_t nLength = 0;
	int nDecodeBytes = Decompress_UINT32(pData, unDataSize, nLength);
	if (nDecodeBytes < 0) return nDecodeBytes;
	if (nLength < 0) nLength = 0;
	if (unSize > nLength)
	{
		memset(&pByString[nLength], 0, unSize - nLength);
		unSize = nLength;
	}
	memcpy(pByString, pData + nDecodeBytes, unSize);

	return nDecodeBytes + nLength;
}
} // namespace minicc