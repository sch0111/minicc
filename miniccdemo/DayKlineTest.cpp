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


///����ѭ����.
#define DO_TEST_TIMES  1000
///ѹ���������Դ���.
#define DATA_FEATURE_HANDLE


void CheckRetCode(char *szMark, int nRetCode)
{
	if (nRetCode != MINICC_OK) {
		std::cout << szMark << "����ʧ�ܣ�RetCode = " << nRetCode << std::endl;
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
	static int nYear = 0;           //����
	static int nMonth = 0;
	static int nDate = 0;
	static double dOpen = 0.0;		//���̼�
	static double dClose = 0.0;   	//���̼�
	static double dSD = 0.0;        //�ǵ�
	static double dRise = 0.0;      //�Ƿ�
	static double dLow = 0.0;		//��ͼ�
	static double dHigh = 0.0;		//��߼�
	static __int64 iVolume = 0;	    //�ɽ���(��)
	static double dTurover = 0.0;	//�ɽ���
	static double dTurnoverRate = 0.0;//������

	ReplaceAll(strRecord, "%", "0");
	nRetCode = sscanf_s(strRecord.c_str(), "[\"%d-%d-%d\",\"%lf\", \"%lf\", \"%lf\", \"%lf\", \"%lf\", \"%lf\", \"%I64d\", \"%lf\", \"%lf\"]",
		&nYear, &nMonth, &nDate, &dOpen, &dClose, &dSD, &dRise, &dLow, &dHigh, &iVolume, &dTurover, &dTurnoverRate);
	_ASSERT(nRetCode == 12);

	//["2016-12-30", "16.07", "16.21", "0.14", "0.87%", "16.04", "16.23", "122622", "19765.39", "0.06%"]
	dateMarketData.nDate = nYear * 10000 + nMonth * 100 + nDate;
	dateMarketData.nOpen = (int)(dOpen*10000 + 0.000005);		//���̼ۣ�*10000��ԭֵ��
	dateMarketData.nClose = (int)(dClose * 10000 + 0.000005);   	//���̼ۣ�*10000��ԭֵ��
	dateMarketData.nSD = (int)(dSD * 10000 + 0.000005);        //�ǵ���*10000��ԭֵ��
	dateMarketData.nRise = (int)(dRise * 100 + 0.0005);      //�Ƿ���*100��ԭֵ��(%).
	dateMarketData.nLow = (int)(dLow * 10000 + 0.000005);		//��ͼۣ�*10000��ԭֵ��
	dateMarketData.nHigh = (int)(dHigh * 10000 + 0.000005);		//��߼ۣ�*10000��ԭֵ��
	dateMarketData.iVolume = iVolume;	//�ɽ���(��)
	dateMarketData.iTurover = (int)(dTurover * 10000 + 0.000005);	//�ɽ���(Ԫ)
	dateMarketData.nTurnoverRate = (int)(dTurnoverRate * 100 + 0.0005);//�����ʣ�*100��ԭֵ��(%)

	return nRetCode;
}

int CDayKlineTest::GetDayKData(std::string &strHtml, std::vector<MarketData<int>> &vecMarketData)
{
	//�Ѻ���ʷ�����ֶ�˵��.
	//["2016-12-30", "16.07", "16.21", "0.14", "0.87%", "16.04", "16.23", "122622", "19765.39", "0.06%"]
	//����|���̼�|���̼�|�ǵ�|�Ƿ�|��ͼ�|��߼�|�ɽ�������λ���֣�|�ɽ����λ����|������|

	std::string strRecord;
	char *pOrigStr = (char *)strHtml.c_str();
	///��¼��ʼָ��.
	char *pRecordBPTR = strstr(pOrigStr, ":[[");
	///��¼����ָ��.
	char *pRecordEPTR = nullptr;

	if (pRecordBPTR) {
		pRecordBPTR += 2;
		pRecordEPTR = strstr(pRecordBPTR, "]");
	}

	while (pRecordBPTR && pRecordEPTR)
	{
		strRecord = std::string(pRecordBPTR, pRecordEPTR - pRecordBPTR + 1);

		///��¼��ȡ.
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
	std::string strHtml = _T("");  ///�����ҳ����.
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
			throw std::invalid_argument("��ȡ�����������ݣ�");
		}

		uint32_t unStructItems = (uint32_t)vecMarketData.size();
		uint32_t unSrcDataSize = sizeof(MarketData<int>) * unStructItems;
		uint32_t unDstlen = cr.GetMaxCompSize(unSrcDataSize, unStructItems);

		cout << "ԭʼ���ȣ�" << unSrcDataSize << " | ���ѹ�����ȣ�" << unDstlen << endl;

		uint32_t unComplen = 0;
		uint32_t unDecomplen = unDstlen;
		char *pDst = new char[unDstlen];
		memset(pDst, 0, unDstlen);

		///��β���.
		int nTestTimes = DO_TEST_TIMES;
		if (nTestTimes > 0)
		{
			ULONGLONG ulTatalStartTick = GetTickCount64();
			uint32_t unStructItems = (uint32_t)vecMarketData.size();
			uint32_t unTatalSrcDataSize = sizeof(MarketData<int>) * unStructItems * nTestTimes;
			uint32_t unTatalComplen = 0;

			cout << "ѭ�����Դ�����" << nTestTimes << endl;

			ulStartTick = GetTickCount64();

#ifdef DATA_FEATURE_HANDLE
			BeforeCompress(vecMarketData);
#endif

			for (int i = 0; i < nTestTimes; i++) {
				nRetCode = cr.Compress(vecMarketData, pDst, unDstlen, unComplen);
				CheckRetCode("ѭ��ѹ��", nRetCode);
				unTatalComplen += unComplen;
			}

			nUseTicks = (int)(GetTickCount64() - ulStartTick);
			cout << "ѹ���ۼƺ�ʱ��ms����" << nUseTicks << endl;

			ulStartTick = GetTickCount64();
			for (int i = 0; i < nTestTimes; i++) {
				nRetCode = dcr.Decompress(vecMarketData, pDst, unDstlen/*, unDecomplen*/);
				CheckRetCode("ѭ����ѹ", nRetCode);
			}

#ifdef DATA_FEATURE_HANDLE
			AfterDecompress(vecMarketData);
#endif
			nUseTicks = (int)(GetTickCount64() - ulStartTick);
			cout << "��ѹ�ۼƺ�ʱ��ms����" << nUseTicks << endl;

			nUseTicks = (int)(GetTickCount64() - ulTatalStartTick);
			cout << "ѹ����ѹ�ۼƺ�ʱ��ms����" << nUseTicks << endl;
			cout << "ԭʼ���ȣ�" << unTatalSrcDataSize / (1024 * 1024 * 1.0) <<\
				    "M | ѹ�����ȣ�" << unTatalComplen / (1024 * 1024 * 1.0) <<\
				    "M | ѹ���ȣ�" << unTatalComplen / (unTatalSrcDataSize*1.0) <<\
					" �� " << (unTatalSrcDataSize*1.0) / unTatalComplen << endl;

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
		cout << "ѹ������δ֪�쳣��" << endl;
	}
}

///ѹ��ǰ����.
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

///��ѹ����ѹ��ǰ�����������.
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

	cout << "���������������ݳ��ȣ�" << strHtml.size() << endl;

	std::vector<MarketData<int>> vecTmpMarketData;
	GetDayKData(strHtml, vecTmpMarketData);

	///�򵥷�����������.
	std::vector<MarketData<int>> vecMarketData;
	for (int i = ((int)vecTmpMarketData.size() - 1); i >= 0; i--) {
		vecMarketData.push_back(vecTmpMarketData[i]);
	}

	cout << "************�Ѻ���������ѹ������************" << endl;

	///��������ѹ������.
	DayKlineCompPerformanceTest(vecMarketData);

	cout << "minicc DayKlineTest end." << endl;
	cout << "------------------------------------------------" << endl;
}

