#include "stdafx.h"
#include "DayKlineTest.h"
#include <stdio.h>
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "Wininet.lib")

#include "SimpleCompressor.hpp"
#include "SimpleDecompressor.hpp"

#include "ComplexCompressor.hpp"
#include "ComplexDecompressor.hpp"


///测试循环数.
#define DO_TEST_TIMES  1000
///压缩数据特性处理.
#define DATA_FEATURE_HANDLE


void CheckRetCode(char *szMark, int nRetCode)
{
	if (nRetCode != MINICC_OK) {
		std::cout << szMark << "操作失败：RetCode = " << nRetCode << std::endl;
	}
}

CDayKlineTest::CDayKlineTest()
{
}


CDayKlineTest::~CDayKlineTest()
{
}

int CDayKlineTest::ReplaceAll(std::string& str, const std::string& pattern, const std::string& newpat)
{
	int count = 0;
	const size_t nsize = newpat.size();
	const size_t psize = pattern.size();

	for (size_t pos = str.find(pattern, 0);
		pos != std::string::npos;
		pos = str.find(pattern, pos + nsize))
	{
		str.replace(pos, psize, newpat);
		count++;
	}

	return count;
}


int CDayKlineTest::ParseKDataRecord(std::string &strRecord, MarketData<int> &dateMarketData)
{
	static int nRetCode = -1;
	static int nYear = 0;           //日期
	static int nMonth = 0;
	static int nDate = 0;
	static double dOpen = 0.0;		//开盘价
	static double dClose = 0.0;   	//收盘价
	static double dSD = 0.0;        //涨跌
	static double dRise = 0.0;      //涨幅
	static double dLow = 0.0;		//最低价
	static double dHigh = 0.0;		//最高价
	static __int64 iVolume = 0;	    //成交量(手)
	static double dTurover = 0.0;	//成交额
	static double dTurnoverRate = 0.0;//换手率

	ReplaceAll(strRecord, "%", "0");
	nRetCode = sscanf_s(strRecord.c_str(), "[\"%d-%d-%d\",\"%lf\", \"%lf\", \"%lf\", \"%lf\", \"%lf\", \"%lf\", \"%I64d\", \"%lf\", \"%lf\"]",
		&nYear, &nMonth, &nDate, &dOpen, &dClose, &dSD, &dRise, &dLow, &dHigh, &iVolume, &dTurover, &dTurnoverRate);
	_ASSERT(nRetCode == 12);

	//["2016-12-30", "16.07", "16.21", "0.14", "0.87%", "16.04", "16.23", "122622", "19765.39", "0.06%"]
	dateMarketData.nDate = nYear * 10000 + nMonth * 100 + nDate;
	dateMarketData.nOpen = (int)(dOpen*10000 + 0.000005);		//开盘价（*10000得原值）
	dateMarketData.nClose = (int)(dClose * 10000 + 0.000005);   	//收盘价（*10000得原值）
	dateMarketData.nSD = (int)(dSD * 10000 + 0.000005);        //涨跌（*10000得原值）
	dateMarketData.nRise = (int)(dRise * 100 + 0.0005);      //涨幅（*100得原值）(%).
	dateMarketData.nLow = (int)(dLow * 10000 + 0.000005);		//最低价（*10000得原值）
	dateMarketData.nHigh = (int)(dHigh * 10000 + 0.000005);		//最高价（*10000得原值）
	dateMarketData.iVolume = iVolume;	//成交量(手)
	dateMarketData.iTurover = (int)(dTurover * 10000 + 0.000005);	//成交额(元)
	dateMarketData.nTurnoverRate = (int)(dTurnoverRate * 100 + 0.0005);//换手率（*100得原值）(%)

	return nRetCode;
}

int CDayKlineTest::GetDayKData(std::string &strHtml, std::vector<MarketData<int>> &vecMarketData)
{
	//搜狐历史数据字段说明.
	//["2016-12-30", "16.07", "16.21", "0.14", "0.87%", "16.04", "16.23", "122622", "19765.39", "0.06%"]
	//日期|开盘价|收盘价|涨跌|涨幅|最低价|最高价|成交量（单位：手）|成交额（单位：万）|换手率|

	std::string strRecord;
	char *pOrigStr = (char *)strHtml.c_str();
	///记录起始指针.
	char *pRecordBPTR = strstr(pOrigStr, ":[[");
	///记录结束指针.
	char *pRecordEPTR = nullptr;

	if (pRecordBPTR) {
		pRecordBPTR += 2;
		pRecordEPTR = strstr(pRecordBPTR, "]");
	}

	while (pRecordBPTR && pRecordEPTR)
	{
		strRecord = std::string(pRecordBPTR, pRecordEPTR - pRecordBPTR + 1);

		///记录读取.
		MarketData<int> dateMarketData;
		ParseKDataRecord(strRecord, dateMarketData);
		vecMarketData.push_back(dateMarketData);

		pRecordBPTR = strstr(pRecordEPTR, ",[");
		if (pRecordBPTR) {
			pRecordBPTR += 1;
			pRecordEPTR = strstr(pRecordBPTR, "]");
		}
	}

	return 0;
}

std::string CDayKlineTest::GetHttpResponse(std::string strURL)
{
	std::string strHtml = _T("");  ///存放网页数据.
	const int MAX_SIZE = 1024;

	HINTERNET hSession = InternetOpen(_T("UrlTest"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		HINTERNET hHttp = InternetOpenUrl(hSession, strURL.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
		if (hHttp != NULL)
		{
			char szTemp[MAX_SIZE];
			ULONG lNumber = 1;
			while (lNumber > 0)
			{
				InternetReadFile(hHttp, szTemp, MAX_SIZE - 1, &lNumber);
				szTemp[lNumber] = '\0';
				strHtml += szTemp;
			}
			InternetCloseHandle(hHttp);
			hHttp = NULL;
		}

		InternetCloseHandle(hSession);
		hSession = NULL;
	}

	return strHtml;
}

void CDayKlineTest::DayKlineCompPerformanceTest(std::vector<MarketData<int>> &vecMarketData)
{
	int nRetCode = -1;

	minicc::CSimpleCompressor cr;
	minicc::CSimpleDecompressor dcr;

	try
	{
		ULONGLONG ulStartTick = GetTickCount64();

		int nUseTicks = 0;
		if (vecMarketData.size() <= 0) {
			throw std::invalid_argument("获取不到行情数据！");
		}

		uint32_t unStructItems = (uint32_t)vecMarketData.size();
		uint32_t unSrcDataSize = sizeof(MarketData<int>) * unStructItems;
		uint32_t unDstlen = cr.GetMaxCompSize(unSrcDataSize, unStructItems);

		cout << "原始长度：" << unSrcDataSize << " | 最大压缩长度：" << unDstlen << endl;

		uint32_t unComplen = 0;
		uint32_t unDecomplen = unDstlen;
		char *pDst = new char[unDstlen];
		memset(pDst, 0, unDstlen);

		///多次测试.
		int nTestTimes = DO_TEST_TIMES;
		if (nTestTimes > 0)
		{
			ULONGLONG ulTatalStartTick = GetTickCount64();
			uint32_t unStructItems = (uint32_t)vecMarketData.size();
			uint32_t unTatalSrcDataSize = sizeof(MarketData<int>) * unStructItems * nTestTimes;
			uint32_t unTatalComplen = 0;

			cout << "循环测试次数：" << nTestTimes << endl;

			ulStartTick = GetTickCount64();

#ifdef DATA_FEATURE_HANDLE
			BeforeCompress(vecMarketData);
#endif

			for (int i = 0; i < nTestTimes; i++) {
				nRetCode = cr.Compress(vecMarketData, pDst, unDstlen, unComplen);
				CheckRetCode("循环压缩", nRetCode);
				unTatalComplen += unComplen;
			}

			nUseTicks = (int)(GetTickCount64() - ulStartTick);
			cout << "压缩累计耗时（ms）：" << nUseTicks << endl;

			ulStartTick = GetTickCount64();
			for (int i = 0; i < nTestTimes; i++) {
				nRetCode = dcr.Decompress(vecMarketData, pDst, unDstlen/*, unDecomplen*/);
				CheckRetCode("循环解压", nRetCode);
			}

#ifdef DATA_FEATURE_HANDLE
			AfterDecompress(vecMarketData);
#endif
			nUseTicks = (int)(GetTickCount64() - ulStartTick);
			cout << "解压累计耗时（ms）：" << nUseTicks << endl;

			nUseTicks = (int)(GetTickCount64() - ulTatalStartTick);
			cout << "压缩解压累计耗时（ms）：" << nUseTicks << endl;
			cout << "原始长度：" << unTatalSrcDataSize / (1024 * 1024 * 1.0) <<\
				    "M | 压缩长度：" << unTatalComplen / (1024 * 1024 * 1.0) <<\
				    "M | 压缩比：" << unTatalComplen / (unTatalSrcDataSize*1.0) <<\
					" 或 " << (unTatalSrcDataSize*1.0) / unTatalComplen << endl;

			if (pDst) {
				delete[] pDst;
				pDst = nullptr;
			}
		}

		if (pDst) {
			delete[] pDst;
			pDst = nullptr;
		}
	}
	catch (std::invalid_argument ex) {
		cout << ex.what() << endl;
	}
	catch (...) {
		cout << "压缩操作未知异常！" << endl;
	}
}

///压缩前处理.
void CDayKlineTest::BeforeCompress(std::vector<MarketData<int>> &vecMarketData)
{
	for (int i = ((int)vecMarketData.size() - 1); i >= 0; i--)
	{
		vecMarketData[i].nOpen -= vecMarketData[i].nLow;
		vecMarketData[i].nClose -= vecMarketData[i].nLow;
		vecMarketData[i].nHigh -= vecMarketData[i].nLow;

		if (i != 0) {
			vecMarketData[i].nDate -= vecMarketData[i - 1].nDate;
		}
	}
}

///解压后处理（压缩前处理反向操作）.
void CDayKlineTest::AfterDecompress(std::vector<MarketData<int>> &vecMarketData)
{
	for (int i = 0; i < (int)vecMarketData.size(); i++)
	{
		vecMarketData[i].nOpen += vecMarketData[i].nLow;
		vecMarketData[i].nClose += vecMarketData[i].nLow;
		vecMarketData[i].nHigh += vecMarketData[i].nLow;

		if (i > 0) {
			vecMarketData[i].nDate += vecMarketData[i - 1].nDate;
		}
	}
}

void CDayKlineTest::DoTest()
{
	cout << "------------------------------------------------" << endl;
	cout << "DayKlineTest begin." << endl;

	int nRetCode = -1;
	std::string strURL = std::string("http://q.stock.sohu.com/hisHq?code=cn_600000&start=20000101&end=20170101&stat=1&order=D&period=d&callback=historySearchHandler&rt=jsonp");
	//std::string strURL = std::string("http://q.stock.sohu.com/hisHq?code=cn_600000&start=20170111&end=20170111&stat=1&order=D&period=d&callback=historySearchHandler&rt=jsonp");
	std::string strHtml = GetHttpResponse(strURL);

	cout << "下载日线数据内容长度：" << strHtml.size() << endl;

	std::vector<MarketData<int>> vecTmpMarketData;
	GetDayKData(strHtml, vecTmpMarketData);

	///简单反向日期排序.
	std::vector<MarketData<int>> vecMarketData;
	for (int i = ((int)vecTmpMarketData.size() - 1); i >= 0; i--) {
		vecMarketData.push_back(vecTmpMarketData[i]);
	}

	cout << "************搜狐日线数据压缩测试************" << endl;

	///日线数据压缩测试.
	DayKlineCompPerformanceTest(vecMarketData);

	cout << "minicc DayKlineTest end." << endl;
	cout << "------------------------------------------------" << endl;
}

