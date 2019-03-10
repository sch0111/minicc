#include "stdafx.h"
#include "miniccTest.h"
#include "SimpleCompressor.hpp"
#include "SimpleDecompressor.hpp"

#pragma pack(push, 1)

struct STestStruct
{
	bool bBool;
	unsigned char ucCharVal;
	char cCharVal;
	TCHAR szCharVal[8];
	int	nInt32Val;
	int	arrnInt32Val[5];
	uint32_t unInt32Val;
	int64_t iInt64Val; 
	uint64_t uiInt64Val;
	double dDoubleVal;
	float fFloatVal;
	std::string strVal;
	short sShortVal;

	META(bBool, ucCharVal, cCharVal, szCharVal, nInt32Val, arrnInt32Val, unInt32Val, iInt64Val, uiInt64Val, dDoubleVal, fFloatVal, strVal, sShortVal);
};

#pragma pack(pop)

extern void CheckRetCode(char *szMark, int nRetCode);

CMiniccTest::CMiniccTest()
{
}


CMiniccTest::~CMiniccTest()
{
}

void CMiniccTest::DoTest()
{
	cout << "------------------------------------------------" << endl;
	cout << "minicc common test begin." << endl;

	int nRetCode = -1;

	STestStruct arrTestStruct[2];
	memset(arrTestStruct, 0, sizeof(STestStruct) * 2);

	arrTestStruct[0].bBool = false;
	arrTestStruct[0].ucCharVal = 81;
	arrTestStruct[0].cCharVal = '$';
	//sprintf_s(arrTestStruct[0].szCharVal, "%s", "测试");
	strcpy_s(arrTestStruct[0].szCharVal, "测试");
	//arrTestStruct[0].szCharVal[0] = '$';
	//arrTestStruct[0].szCharVal[1] = '$';
	arrTestStruct[0].nInt32Val = 10;
	for (int i = 0; i < sizeof(arrTestStruct[0].arrnInt32Val) / sizeof(int); i++)
	{
		arrTestStruct[0].arrnInt32Val[i] = i + 1;
		//arrTestStruct[0].arrnInt32Val[i] = rand();
	}
	arrTestStruct[0].unInt32Val = 10;
	arrTestStruct[0].iInt64Val = 10;
	arrTestStruct[0].uiInt64Val = 10;
	arrTestStruct[0].dDoubleVal = 10.2;
	arrTestStruct[0].fFloatVal = 10.2f;
	arrTestStruct[0].strVal = "str1";

	arrTestStruct[1].bBool = false;
	arrTestStruct[1].ucCharVal = 81;
	arrTestStruct[1].cCharVal = '$';
	arrTestStruct[1].szCharVal[0] = '$';
	arrTestStruct[1].szCharVal[1] = '$';
	arrTestStruct[1].nInt32Val = 10;
	for (int i = 0; i < sizeof(arrTestStruct[1].arrnInt32Val) / sizeof(int); i++)
	{
		arrTestStruct[1].arrnInt32Val[i] = i + 1;
		//arrTestStruct[1].arrnInt32Val[i] = rand();
	}
	arrTestStruct[1].unInt32Val = 10;
	arrTestStruct[1].iInt64Val = 10;
	arrTestStruct[1].uiInt64Val = 10;
	arrTestStruct[1].dDoubleVal = 10.2;
	arrTestStruct[1].fFloatVal = 10.2f;
	arrTestStruct[1].strVal = "str1";

	std::vector<STestStruct> container_st;
	container_st.push_back(arrTestStruct[0]);
	container_st.push_back(arrTestStruct[1]);

	cout << "vec_st size:" << container_st.size() << endl;


	minicc::CSimpleCompressor cr;
	minicc::CSimpleDecompressor dcr;

	try
	{
		uint32_t unSrcDataSize = sizeof(STestStruct) * 2;
		uint32_t unStructItems = 2;
		uint32_t unDstlen = cr.GetMaxCompSize(unSrcDataSize, unStructItems);

		cout << "原始长度：" << unSrcDataSize << " | 最大压缩长度：" << unDstlen << endl;

		uint32_t unComplen = 0;
		uint32_t unDecomplen = unDstlen;
		char *pDst = new char[unDstlen];
		memset(pDst, 0, unDstlen);


		nRetCode = cr.Compress(container_st, pDst, unDstlen, unComplen);
		CheckRetCode("单结构体容器压缩", nRetCode);
		memset(&container_st[0], 0, sizeof(STestStruct));
		memset(&container_st[1], 0, sizeof(STestStruct));

		cout << "原始长度：" << unSrcDataSize << " | 压缩长度：" << unComplen << " | 压缩比：" << (unSrcDataSize*1.0) / unComplen << endl;

		nRetCode = dcr.Decompress(container_st, pDst, unDstlen);
		CheckRetCode("单结构体容器解压", nRetCode);

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

	cout << "minicc common test end." << endl;
	cout << "------------------------------------------------" << endl;
}