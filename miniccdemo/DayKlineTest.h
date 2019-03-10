#pragma once
#include "Common.hpp"
#include <string>
#include <vector>

#pragma pack(push, 1)

//搜狐历史数据字段说明.
//["2016-12-30", "16.07", "16.21", "0.14", "0.87%", "16.04", "16.23", "122622", "19765.39", "0.06%"]
//日期|开盘价|收盘价|涨跌|涨幅|最低价|最高价|成交量（单位：手）|成交额（单位：万）|换手率|
///日线数据.
template<typename T>
struct MarketData
{
	int     nDate;	    //数据日期
	int	  	nOpen;		//开盘价（*10000得原值）
	int	  	nClose;   	//收盘价（*10000得原值）
	int     nSD;        //涨跌（*10000得原值）
	int     nRise;      //涨幅（*100得原值）.
	int	  	nLow;		//最低价（*10000得原值）
	int	  	nHigh;		//最高价（*10000得原值）
	__int64 iVolume;	//成交量(手)
	__int64	iTurover;	//成交额(元)
	int     nTurnoverRate;//换手率（*100得原值）

	META(nDate, nOpen, nClose, nSD, nRise, nLow, nHigh, iVolume, iTurover, nTurnoverRate);
};


#pragma pack(pop)

class CDayKlineTest
{
public:
	CDayKlineTest();
	~CDayKlineTest();

public:
	///获取网页信息.
	static std::string CDayKlineTest::GetHttpResponse(std::string strURL);
	///日K压缩测试.
	static void DoTest();

private:
	///替换字符串.
	static int ReplaceAll(std::string& str, const std::string& pattern, const std::string& newpat);
	///获取日线数据.
	static int ParseKDataRecord(std::string &strRecord, MarketData<int> &dateMarketData);
	///获取日线数据.
	static int GetDayKData(std::string &strHtml, std::vector<MarketData<int>> &vecMarketData);

	///日线数据压缩性能测试.
	static void DayKlineCompPerformanceTest(std::vector<MarketData<int>> &vecMarketData);

	///压缩前处理.
	static void BeforeCompress(std::vector<MarketData<int>> &vecMarketData);
	///解压后处理（压缩前处理反向操作）.
	static void AfterDecompress(std::vector<MarketData<int>> &vecMarketData);
};

