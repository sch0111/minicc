#pragma once

/**
* @System		�㷨ͨ�ö��巽��.
*
*/

#include "Error.hpp"


namespace minicc
{

///���������Ϣ.
//#define PRINT_DEBUG_INFO
///��������Ϣ����.
#define MAX_ERROR_INFO  256
///��鷵��ֵ.
#define CHECK_RETCODE(nRetCode) if (nRetCode < MINICC_OK) return nRetCode;
///������ݴ�С����ǰ��Ҫ��Խ�ѹ���������ݴ��ҵ����ڴ������.
#define LIMIT_DATALENGTH  10*1024*1024


///λ�����������.
#define _BIT0(a)	((~((~(int64_t)0)<<(64-a)))<<a)
#define _BIT1(a)	(~((~(int64_t)0)<<a))

static const int64_t BIT_UNSIGNED[] = { _BIT0(7), _BIT0(14), _BIT0(21), _BIT0(28), _BIT0(35), _BIT0(42), _BIT0(49), _BIT0(56), _BIT0(63) };
static const int64_t BIT_SIGNED[] = { _BIT0(6), _BIT0(13), _BIT0(20), _BIT0(27), _BIT0(34), _BIT0(41), _BIT0(48), _BIT0(55), _BIT0(62) };
static const int64_t BIT_FILT = _BIT1(7);

static const char BIT_STOPFLAG = (char)(0x80);
static const char BIT_SIGNFLAG = (char)(0x40);
static const char BIT_DATA = (char)(0x7F);


///float/double����ѹ������С��λ��.
static int s_nCompRetDecDigits = 4;

///����������ݳ���.
static bool OverLimitDataLength(int nLength)
{
	if (nLength > LIMIT_DATALENGTH) {
		return true;
	}
	else {
		return false;
	}
}

///��ȡunsigned��������ѹ���ֽ���.
static int GetNeedBytesOfUnSigned(uint64_t &uiValue)
{
	int i = 0;
	for (i = 0; i<9; i++)
	{
		if ((uiValue & BIT_UNSIGNED[i]) == 0)
			break;
	}

	return i + 1;
}

///��ȡsigned��������ѹ���ֽ���.
static int GetNeedBytesOfSigned(int64_t &iValue)
{
	int i = 0;
	if (iValue < 0) iValue = (~(iValue)+1);

	for (i = 0; i<9; i++) {
		if ((iValue & BIT_SIGNED[i]) == 0)
			break;
	}

	return i + 1;
}

///��ȡdouble������С��λ����.
static uint64_t ReverseValue_INT64(uint64_t &uiValue, int &nDigit)
{
	if (uiValue == 0) return 0;

	uint64_t uiReverseValue = 0;
	int i;
	for (i = 0; i<nDigit; i++) {
		uiReverseValue *= 10;
		uiReverseValue += uiValue % 10;
		uiValue /= 10;
	}

	return uiReverseValue;
}


static const unsigned char sg_Bits[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

///����ӳ���.
static int SetMapSizeByAmount(char *pMapData, uint32_t unDatalen, int nAmount)
{
	if (nAmount <= 0) return 0;
	int nSize = (nAmount - 1) / 7 + 1;
	if ((uint32_t)nSize > unDatalen) return MINICC_LackDataSize;

	memset(pMapData, 0, nSize);
	pMapData[nSize - 1] |= sg_Bits[0];

	return nSize;
}

///��ȡӳ���.
static int GetMapSize(char *pMapData, int nMaxSize)
{
	int i = 0;
	for (i = 0; i<nMaxSize; i++) {
		if (pMapData[i] & sg_Bits[0])
			return i + 1;
	}
	return 0;
}

///�ж��Ƿ�ѹ��.
static bool IsEncoded(char *pMapData, int nMapSize, int &nPlace)
{
	int nOldPlace = nPlace;
	nPlace++;
	if (nOldPlace < 0 || nOldPlace / 7 >= nMapSize)
		return false;
	else if (pMapData[nOldPlace / 7] & sg_Bits[nOldPlace % 7 + 1])
		return true;
	else
		return false;
}

///�ж��Ƿ�ѹ��.
static bool IsEncoded(char *pMapData, std::size_t &nPlace)
{
	if (nPlace < 0)
		return false;
	else if (pMapData[nPlace / 7] & sg_Bits[nPlace % 7 + 1])
		return true;
	else
		return false;
}

///���ʮ�����ƻ���.
static void PrintBufferByHex(char* pBytes, int nByteSize)
{
	printf("\r\nPrintBufferByHex input size = %d:\r\n", nByteSize);

	for (int i = 0; i<nByteSize; i++) {
		printf(" %02X", (unsigned char)pBytes[i]);
	}
}

} // namespace minicc