#pragma once

/**
* @Brief		������������ѹ����ѹ����.
*/


#include "stdint.h"
#include <math.h>
#include "AlgorithmComDefine.hpp"


namespace minicc
{

///�ֶ��ֶ�ѹ����ʶ.
#define	SetQuickCompEncode(pMapData, nPlace) pMapData[nPlace/7] |= sg_Bits[nPlace%7+1];

///ѹ������ƫ�Ƽ�ѹ���ֽ�������(ѹ��ʱ�ѱ�֤�����㹻).
#define	QuickCompEncodeAdd(pDst, unDstlen, nBytes)\
	{\
	   if (nBytes >= 0)\
	   	   {\
		   unDstlen -= nBytes; pDst += nBytes;\
	   	   }\
	   	   else{\
		   return nBytes;\
	   }\
	}\

///�����������ѹ������ֵ.
#define CHECK_COMPRESS_RETCODE(nRetCode) if (nRetCode < 0) return nRetCode;

static int Compress_INT64(char *pData, uint32_t &unDataSize, int64_t iValue)
{
	int i = 0;
	bool bSign = (iValue < 0) ? false : true;
	int nNeedBytes = GetNeedBytesOfSigned(iValue);

	///������治��.
	if ((int)unDataSize < nNeedBytes)
	{
		return MINICC_LackDataSize;
	}
	for (i = nNeedBytes - 1; i >= 0; i--)
	{
		pData[i] = (iValue & BIT_FILT);
		iValue = (iValue >> 7);
	}

	pData[nNeedBytes - 1] |= BIT_STOPFLAG;
	if (!bSign) pData[0] |= BIT_SIGNFLAG;

	return nNeedBytes;
}

static int Compress_UINT64(char *pData, uint32_t &unDataSize, uint64_t uiValue)
{
	int i = 0;
	int nNeedBytes = GetNeedBytesOfUnSigned(uiValue);
	for (i = nNeedBytes - 1; i >= 0; i--)
	{
		pData[i] = (uiValue & BIT_FILT);
		uiValue = (uiValue >> 7);
	}
	pData[nNeedBytes - 1] |= BIT_STOPFLAG;

	return nNeedBytes;
}

static int Compress_Char(char *pData, uint32_t &unDataSize, char cValue)
{
	pData[0] = cValue;
	return 1;
}

static int Compress_UINT8(char *pData, uint32_t &unDataSize, uint8_t unValue)
{
	pData[0] = unValue;
	return 1;
}

static int Compress_INT8(char *pData, uint32_t &unDataSize, char nValue)
{
	pData[0] = nValue;
	return 1;
}

static int Compress_BYTE(char *pData, uint32_t &unDataSize, char byValue)
{
	pData[0] = byValue;
	return 1;
}

static int Compress_INT16(char *pData, uint32_t &unDataSize, int16_t nValue)
{
	return Compress_INT64(pData, unDataSize, nValue);
}

static int Compress_UINT16(char *pData, uint32_t &unDataSize, uint16_t unValue)
{
	return Compress_UINT64(pData, unDataSize, unValue);
}

static int Compress_INT32(char *pData, uint32_t &unDataSize, int nValue)
{
	return Compress_INT64(pData, unDataSize, nValue);
}

static int Compress_UINT32(char *pData, uint32_t &unDataSize, uint32_t unValue)
{
	return Compress_UINT64(pData, unDataSize, unValue);
}

static int Compress_Float(char *pData, uint32_t &unDataSize, float fValue, int nDecimal)
{
	int64_t iInteger = (int64_t)fValue;
	uint64_t uiDecimal = (uint64_t)(/*fabs(*/(fValue - iInteger)*pow(10.0, nDecimal)/*+0.05*/);
	int nIntegerBytes = Compress_INT64(pData, unDataSize, iInteger);
	int nDecimalBytes = Compress_UINT64(&pData[nIntegerBytes], unDataSize, ReverseValue_INT64(uiDecimal, nDecimal));
	return nIntegerBytes + nDecimalBytes;
}

static int Compress_Double(char *pData, uint32_t &unDataSize, double dValue, int nDecimal)
{
	int64_t iInteger = (int64_t)dValue;
	uint64_t uiDecimal = (uint64_t)(/*fabs(*/(dValue - iInteger)*pow(10.0, nDecimal)/*+0.05*/);
	int nIntegerBytes = Compress_INT64(pData, unDataSize, iInteger);
	int nDecimalBytes = Compress_UINT64(&pData[nIntegerBytes], unDataSize, ReverseValue_INT64(uiDecimal, nDecimal));
	return nIntegerBytes + nDecimalBytes;
}

static int Compress_String(char *pData, uint32_t &unDataSize, char *pString)
{
	uint32_t nLength = (uint32_t)strlen(pString);
	int nEncodeBytes = Compress_UINT32(pData, unDataSize, nLength);
	memcpy(pData + nEncodeBytes, pString, nLength);

	return nLength + nEncodeBytes;
}

static int Compress_String(char *pData, uint32_t &unDataSize, char *pString, uint32_t unMaxLength)
{
	uint32_t nLength = (uint32_t)strlen(pString);
	if (nLength > unMaxLength) nLength = unMaxLength;
	int nEncodeBytes = Compress_UINT32(pData, unDataSize, nLength);
	memcpy(pData + nEncodeBytes, pString, nLength);

	return nLength + nEncodeBytes;
}

static int Compress_BinaryStr(char *pData, uint32_t &unDataSize, char *pByString, int nSize)
{
	if (nSize < 0) nSize = 0;
	int nEncodeBytes = Compress_INT32(pData, unDataSize, nSize);
	memcpy(pData + nEncodeBytes, pByString, nSize);

	return nSize + nEncodeBytes;
}

} // namespace minicc