#pragma once
#include "Common.hpp"
#include <string>
#include <vector>

#pragma pack(push, 1)

//�Ѻ���ʷ�����ֶ�˵��.
//["2016-12-30", "16.07", "16.21", "0.14", "0.87%", "16.04", "16.23", "122622", "19765.39", "0.06%"]
//����|���̼�|���̼�|�ǵ�|�Ƿ�|��ͼ�|��߼�|�ɽ�������λ���֣�|�ɽ����λ����|������|
///��������.
template<typename T>
struct MarketData
{
	int     nDate;	    //��������
	int	  	nOpen;		//���̼ۣ�*10000��ԭֵ��
	int	  	nClose;   	//���̼ۣ�*10000��ԭֵ��
	int     nSD;        //�ǵ���*10000��ԭֵ��
	int     nRise;      //�Ƿ���*100��ԭֵ��.
	int	  	nLow;		//��ͼۣ�*10000��ԭֵ��
	int	  	nHigh;		//��߼ۣ�*10000��ԭֵ��
	__int64 iVolume;	//�ɽ���(��)
	__int64	iTurover;	//�ɽ���(Ԫ)
	int     nTurnoverRate;//�����ʣ�*100��ԭֵ��

	META(nDate, nOpen, nClose, nSD, nRise, nLow, nHigh, iVolume, iTurover, nTurnoverRate);
};


#pragma pack(pop)

class CDayKlineTest
{
public:
	CDayKlineTest();
	~CDayKlineTest();

public:
	///��ȡ��ҳ��Ϣ.
	static std::string CDayKlineTest::GetHttpResponse(std::string strURL);
	///��Kѹ������.
	static void DoTest();

private:
	///�滻�ַ���.
	static int ReplaceAll(std::string& str, const std::string& pattern, const std::string& newpat);
	///��ȡ��������.
	static int ParseKDataRecord(std::string &strRecord, MarketData<int> &dateMarketData);
	///��ȡ��������.
	static int GetDayKData(std::string &strHtml, std::vector<MarketData<int>> &vecMarketData);

	///��������ѹ�����ܲ���.
	static void DayKlineCompPerformanceTest(std::vector<MarketData<int>> &vecMarketData);

	///ѹ��ǰ����.
	static void BeforeCompress(std::vector<MarketData<int>> &vecMarketData);
	///��ѹ����ѹ��ǰ�����������.
	static void AfterDecompress(std::vector<MarketData<int>> &vecMarketData);
};

